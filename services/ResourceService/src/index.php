<?php

namespace Islandora\ResourceService;

require_once __DIR__.'/../vendor/autoload.php';

use Silex\Application;
use Islandora\Chullo\FedoraApi;
use Islandora\Chullo\TriplestoreClient;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Psr\Http\Message\ResponseInterface;
use Silex\Provider\TwigServiceProvider;
use Symfony\Component\Yaml\Yaml;

date_default_timezone_set('UTC');

$app = new Application();

$app['debug'] = true;

$app->register(new \Silex\Provider\TwigServiceProvider(), array(
  'twig.path' => __DIR__.'/../templates',
));


$app['twig'] = $app->share($app->extend('twig', function($twig, $app) {
  return $twig;
}));

$app['api'] =  $app->share(function() use ($app) {
  return FedoraApi::create($app['config']['islandora']['fedoraProtocol'].'://'.$app['config']['islandora']['fedoraHost'].$app['config']['islandora']['fedoraPath']);
});

$app['triplestore'] = $app->share(function() use ($app) {
  return TriplestoreClient::create($app['config']['islandora']['tripleProtocol'].'://'.$app['config']['islandora']['tripleHost'].$app['config']['islandora']['triplePath']);
});

/**
 * Ultra simplistic YAML settings loader.
 */
$app['config'] = $app->share(function() use ($app){
  if ($app['debug']) {
    $configFile = __DIR__.'/../config/settings.dev.yml';
  }
  else {
    $configFile = __DIR__.'/../config/settings.yml';
  }
  $settings = Yaml::parse(file_get_contents($configFile));
  return $settings;
});

/**
 * before middleware to handle browser requests.
 */
$htmlHeaderToTurtle = function(Request $request) {
  // In case the request was made by a browser, avoid 
  // returning the whole Fedora4 API Rest interface page.
  if (0 === strpos($request->headers->get('Accept'),'text/html')) {
    $request->headers->set('Accept', 'text/turtle', TRUE);
  }
};


/**
 * before middleware to normalize host header to same as fedora's running
 * instance. 
 */
$hostHeaderNormalize  = function(Request $request) use ($app) {
  // Normalize Host header to Repo's real location
  $request->headers->set('Host', $app['config']['islandora']['fedoraHost'], TRUE);
};

/**
 * Converts request $id (uuid) into a fedora4 resourcePath
 */
$idToUri = function ($id) use ($app) {
  // Run only if $id given /can also be refering root resource,
  // we accept only UUID V4 or empty
  if (NULL != $id) {
    $sparql_query = $app['twig']->render('getResourceByUUIDfromTS.sparql', array(
      'uuid' => $id,
    ));
     try {
       $sparql_result = $app['triplestore']->query($sparql_query);
     }
     catch (\Exception $e) {
       $app->abort(503, 'Chullo says "Triple Store Not available"');
     }
    // We only assign one in case of multiple ones
    // Will have to check for edge cases?
    foreach ($sparql_result as $triple) {
      return $triple->s->getUri();
    }
    // Abort the routes if we don't get a subject from the tripple.
    $app->abort(404, sprintf('Failed getting resource Path for "%s" from triple store', $id));
  } 
  else {
    // If $id is empty then assume we are dealing with fedora base rest endpoint
    return $app['config']['islandora']['fedoraProtocol'].'://'.$app['config']['islandora']['fedoraHost'].$app['config']['islandora']['fedoraPath'];
  }
    
};

/**
 * Convert returned Guzzle responses to Symfony responses.
 */
$app->view(function (ResponseInterface $psr7) {
  return new Response($psr7->getBody(), $psr7->getStatusCode(), $psr7->getHeaders());
});

/**
 * Resource GET route. takes $id (valid UUID or empty) as first value to match, optional a child resource path
 * takes 'rx' and/or 'metadata' as optional query arguments
 * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-GETRetrievethecontentoftheresource
 */
$app->get("/islandora/resource/{id}/{child}",function (Application $app, Request $request, $id, $child) {
   $tx = $request->query->get('tx', "");
   $metadata = $request->query->get('metadata', FALSE) ? '/fcr:metadata' : ""; 
   try {
     $response = $app['api']->getResource($app->escape($id).'/'.$child.$metadata, $request->headers->all(), $tx);
   }
   catch (\Exception $e) {
     $app->abort(503, 'Chullo says "Fedora4 Repository Not available"');
   }
   return $response;
})
->value('child',"")
->value('id',"")
->convert('id', $idToUri)
->assert('id',$app['config']['islandora']['resourceIdRegex'])
->before($htmlHeaderToTurtle) 
->before($hostHeaderNormalize); 

/**
 * Resource POST route. takes $id (valid UUID or empty) for the parent resource as first value to match
 * takes 'rx' and/or 'checksum' as optional query arguments
 * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-BluePOSTCreatenewresourceswithinaLDPcontainer
 */
$app->post("/islandora/resource/{id}",function (Application $app, Request $request, $id, $checksum) {
   $tx = $request->query->get('tx', "");
   $checksum = $request->query->get('checksum', "");
   try {
     $response = $app['api']->createResource($app->escape($id), $request->getContent(), $request->headers->all(), $tx, $checksum);
   }
   catch (\Exception $e) {
     $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
   }
   return $response;
})
->value('id',"")
->assert('id',$app['config']['islandora']['resourceIdRegex'])
->before($htmlHeaderToTurtle)
->before($hostHeaderNormalize);

/**
 * Resource PUT route. takes $id (valid UUID or empty) for the resource to be update/created as first value to match, 
 * optional a Child resource relative path
 * takes 'rx' and/or 'checksum' as optional query arguments
 * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-YellowPUTCreatearesourcewithaspecifiedpath,orreplacethetriplesassociatedwitharesourcewiththetriplesprovidedintherequestbody.
 */
$app->put("/islandora/resource/{id}/{child}",function (Application $app, Request $request, $id, $child) {
   $tx = $request->query->get('tx', "");
   $checksum = $request->query->get('checksum', "");
   try {
     $response = $app['api']->saveResource($app->escape($id).'/'.$child, $request->getContent(), $request->headers->all(), $tx, $checksum);
   }
   catch (\Exception $e) {
     $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
   }
   return $response;
})
->value('child',"")
->assert('id',$app['config']['islandora']['resourceIdRegex'])
->before($htmlHeaderToTurtle)
->before($hostHeaderNormalize);

/**
* Resource PATCH route. takes $id (valid UUID or empty) for the resource to be modified via SPARQL as first value to match, 
* optional a Child resource relative path
* takes 'rx' as optional query argument
* @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-GreenPATCHModifythetriplesassociatedwitharesourcewithSPARQL-Update
*/
$app->patch("/islandora/resource/{id}/{child}",function (Application $app, Request $request, $id, $child) {
  $tx = $request->query->get('tx', "");
  try {
    $response = $app['api']->modifyResource($app->escape($id).'/'.$child, $request->getContent(), $request->headers->all(), $tx);
  }
  catch (\Exception $e) {
    $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
  }
  return $response;
})
->value('child',"")
->assert('id',$app['config']['islandora']['resourceIdRegex'])
->before($htmlHeaderToTurtle)
->before($hostHeaderNormalize);

/**
* Resource DELETE route. takes $id (valid UUID or empty) for the resource to be modified via SPARQL as first value to match, 
* optional a Child resource relative path
* takes 'rx' as optional query argument, also 'force' to remove the tombstone in one step
* @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-RedDELETEDeletearesource
*/
$app->delete("/islandora/resource/{id}/{child}",function (Application $app, Request $request, $id, $child) {
  $tx = $request->query->get('tx', "");
  $force = $request->query->get('force', FALSE);
  try {
    $response = $app['api']->deleteResource($app->escape($id).'/'.$child, $tx);
    //remove tombstone also if 'force' == true and previous response is 204
    if ((204 == $response->getStatusCode() || 410 == $response->getStatusCode()) && $force) {
      $response= $app['api']->deleteResource($app->escape($id).'/'.$child.'/fcr:tombstone', $tx);
    }
  }
  catch (\Exception $e) {
    $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
  }
  return $response;
})
->value('child',"")
->assert('id',$app['config']['islandora']['resourceIdRegex'])
->before($htmlHeaderToTurtle)
->before($hostHeaderNormalize);

$app->after(function (Request $request, Response $response, Application $app) {
  // Todo a closing controller, not sure what now but i had an idea.
});
$app->error(function (\Symfony\Component\HttpKernel\Exception\HttpException $e, $code) use ($app){
  if ($app['debug']) {
    return;
  }
  return new response(sprintf('Islandora Resource Service exception: %s / HTTP %d response', $e->getMessage(), $code), $code);
});
$app->error(function (\Symfony\Component\HttpKernel\Exception\NotFoundHttpException $e, $code) use ($app){
  if ($app['debug']) {
    return;
  }
  //Not sure what the best "verbose" message is
  return new response(sprintf('Islandora Resource Service exception: %s / HTTP %d response', $e->getMessage(), $code), $code);
});
$app->error(function (\Exception $e, $code) use ($app){
  if ($app['debug']) {
    return;
  }  
  return new response(sprintf('Islandora Resource Service uncatched exception: %s %d response', $e->getMessage(), $code), $code);
});


$app->run();
