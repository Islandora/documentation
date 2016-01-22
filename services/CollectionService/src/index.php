<?php

namespace Islandora\CollectionService;

require_once __DIR__.'/../vendor/autoload.php';

use Silex\Application;
use Islandora\Chullo\FedoraApi;
use Islandora\Chullo\TriplestoreClient;
use Islandora\ResourceService\Provider\ResourceServiceProvider;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Psr\Http\Message\ResponseInterface;
use Silex\Provider\TwigServiceProvider;
use Symfony\Component\Yaml\Yaml;

date_default_timezone_set('UTC');

$app = new Application();

$app['debug'] = true;
$app->register(new \Silex\Provider\ServiceControllerServiceProvider());
$app->register(new \Silex\Provider\TwigServiceProvider(), array(
  'twig.path' => __DIR__.'/../templates',
));

$app['twig'] = $app->share($app->extend('twig', function($twig, $app) {
  return $twig;
}));
$islandoraResourceServiceProvider = new \Islandora\ResourceService\Provider\ResourceServiceProvider;

$app->register($islandoraResourceServiceProvider);
$app->mount("/islandora", $islandoraResourceServiceProvider);
$app->register(new \Islandora\ResourceService\Provider\UuidServiceProvider(), array(
  'UuidServiceProvider.default_namespace' => $app['config']['islandora']['defaultNamespaceDomainUuuidV5'],
));

$app['uuid'] = $app['islandora.uuid4'];

$isFedora4Content = function (Request $request) use ($app) {
  $rdf_content_types = array(
    "text/turtle",
    "text/rdf+n3",
    "application/n3",
    "text/n3",
    "application/rdf+xml",
    "application/n-triples",
    "application/sparql-update"
  );
  if (in_array($request->headers->get('Content-type'), $rdf_content_types)) {
    return true;
  }
  return false;
}

/**
 * Convert returned Guzzle responses to Symfony responses.
 */
$app->view(function (ResponseInterface $psr7) {
  return new Response($psr7->getBody(), $psr7->getStatusCode(), $psr7->getHeaders());
});

/**
 * Resource POST route. takes $id (valid UUID or empty) for the parent resource as first value to match
 * takes 'rx' and/or 'checksum' as optional query arguments
 * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-BluePOSTCreatenewresourceswithinaLDPcontainer
 */
$app->post("/islandora/collection",function (Application $app, Request $request) {
  
   error_log($app['uuid']);
   $tx = $request->query->get('tx', "");
   $url = $request->getUriForPath('/islandora/resource/');
   //static public Request create(string $uri, string $method = 'GET', array $parameters = array(), array $cookies = array(), array $files = array(), array $server = array(), string $content = null)
   $subRequest = Request::create($url, 'POST', array(), $request->cookies->all(), $request->files->all(), $request->server->all());
   error_log($subRequest->__toString());
   //$response = $app->handle($subRequest, HttpKernelInterface::SUB_REQUEST, false);
   return "ok";//$response;
})

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
