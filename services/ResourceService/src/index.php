<?php

namespace Islandora\ResourceService;

require_once __DIR__.'/../vendor/autoload.php';

use Silex\Application;
use Islandora\Chullo\Chullo;
use Islandora\Chullo\TriplestoreClient;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Psr\Http\Message\ResponseInterface;
use Silex\Provider\TwigServiceProvider;
use Symfony\Bridge\PsrHttpMessage\Factory\HttpFoundationFactory;


date_default_timezone_set('UTC');

$app = new \Silex\Application();

$app['debug'] = true;

$app->register(new \Silex\Provider\TwigServiceProvider(), array(
    'twig.path' => __DIR__.'/../templates',
));

$app['twig'] = $app->share($app->extend('twig', function($twig, $app) {
    return $twig;
}));

$app['fedora'] =  $app->share(function() {
    return Chullo::create('http://localhost:8080/rest');
});

$app['triplestore'] = $app->share(function() {
  return TriplestoreClient::create('http://localhost:9999/bigdata/sparql');
});
$app['data.resourcepath'] = NULL;

/**
 * Convert returned Guzzle responses to Symfony responses.
 */
$app->view(function (ResponseInterface $psr7) {
  return new Response($psr7->getBody(), $psr7->getStatusCode(), $psr7->getHeaders());
});
/**
 * resource GET route. takes an UUID as first value to match, optional a child resource
 */
$app->get("/islandora/resource/{uuid}/{child}",function (\Silex\Application $app, Request $request, $uuid, $child) {
   if (NULL === $app['data.resourcepath']) {
     $app->abort(404, 'Failed getting resource Path for {$uuid}');
   } 
   $tx = $request->query->get('tx',"");
   $metadata = $request->query->get('metadata',FALSE) ? '/fcr:metadata' : ""; 
   $response = $app['fedora']->getResource($app->escape($app['data.resourcepath']) . '/' . $child . $metadata , $request->headers->all(), $tx);
   if (NULL === $response )
     {
       $app->abort(404, 'Failed getting resource from Fedora4');
     }
   return $response;
})
->value('child',"")
->assert('uuid','([a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12})')
->before(function (Request $request) use ($app) {
  // In case the request was made by a browser, avoid 
  // returning the whole Fedora4 API Rest interface page.
    if (0 === strpos($request->headers->get('Accept'),'text/html')) {
      $request->headers->set('Accept', 'text/turtle', TRUE);
    }
    $sparql_query = $app['twig']->render('getResourceByUUIDfromTS.sparql', array(
      'uuid' => $request->attributes->get('uuid'),
    ));
   
    $sparql_result = $app['triplestore']->query($sparql_query);
    foreach ($sparql_result  as $triple) {
        $app['data.resourcepath'] = $triple->s->getUri();
    }

});

$app->after(function (Request $request, Response $response, \Silex\Application $app) {
});



$app->run();
