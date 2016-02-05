<?php

namespace Islandora\TransactionService\Controller;

use Silex\Application;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;

class TransactionController {

  public function create(Application $app, Request $request) {
    try {
      $response = $app['api']->createTransaction();
     } catch (\Exception $e) {
       $app->abort(503, 'Chullo says "Fedora4 Repository Not available"');
     }
     return $response;
  }

  public function extend(Application $app, Request $request, $id) {
    try {
      $response = $app['api']->extendTransaction($id);
     } catch (\Exception $e) {
       $app->abort(503, 'Chullo says "Fedora4 Repository Not available"');
     }
     return $response;
  }

  public function commit(Application $app, Request $request, $id) {
    try {
      $response = $app['api']->commitTransaction($id);
    } catch (\Exception $e) {
      $app->abort(503, 'Chullo says "Fedora4 Repository Not available"');
    }
    return $response;
  }

  public function rollback(Application $app, Request $request, $id) {
    try {
      $response = $app['api']->rollbackTransaction($id);
     } catch (\Exception $e) {
       $app->abort(503, 'Chullo says "Fedora4 Repository Not available"');
     }
     return $response;
  }

}
