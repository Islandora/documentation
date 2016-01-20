<?php

namespace Islandora\ResourceService\Provider;

use Silex\Application;
use Silex\ServiceProviderInterface;
use Silex\ControllerProviderInterface;
use Islandora\Chullo\FedoraApi;
use Islandora\Chullo\TriplestoreClient;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Symfony\Component\Yaml\Yaml;
use Islandora\ResourceService\Controller\ResourceController;

class ResourceServiceProvider implements ServiceProviderInterface, ControllerProviderInterface
{
  /**
   * Part of ServiceProviderInterface
   */
  function register(Application $app) {
    //
    // Define controller services
    //
    $app['islandora.resourcecontroller'] = $app->share(function() use ($app) {
      return new \Islandora\ResourceService\Controller\ResourceController($app);
    });
    if (!isset($app['twig'])) {
      $app['twig'] = $app->share($app->extend('twig', function($twig, $app) {
        return $twig;
      }));
    }
    if (!isset($app['api'])) {
      $app['api'] =  $app->share(function() use ($app) {
        return FedoraApi::create($app['config']['islandora']['fedoraProtocol'].'://'.$app['config']['islandora']['fedoraHost'].$app['config']['islandora']['fedoraPath']);
      });
    }  
    if (!isset($app['triplestore'])) {
      $app['triplestore'] = $app->share(function() use ($app) {
        return TriplestoreClient::create($app['config']['islandora']['tripleProtocol'].'://'.$app['config']['islandora']['tripleHost'].$app['config']['islandora']['triplePath']);
      });
    }
    /**
    * Ultra simplistic YAML settings loader.
    */
    if (!isset($app['config'])) {
      $app['config'] = $app->share(function() use ($app){
        if ($app['debug']) {
          $configFile = __DIR__.'/../../config/settings.dev.yml';
        }
        else {
          $configFile = __DIR__.'/../../config/settings.yml';
        }
        $settings = Yaml::parse(file_get_contents($configFile));
        return $settings;
      });
    }
   /** 
   * Make our middleware callback functions protected
   */
   /**
    * before middleware to handle browser requests.
    */
   $app['islandora.htmlHeaderToTurtle'] = $app->protect(function(Request $request) {
     // In case the request was made by a browser, avoid 
     // returning the whole Fedora4 API Rest interface page.
     if (0 === strpos($request->headers->get('Accept'),'text/html')) {
       $request->headers->set('Accept', 'text/turtle', TRUE);
     }
   });


   /**
    * before middleware to normalize host header to same as fedora's running
    * instance. 
    */
   $app['islandora.hostHeaderNormalize'] = $app->protect(function(Request $request) use ($app) {
     // Normalize Host header to Repo's real location
     $request->headers->set('Host', $app['config']['islandora']['fedoraHost'], TRUE);
   });

   /**
    * Converts request $id (uuid) into a fedora4 resourcePath
    */
   $app['islandora.idToUri'] = $app->protect(function ($id) use ($app) {
     // Run only if $id given /can also be refering root resource,
     // we accept only UUID V4 or empty
     if (NULL != $id) {
       $sparql_query = $app['twig']->render('getResourceByUUIDfromTS.sparql', array(
         'uuid' => $id,
       ));
       error_log($sparql_query);
        try {
          $sparql_result = $app['triplestore']->query($sparql_query);
        }
        catch (\Exception $e) {
          $app->abort(503, 'Chullo says "Triple Store Not available"');
        }
       // We only assign one in case of multiple ones
       // Will have to check for edge cases?
       foreach ($sparql_result as $triple) {
         return $triple->s->getUri();
       }
       // Abort the routes if we don't get a subject from the tripple.
       $app->abort(404, sprintf('Failed getting resource Path for "%s" from triple store', $id));
     } 
     else {
       // If $id is empty then assume we are dealing with fedora base rest endpoint
       return $app['config']['islandora']['fedoraProtocol'].'://'.$app['config']['islandora']['fedoraHost'].$app['config']['islandora']['fedoraPath'];
     }
   });
  
  }

  function boot(Application $app) {
  }

  /**
   * Part of ControllerProviderInterface
   */
  public function connect(Application $app) {
    $controllers = $app['controllers_factory'];
    //
    // Define routing referring to controller services
    //
    $controllers->get("/resource/{id}/{child}", "islandora.resourcecontroller:get")
      ->convert('id', $app['islandora.idToUri'])
      ->assert('id',$app['config']['islandora']['resourceIdRegex'])
      ->before($app['islandora.hostHeaderNormalize']) 
      ->before($app['islandora.htmlHeaderToTurtle'])
      ->value('id',"")
      ->value('child',"")
      ->bind('islandora.resourceGet');
    $controllers->post("/resource/{id}", "islandora.resourcecontroller:post")
      ->convert('id', $app['islandora.idToUri'])
      ->assert('id',$app['config']['islandora']['resourceIdRegex'])
      ->before($app['islandora.hostHeaderNormalize']) 
      ->before($app['islandora.htmlHeaderToTurtle'])
      ->value('id',"")
      ->bind('islandora.resourcePost');
    $controllers->put("/resource/{id}/{child}", "islandora.resourcecontroller:put")
      ->convert('id', $app['islandora.idToUri'])
      ->assert('id',$app['config']['islandora']['resourceIdRegex'])
      ->before($app['islandora.hostHeaderNormalize']) 
      ->before($app['islandora.htmlHeaderToTurtle'])
      ->value('id',"")
      ->value('child',"")
      ->bind('islandora.resourcePut');
    $controllers->patch("/resource/{id}/{child}", "islandora.resourcecontroller:patch")
      ->convert('id', $app['islandora.idToUri'])
      ->assert('id',$app['config']['islandora']['resourceIdRegex'])
      ->before($app['islandora.hostHeaderNormalize']) 
      ->before($app['islandora.htmlHeaderToTurtle'])
      ->value('id',"")
      ->value('child',"")
      ->bind('islandora.resourcePatch');
    $controllers->delete("/resource/{id}/{child}", "islandora.resourcecontroller:delete")
      ->convert('id', $app['islandora.idToUri'])
      ->assert('id',$app['config']['islandora']['resourceIdRegex'])
      ->before($app['islandora.hostHeaderNormalize']) 
      ->before($app['islandora.htmlHeaderToTurtle'])
      ->value('id',"")
      ->value('child',"")
      ->bind('islandora.resourceDelete');
    return $controllers;
  }
}