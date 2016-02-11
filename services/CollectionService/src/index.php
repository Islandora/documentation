<?php

namespace Islandora\CollectionService;

require_once __DIR__.'/../vendor/autoload.php';

use Silex\Application;
use Islandora\ResourceService\Provider\ResourceServiceProvider;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Psr\Http\Message\ResponseInterface;
use Silex\Provider\TwigServiceProvider;

date_default_timezone_set('UTC');

$app = new Application();

$app['debug'] = true;
$app->register(new \Silex\Provider\ServiceControllerServiceProvider());
$app->register(new \Silex\Provider\TwigServiceProvider(), array(
  'twig.path' => __DIR__.'/../templates',
));

$islandoraResourceServiceProvider = new \Islandora\ResourceService\Provider\ResourceServiceProvider;

//Registers Resource Service and defines current app's path for config context
$app->register($islandoraResourceServiceProvider, array(
  'islandora.BasePath' => __DIR__,
));
$app->mount("/islandora", $islandoraResourceServiceProvider);
$app->register(new \Islandora\ResourceService\Provider\UuidServiceProvider(), array(
  'UuidServiceProvider.default_namespace' => $app['config']['islandora']['defaultNamespaceDomainUuuidV5'],
));

//We will use a static uuid5 to fake a real IRI to replace <> during turtle parsing
$app['uuid5'] = $app->share(function () use ($app) {
  return $app['islandora.uuid5']($app['config']['islandora']['fedoraProtocol'].'://'.$app['config']['islandora']['fedoraHost'].$app['config']['islandora']['fedoraPath']);
});
//This is the uuidV4 generator
$app['uuid'] = $app['islandora.uuid4'];

/**
 * Convert returned Guzzle responses to Symfony responses.
 */

$app->view(function (ResponseInterface $psr7) {
  return new Response($psr7->getBody(), $psr7->getStatusCode(), $psr7->getHeaders());
});

/**
 * Collection POST route. 
 * Takes $id (valid UUID or empty) for the parent resource as first value to match, 
 * and also takes 'rx' as an optional query argument.
 */
$app->post("/islandora/collection/{id}", function (Request $request, $id) use ($app) {
  $tx = $request->query->get('tx', "");

  //Check for format
  $format = NULL;
  try {
    $format = \EasyRdf_Format::getFormat($contentType = $request->headers->get('Content-Type', 'text/turtle'));
  } catch (\EasyRdf_Exception $e) {
    $app->abort(415, $e->getMessage());
  }

  //Now check if body can be parsed in that format
  if ($format) { //EasyRdf_Format
    //@see http://www.w3.org/2011/rdfa-context/rdfa-1.1 for defaults
    \EasyRdf_Namespace::set('pcdm', 'http://pcdm.org/models#');
    \EasyRdf_Namespace::set('nfo', 'http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/');
    \EasyRdf_Namespace::set('isl', 'http://www.islandora.ca/ontologies/2016/02/28/isl/v1.0/');
    \EasyRdf_Namespace::set('ldp', 'http://www.w3.org/ns/ldp');

    //Fake IRI, default LDP one for current resource "<>" is not a valid IRI!
    $fakeIri = new \EasyRdf_ParsedUri('urn:uuid:'.$app['uuid5']);

    $graph = new \EasyRdf_Graph();
    try {
      $graph->parse($request->getContent(), $format->getName(), $fakeIri);
    } catch (\EasyRdf_Exception $e) {
      $app->abort(415, $e->getMessage());
    }
    //Add a pcmd:Collection type
    $graph->resource($fakeIri, 'pcdm:Collection');

    //Check if we got an UUID inside posted RDF. We won't validate it here because it's the caller responsability
    if (NULL != $graph->countValues($fakeIri, '<http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/uuid>')) {
      $existingUuid = $graph->getLiteral($fakeIri, '<http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/uuid>');
      $graph->addResource($fakeIri, 'http://www.islandora.ca/ontologies/2016/02/28/isl/v1.0/hasURN', 'urn:uuid:'.$existingUuid); //Testing an Islandora Ontology!
    } else {
      //No UUID from the caller in RDF, lets put something there
      $tmpUuid = $app['uuid']; //caching here, since it's regenerated each time it is used and I need the same twice
      $graph->addLiteral($fakeIri,"http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/uuid",$tmpUuid); //Keeps compat for now with other services
      $graph->addResource($fakeIri,"http://www.islandora.ca/ontologies/2016/02/28/isl/v1.0/hasURN",'urn:uuid:'.$tmpUuid); //Testing an Islandora Ontology
    }
    //Restore LDP <> IRI on serialised graph
    $pcmd_collection_rdf= str_replace($fakeIri, '', $graph->serialise('turtle'));
  }

  $urlRoute = $request->getUriForPath('/islandora/resource/');
 
  $subRequestPost = Request::create($urlRoute.$id, 'POST', array(), $request->cookies->all(), array(), $request->server->all(), $pcmd_collection_rdf);
  $subRequestPost->query->set('tx', $tx);
  $subRequestPost->headers->set('Content-Type', 'text/turtle');
  $responsePost = $app->handle($subRequestPost, HttpKernelInterface::SUB_REQUEST, false);

  if (201 == $responsePost->getStatusCode()) {// OK, collection created
    //Lets take the location header in the response
    $indirect_container_rdf = $app['twig']->render('createIndirectContainerfromTS.ttl', array(
      'resource' => $responsePost->headers->get('location'),
    ));

    $subRequestPut = Request::create($urlRoute.$id, 'PUT', array(), $request->cookies->all(), array(), $request->server->all(), $indirect_container_rdf);
    $subRequestPut->query->set('tx', $tx);
    $subRequestPut->headers->set('Slug', 'members');
    //Can't use in middleware, but needed. Without Fedora 4 throws big java errors!
    $subRequestPut->headers->set('Host', $app['config']['islandora']['fedoraHost'], TRUE);
    $subRequestPut->headers->set('Content-Type', 'text/turtle');
    //Here is the thing. We don't know if UUID of the collection we just created is already in the tripple store.
    //So what to do? We could just try to use our routes directly, but UUID check agains triplestore we could fail!
    //lets invoke the controller method directly
    $responsePut = $app['islandora.resourcecontroller']->put($app, $subRequestPut, $responsePost->headers->get('location'), "members");
    if (201 == $responsePut->getStatusCode()) {// OK, indirect container created
      //Return only the last created resource
      $putHeaders = $responsePut->getHeaders();
      //Guzzle psr7 response objects are inmutable. So we have to make this an array and add directly
      $putHeaders['Link'] = array('<'.$urlRoute.$tmpUuid.'/members>; rel="alternate"');

      return new Response($responsePut->getBody(), 200, $putHeaders);
    }

    return $responsePut;
  }
  //Abort if PCDM collection object could not be created
  $app->abort($responsePost->getStatusCode(), 'Failed creating PCDM Collection');
})
->value('id',"");

$app->after(function (Request $request, Response $response) use ($app) {
  $response->headers->set('X-Powered-By', 'Islandora Collection REST API v'.$app['config']['islandora']['apiVersion'], TRUE); //Nice

});

//Common error Handling
$app->error(function (\EasyRdf_Exception $e, $code) use ($app) {
  if ($app['debug']) {
    return;
  }

  return new response(sprintf('RDF Library exception', $e->getMessage(), $code), $code);
});
$app->error(function (\Symfony\Component\HttpKernel\Exception\HttpException $e, $code) use ($app) {
  if ($app['debug']) {
    return;
  }

  return new response(sprintf('Islandora Collection Service exception: %s / HTTP %d response', $e->getMessage(), $code), $code);
});
$app->error(function (\Symfony\Component\HttpKernel\Exception\NotFoundHttpException $e, $code) use ($app) {
  if ($app['debug']) {
    return;
  }
  //Not sure what the best "verbose" message is
  return new response(sprintf('Islandora Collection Service exception: %s / HTTP %d response', $e->getMessage(), $code), $code);
});
$app->error(function (\Exception $e, $code) use ($app) {
  if ($app['debug']) {
    return;
  }

  return new response(sprintf('Islandora Collection Service uncatched exception: %s %d response', $e->getMessage(), $code), $code);
});

$app->run();
