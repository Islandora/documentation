<?php

namespace Islandora\TransactionService\Provider;

use Silex\Application;
use Silex\ServiceProviderInterface;
use Silex\ControllerProviderInterface;
use Islandora\Chullo\FedoraApi;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\HttpKernelInterface;
use Symfony\Component\Yaml\Yaml;
use Islandora\ResourceService\Controller\ResourceController;
use Islandora\TransactionService\Controller\TransactionController;

class TransactionServiceProvider implements ServiceProviderInterface, ControllerProviderInterface {

  /**
   * Part of ServiceProviderInterface
   */
  public function register(Application $app) {
    //
    // Define controller services
    //
    $app['islandora.transactioncontroller'] = $app->share(function() use ($app) {
      return new \Islandora\TransactionService\Controller\TransactionController($app);
    });
    
    if (!isset($app['islandora.resourcecontroller'])) {
      $app['islandora.resourcecontroller'] = $app->share(function() use ($app) {
        return new \Islandora\ResourceService\Controller\ResourceController($app);
      });
    }

    if (!isset($app['api'])) {
      $app['api'] =  $app->share(function() use ($app) {
        return FedoraApi::create($app['config']['islandora']['fedoraProtocol'].'://'.$app['config']['islandora']['fedoraHost'].$app['config']['islandora']['fedoraPath']);
      });
    }

    // Common simple YAML config parser.
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

  }

  public function boot(Application $app) {}

  public function connect(Application $app) {
    $controllers = $app['controllers_factory'];

    $controllers->get("/transaction/{id}", "islandora.resourcecontroller:get")
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
    ->bind('islandora.transactionGet');

    $controllers->post("/transaction", "islandora.transactioncontroller:create")
    ->bind('islandora.transactionCreate');

    $controllers->post("/transaction/{id}/extend", "islandora.transactioncontroller:extend")
    ->value('id',"")
    ->bind('islandora.transactionExtend');

    $controllers->post("/transaction/{id}/commit", "islandora.transactioncontroller:commit")
    ->value('id',"")
    ->bind('islandora.transactionCommit');

    $controllers->post("/transaction/{id}/rollback", "islandora.transactioncontroller:rollback")
    ->value('id',"")
    ->bind('islandora.transactionRollback');

    return $controllers;
  }

}