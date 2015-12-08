<?php

namespace Islandora\TransactionService;

require_once __DIR__.'/../vendor/autoload.php';

use GuzzleHttp\Client;
use GuzzleHttp\Exception\ClientException;
use Islandora\Chullo\Chullo;
use Silex\Application;
use Symfony\Component\HttpFoundation\Response;

date_default_timezone_set('UTC');

$app = new Application();

$app['debug'] = true;

$app['fedora'] = function () use ($app) {
    $client = new Client(['base_uri' => 'http://127.0.0.1:8080/fcrepo/rest']);
    return new Chullo($client);
};

$app->post(
    "/islandora/transaction",
    function (Application $app) {
        try {
            return new Response($app['fedora']->createTransaction(), 201);
        }
        catch (ClientException $e) {
            $response = $e->getResponse();
            $app->abort($response->getStatusCode(), $response->getReasonPhrase());
        }
    }
);

$app->post(
    "/islandora/transaction/{id}/extend",
    function (Application $app, $id) {
        try {
            $app['fedora']->extendTransaction($id);
            return new Response("", 204);
        }
        catch (ClientException $e) {
            $response = $e->getResponse();
            $app->abort($response->getStatusCode(), $response->getReasonPhrase());
        }
    }
);

$app->post(
    "/islandora/transaction/{id}/commit",
    function (Application $app, $id) {
        try {
            $app['fedora']->commitTransaction($id);
            return new Response("", 204);
        }
        catch (ClientException $e) {
            $response = $e->getResponse();
            $app->abort($response->getStatusCode(), $response->getReasonPhrase());
        }
    }
);

$app->post(
    "/islandora/transaction/{id}/rollback",
    function (Application $app, $id) {
        try {
            return $app['fedora']->rollbackTransaction($id);
            return new Response("", 204);
        }
        catch (ClientException $e) {
            $response = $e->getResponse();
            $app->abort($response->getStatusCode(), $response->getReasonPhrase());
        }
    }
);

$app->run();
