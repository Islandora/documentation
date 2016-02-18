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
  public function delete(Application $app, Request $request, $id, $child) {
    $tx = $request->query->get('tx', "");
    $force = $request->query->get('force', FALSE);
    try {
      $response = $app['api']->deleteResource($app->escape($id).'/'.$child, $tx);
      //remove tombstone also if 'force' == true and previous response is 204
      if ((204 == $response->getStatusCode() || 410 == $response->getStatusCode()) && $force) {
        $response= $app['api']->deleteResource($app->escape($id).'/'.$child.'/fcr:tombstone', $tx);
      }
    }
    catch (\Exception $e) {
      $app->abort(503, '"Chullo says Fedora4 Repository is Not available"');
    }
    return $response;
  }
  
}