<?php

namespace Islandora\ResourceService\Provider;

use Silex\Application;
use Silex\ServiceProviderInterface;
use Ramsey\Uuid\Uuid;

class UuidServiceProvider implements ServiceProviderInterface
{
    /**
  * ServiceProviderInterface
  */
    public function register(Application $app)
    {
        //
        // Define controller services
        //
    
        //made shared, only need to make one instance of the base uuid
        $app['islandora.baseuuid5'] = $app->share(
            function () use ($app) {
                if (!isset($app['UuidServiceProvider.default_namespace'])) {
                    throw new \RuntimeException('A Default Namespace is needed to generate UUID V5');
                }
                $uuid5 = Uuid::uuid5(Uuid::NAMESPACE_DNS, $app['UuidServiceProvider.default_namespace']);
                return $uuid5;
            }
        );
    
        $app['islandora.uuid5'] = $app->protect(
            function ($word, $namespace = null) use ($app) {
                if (!isset($namespace)) {
                    $uuid5 = Uuid::uuid5($app['islandora.baseuuid5']->toString(), $word);
                } else {
                    $baseuuid5 = Uuid::uuid5(Uuid::NAMESPACE_DNS, $namespace);
                    $uuid5 = Uuid::uuid5($baseuuid5->toString(), $word);
                }
                return $uuid5->toString();
            }
        );
    
        $app['islandora.uuid4'] = $app->protect(
            function () use ($app) {
                $uuid4 = Uuid::uuid4();
                return $uuid4->toString();
            }
        );
    }

    public function boot(Application $app)
    {
    }
}
