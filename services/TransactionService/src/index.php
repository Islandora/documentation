<?php

namespace Islandora\TransactionService;

require_once __DIR__.'/../vendor/autoload.php';

use GuzzleHttp\Exception\ClientException;
use Islandora\Chullo\FedoraApi;
use Silex\Application;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Psr\Http\Message\ResponseInterface;

date_default_timezone_set('UTC');

$app = new Application();

$app['debug'] = true;

$app->register(new \Silex\Provider\ServiceControllerServiceProvider());

$islandoraTransactionService = new \Islandora\TransactionService\Provider\TransactionServiceProvider;

$app->register($islandoraTransactionService);
$app->mount("/islandora", $islandoraTransactionService);

$app['fedora'] = $app->share(function () {
    return FedoraApi::create('http://127.0.0.1:8080/fcrepo/rest');
});

/**
 * Convert returned Guzzle responses to Symfony responses.
 */
$app->view(function (ResponseInterface $psr7) {
    return new Response($psr7->getBody(), $psr7->getStatusCode(), $psr7->getHeaders());
});

/**
 * Convert returned Guzzle responses to Symfony responses.
 */
$app->view(function (ResponseInterface $psr7) {
    return new Response($psr7->getBody(), $psr7->getStatusCode(), $psr7->getHeaders());
});

$app->after(function (Request $request, Response $response, Application $app) {
  // Todo a closing controller, not sure what now but i had an idea.
});
$app->error(function (\Symfony\Component\HttpKernel\Exception\HttpException $e, $code) use ($app){
  if ($app['debug']) {
    return;
  }
  return new Response(sprintf('Islandora Transaction Service exception: %s / HTTP %d response', $e->getMessage(), $code), $code);
});
$app->error(function (\Symfony\Component\HttpKernel\Exception\NotFoundHttpException $e, $code) use ($app){
  if ($app['debug']) {
    return;
  }
  //Not sure what the best "verbose" message is
  return new Response(sprintf('Islandora Transaction Service exception: %s / HTTP %d response', $e->getMessage(), $code), $code);
});
$app->error(function (\Exception $e, $code) use ($app){
  if ($app['debug']) {
    return;
  }  
  return new Response(sprintf('Islandora Transaction Service uncatched exception: %s %d response', $e->getMessage(), $code), $code);
});

$app->run();
