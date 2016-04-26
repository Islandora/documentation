<?php

namespace Islandora\ResourceService\Controller;

use Silex\Application;
use Symfony\Component\HttpFoundation\Request;

class ResourceController
{
  /**
   * Resource GET controller takes $id (valid UUID or empty) as first value to match, optional a child resource path
   * takes 'rx' and/or 'metadata' as optional query arguments
   * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-GETRetrievethecontentoftheresource
   */
  public function get(Application $app, Request $request, $id, $child)
  {
    $tx = $request->query->get('tx', "");
    $metadata = $request->query->get('metadata', false) ? '/fcr:metadata' :
    "";
    try {
      $response = $app['api']->getResource($app
          ->escape($id) . '/' . $child . $metadata, $request->headers->all(),

        $tx);
    } catch (\Exception $e) {
      $app->abort(503, 'Chullo says "Fedora4 Repository Not available"');
    }
    return $response;
  }
  /**
   * Resource POST route controller. takes $id (valid UUID or empty) for the parent resource as first value to match
   * takes 'rx' and/or 'checksum' as optional query arguments
   * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-BluePOSTCreatenewresourceswithinaLDPcontainer
   */
  public function post(Application $app, Request $request, $id)
  {
    $tx = $request->query->get('tx', "");
    $checksum = $request->query->get('checksum', "");
    try {
      $response = $app['api']->createResource($app->escape($id), $request
          ->getContent(), $request->headers->all(), $tx, $checksum);
    } catch (\Exception $e) {
      $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
    }
    return $response;
  }
  /**
   * Resource PUT route. takes $id (valid UUID or empty) for the resource to be update/created as first value to match,
   * optional a Child resource relative path
   * takes 'rx' and/or 'checksum' as optional query arguments
   * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-YellowPUTCreatearesourcewithaspecifiedpath,orreplacethetriplesassociatedwitharesourcewiththetriplesprovidedintherequestbody.
   */
  public function put(Application $app, Request $request, $id, $child)
  {
    $tx = $request->query->get('tx', "");
    $checksum = $request->query->get('checksum', "");
    try {
      $response = $app['api']->saveResource($app->escape($id) . '/' . $child,
        $request->getContent(), $request->headers->all(), $tx, $checksum);
    } catch (\Exception $e) {
      $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
    }
    return $response;
  }
  public function patch(Application $app, Request $request, $id, $child)
  {
    $tx = $request->query->get('tx', "");
    try {
      $response = $app['api']->modifyResource($app->escape($id) . '/' . $child,
        $request->getContent(), $request->headers->all(), $tx);
    } catch (\Exception $e) {
      $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
    }
    return $response;
  }
  /**
   * Resource DELETE route controller. takes $id (valid UUID) for the parent resource as first value to match
   * takes 'rx' and/or 'checksum' as optional query arguments
   * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-RedDELETEDeletearesource
   * @todo check for transaction and create one if empty.
   * @todo test with the force (is strong with this one).
   */
  public function delete(Application $app, Request $request, $id, $child)
  {
    $tx = $request->query->get('tx', "");
    $force = $request->query->get('force', false);

    error_log('---START OF DELETE RESOURCE REQUEST: ');
    error_log($app->escape($id) . '/' . $child);
    $delete_queue = array($app->escape($id) . '/' . $child);
    $sparql_query = $app['twig']->render('findAllOreProxy.sparql', array(
      'resource' => $id,
    ));
    try {
      $sparql_result = $app['triplestore']->query($sparql_query);
    } catch (\Exception $e) {
      $app->abort(503, 'Chullo says "Triple Store Not available"');
    }
    if (count($sparql_result) > 0) {
      foreach ($sparql_result as $ore_proxy) {
        $delete_queue[] = $ore_proxy->obj->getUri();
      }
    }
    error_log(implode(',', $delete_queue));
    $response = '';
    // To the reader: I'm leaving this try outside of the foreach since we're doing fedora connection checking.
    try {
      foreach ($delete_queue as $object_uri) {
        $response = $app['api']->deleteResource($object_uri, $tx);
        $status = $response->getStatusCode();
        // Abort if we do not get a success (204 or 410?) from Fedora.
        if (204 != $status && 410 != $status) {
          $app->abort(503, 'Could not delete resource or proxy at ' .
            $object_uri);
        }
        // Remove fcr:tombstone if 'force' is true.
        if ($force) {
          $response = $app['api']->deleteResource($object_uri .
            '/fcr:tombstone', $tx);
        }
      }
    } catch (\Exception $e) {
      $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
    }
    // Return the last response since, in theory, if we've come this far we've removed everything.
    // If we don't get this far its because we never got a 204/410 or Fedora is down.
    return $response;
  }
}
