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
    public function register(Application $app)
    {
        //
        // Define controller services
        //
        //This is the base path for the application. Used to change the location
        //of yaml config files when registerd somewhere else
        if (!isset($app['islandora.BasePath'])) {
            $app['islandora.BasePath'] = __DIR__.'/..';
        }
    
        // If nobody registered a UuidGenerator first?
        if (!isset($app['UuidGenerator'])) {
            $app['UuidGenerator'] = $app->share(
                $app->share(
                    function () use ($app) {
                        return new UuidGenerator();
                    }
                )
            );
        }
        $app['islandora.collectioncontroller'] = $app->share(
            function () use ($app) {
                return new \Islandora\CollectionService\Controller\CollectionController($app, $app['UuidGenerator']);
            }
        );
        if (!isset($app['twig'])) {
            $app['twig'] = $app->share(
                $app->extend(
                    'twig',
                    function (
                        $twig,
                        $app
                    ) {
                        return $twig;
                    }
                )
            );
        }
        if (!isset($app['api'])) {
            $app['api'] =  $app->share(
                function () use ($app) {
                    return FedoraApi::create(
                        $app['config']['islandora']['fedoraProtocol']
                        .'://'.$app['config']['islandora']['fedoraHost']
                        .$app['config']['islandora']['fedoraPath']
                    );
                }
            );
        }
        if (!isset($app['triplestore'])) {
            $app['triplestore'] = $app->share(
                function () use ($app) {
                    return TriplestoreClient::create(
                        $app['config']['islandora']['tripleProtocol']
                        .'://'.$app['config']['islandora']['tripleHost']
                        .$app['config']['islandora']['triplePath']
                    );
                }
            );
        }
        /**
    * Ultra simplistic YAML settings loader.
    */
        if (!isset($app['config'])) {
            $app['config'] = $app->share(
                function () use ($app) {
                    {
                    if ($app['debug']) {
                        $configFile = $app['islandora.BasePath'].'/../config/settings.dev.yml';
                    } else {
                        $configFile = $app['islandora.BasePath'].'/../config/settings.yml';
                    }
                    }
                    $settings = Yaml::parse(file_get_contents($configFile));
                    return $settings;
                }
            );
        }
    }

    public function boot(Application $app)
    {
    }

    /**
   * Part of ControllerProviderInterface
   */
    public function connect(Application $app)
    {
        $CollectionControllers = $app['controllers_factory'];
        //
        // Define routing referring to controller services
        //

        $CollectionControllers->post("/collection/{id}", "islandora.collectioncontroller:create")
            ->value('id', "")
            ->bind('islandora.collectionCreate');
        $CollectionControllers->post("/collection/{id}/member/{member}", "islandora.collectioncontroller:addMember")
            ->bind('islandora.collectionAddMember');
        $CollectionControllers->delete(
            "/collection/{id}/member/{member}",
            "islandora.collectioncontroller:removeMember"
        )
            ->bind('islandora.collectionRemoveMember');
        return $CollectionControllers;
    }
}
