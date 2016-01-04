<?php

namespace Islandora\TransactionService;

require_once __DIR__.'/../vendor/autoload.php';

use GuzzleHttp\Exception\ClientException;
use Islandora\Chullo\FedoraApi;
use Silex\Application;
use Symfony\Component\HttpFoundation\Response;
use Psr\Http\Message\ResponseInterface;

date_default_timezone_set('UTC');

$app = new Application();

$app['debug'] = true;

$app['fedora'] = $app->share(function () {
    return FedoraApi::create('http://127.0.0.1:8080/fcrepo/rest');
});

/**
 * Convert returned Guzzle responses to Symfony responses.
 */
$app->view(function (ResponseInterface $psr7) {
    return new Response($psr7->getBody(), $psr7->getStatusCode(), $psr7->getHeaders());
});

$app->post(
    "/islandora/transaction",
    function (Application $app) {
        return $app['fedora']->createTransaction();
    }
);

$app->post(
    "/islandora/transaction/{id}/extend",
    function (Application $app, $id) {
        return $app['fedora']->extendTransaction($id);
    }
);

$app->post(
    "/islandora/transaction/{id}/commit",
    function (Application $app, $id) {
        return $app['fedora']->commitTransaction($id);
    }
);

$app->post(
    "/islandora/transaction/{id}/rollback",
    function (Application $app, $id) {
        return $app['fedora']->rollbackTransaction($id);
    }
);

$app->run();
