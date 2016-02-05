<?php

namespace Islandora\TransactionService\Controller;

use Silex\Application;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;

class TransactionController {

  public function create(Application $app, Request $request) {
    return $app['api']->createTransaction();
  }

  public function extend(Application $app, Request $request, $id) {
    print "Extending id $id\n";
    return $app['api']->extendTransaction($id);
  }

  public function commit(Application $app, Request $request, $id) {
    return $app['api']->commitTransaction($id);
  }

  public function rollback(Application $app, Request $request, $id) {
    return $app['api']->rollbackTransaction($id);
  }

}
