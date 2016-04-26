<?php

namespace Islandora\ResourceService\Controller;
use Silex\Application;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;

class ResourceController {
  /**
   * Resource GET controller takes $id (valid UUID or empty) as first value to match, optional a child resource path
   * takes 'rx' and/or 'metadata' as optional query arguments
   * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-GETRetrievethecontentoftheresource
   */
   public function get(Application $app, Request $request, $id, $child) {
     $tx = $request->query->get('tx', "");
     $metadata = $request->query->get('metadata', FALSE) ? '/fcr:metadata' : ""; 
     try {
       $response = $app['api']->getResource($app->escape($id).'/'.$child.$metadata, $request->headers->all(), $tx);
     }
     catch (\Exception $e) {
       $app->abort(503, 'Chullo says "Fedora4 Repository Not available"');
     }
     return $response;
  }
  /**
   * Resource POST route controller. takes $id (valid UUID or empty) for the parent resource as first value to match
   * takes 'rx' and/or 'checksum' as optional query arguments
   * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-BluePOSTCreatenewresourceswithinaLDPcontainer
   */
  public function post(Application $app, Request $request, $id) {
     $tx = $request->query->get('tx', "");
     $checksum = $request->query->get('checksum', "");
     try {
       $response = $app['api']->createResource($app->escape($id), $request->getContent(), $request->headers->all(), $tx, $checksum);
     }
     catch (\Exception $e) {
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
  public function put(Application $app, Request $request, $id, $child) {
     $tx = $request->query->get('tx', "");
     $checksum = $request->query->get('checksum', "");
     try {
       $response = $app['api']->saveResource($app->escape($id).'/'.$child, $request->getContent(), $request->headers->all(), $tx, $checksum);
     }
     catch (\Exception $e) {
       $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
     }
     return $response;
  }
  public function patch(Application $app, Request $request, $id, $child) {
    $tx = $request->query->get('tx', "");
    try {
      $response = $app['api']->modifyResource($app->escape($id).'/'.$child, $request->getContent(), $request->headers->all(), $tx);
    }
    catch (\Exception $e) {
      $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
    }
    return $response;
  }
    /**
   * Resource DELETE route controller. takes $id (valid UUID) for the parent resource as first value to match
   * takes 'rx' and/or 'checksum' as optional query arguments
   * @see https://wiki.duraspace.org/display/FEDORA40/RESTful+HTTP+API#RESTfulHTTPAPI-RedDELETEDeletearesource
   */
  public function delete(Application $app, Request $request, $id, $child) {
    $tx = $request->query->get('tx', "");
    $force = $request->query->get('force', FALSE);
      
    error_log('---START OF DELETE RESOURCE REQUEST: ');
    error_log($id);
    $delete_queue = array($app->escape($id).'/'.$child, );
    $sparql_query = $app['twig']->render('findAllOreProxy.sparql', array(
       'resource' => $id,
    ));
    try {
      $sparql_result = $app['triplestore']->query($sparql_query);
    }
    catch (\Exception $e) {
      $app->abort(503, 'Chullo says "Triple Store Not available"');
    }
    if (count($sparql_result) > 0) {
      foreach ($sparql_result as $ore_proxy) {
        $delete_queue[] = $ore_proxy->obj->getUri();
      }
    }
    error_log(implode(',', $delete_queue));
    foreach($delete_queue as $object_uri) {
      try {
        $response = $app['api']->deleteResource($object_uri, $tx);
        // Check to ensure we've removed the object.
        // @TODO what is a 410 response?
        if (204 != $response->getStatusCode() || 410 != $response->getStatusCode()) {
          $app->abort($response->getStatusCode(), 'Failed to delete object at ' . $object_uri);
        }
        // Remove fcr:tombstone if 'force' == true.
        if ($force) {
          $response = $app['api']->deleteResource($object_uri.'/fcr:tombstone', $tx);
        }
      }
      catch (\Exception $e) {
        $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
      }
      error_log($response->getStatusCode());
    }
  }
  
}