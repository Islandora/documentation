<?php

namespace Islandora\CollectionService\Provider;

use Silex\Application;
use Silex\ServiceProviderInterface;
use Silex\ControllerProviderInterface;
use Islandora\Chullo\FedoraApi;
use Islandora\Chullo\TriplestoreClient;
use Islandora\Chullo\Uuid\UuidGenerator;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Symfony\Component\Yaml\Yaml;
use Islandora\CollectionService\Controller\CollectionController;

class CollectionServiceProvider implements ServiceProviderInterface, ControllerProviderInterface
{
  /**
   * Part of ServiceProviderInterface
   */
  function register(Application $app) {
    //
    // Define controller services
    //
    //This is the base path for the application. Used to change the location
    //of yaml config files when registerd somewhere else
    $app['islandora.BasePath'] = __DIR__.'/..';
    
    // If nobody registered a UuidGenerator first?
    if (!isset($app['UuidGenerator'])) {
      $app['UuidGenerator'] = $app->share($app->share(function() use ($app) {
        return new UuidGenerator();
      }));
    }
    $app['islandora.collectioncontroller'] = $app->share(function() use ($app) {
      return new \Islandora\CollectionService\Controller\CollectionController($app, $app['UuidGenerator']);
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
         {
          if ($app['debug']) {
            $configFile = $app['islandora.BasePath'].'/../config/settings.dev.yml';
          }
          else {
            $configFile = $app['islandora.BasePath'].'/../config/settings.yml';
          }
        }    
        $settings = Yaml::parse(file_get_contents($configFile));
        return $settings;
      });
    }
  }

  function boot(Application $app) {
  }

  /**
   * Part of ControllerProviderInterface
   */
  public function connect(Application $app) {
    $CollectionControllers = $app['controllers_factory'];
    //
    // Define routing referring to controller services
    //

    $CollectionControllers->get("/collection/{id}", "islandora.resourcecontroller:get")
      ->convert('id', $app['islandora.idToUri'])
      ->value('id',"")
      ->value('child', "")
      ->before(function(Request $request) { 
        if (isset($request->attributes->parameters) && $request->attributes->parameters->has('id')) {
          // To get this to work we need to GET /islandora/resource//tx:id
          // So we move the $id to the $child parameter.
          $id = $request->attributes->parameters->get('id');
          $request->attributes->parameters->set('child', $id);
          $request->attributes->parameters->set('id', '');
        }
      })
      ->bind('islandora.collectionGet');

    $CollectionControllers->post("/collection/{id}", "islandora.collectioncontroller:create")
      ->value('id',"")
      ->bind('islandora.collectionCreate');

    $CollectionControllers->post("/collection/{id}/member/{member}", "islandora.collectioncontroller:addMember")
      ->bind('islandora.collectionAddMember');

    $CollectionControllers->delete("/collection/{id}/member/{member}", "islandora.collectioncontroller:removeMember")
      ->bind('islandora.collectionRemoveMember');

    return $CollectionControllers;
  }
}