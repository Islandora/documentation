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

  /**
   * Parse a response to get the transaction ID.
   *
   * @param $response
   *   Either a Symfony or Guzzle/Psr7 response.
   * @return string
   *   The transaction ID.
   */
  public function getId($response) {
    if (get_class($response) == 'Symfony\Component\HttpFoundation\Response') {
      if ($response->headers->has('location')) {
        return $this->parseTransactionId($response->headers->get('location'));
      }
    }
    if (get_class($response) == 'GuzzleHttp\Psr7\Response') {
      if ($response->hasHeader('location')) {
        return $this->parseTransactionId($response->getHeader('location'));
      }
    }
    return NULL;
  }

  /**
   * Utility function to get the transaction ID from the Header.
   *
   * @param array|string $header
   *   array of headers or the single string.
   * @return string
   *   the transaction ID.
   */
  private function parseTransactionId($header) {
    if (is_array($header)) {
      $header = reset($header);
    }
    $ids = explode('tx:', $header);
    return 'tx:' . $ids[1];
  }
}
