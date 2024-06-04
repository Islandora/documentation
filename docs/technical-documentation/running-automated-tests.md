# Running Automated Tests

See the [Drupal Documentation on PHPUnit in Drupal](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal).

## Setting up PhpUnit

Before you can use phpunit, you must first install the following: 

`composer require --dev -W phpspec/prophecy-phpunit drupal/core-dev`

In ISLE, you need to make the database port available to PHPUnit. To do that, edit `docker-compose.yml` and find the section including `image: islandora/mariadb[version number]`. Shortly below is a `labels:` heading; set the value of the `traefik.enable: ` to `"true"`. Apply the changes made to the `docker_compose.yml` using `docker compose up -d`.

Follow the `Configure PHPUnit` and `Create a directory for HTML output` sections in [Drupal Documentation on running phpunit tests](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal/running-phpunit-tests) to make a `phpunit.xml` file. Note that:

* If you place the `phpunit.xml` file in any directory other than `[drupal root]/web/core`, you need to change the 'bootstrap' in the `<phpunit>` tag near the top of the file to point to the relative or absolute location of the `[drupal root]/web/core` folder.

*  When setting the `SIMPLETEST_DB` database credentials in ISLE, 
    * the default username and db_name are `drupal_default`
    * your db_password can be found in `codebase/web/sites/default/settings.php`

* Unless you changed the default values, just swap out [password] for your actual db password in the following:

```
mysql://drupal_default:[password]@islandora.traefik.me:3306/drupal_default`.

```

## Running PHPUnit

If you are in the Drupal root directory (`codebase` on ISLE; the one containing `web`) and your `phpunit.xml` file is also in the Drupal root directory, use the following command to run phpunit for a single test file (here, Islandora's DeleteNodeWithMediaAndFile.php):

`vendor/bin/phpunit web/modules/contrib/islandora/tests/src/Functional/DeleteNodeWithMediaAndFile.php`

If your phpunit.xml is in a different directory, such as web/core, then use the -c flag to specify the path to the directory containing phpunit.xml:

`vendor/bin/phpunit -c web/core web/modules/contrib/islandora/tests/src/Functional/DeleteNodeWithMediaAndFile.php`

## Setting up PHPUnit in PHPStorm

* [Drupal Documentation on running phpunit tests](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal/running-phpunit-tests-within-phpstorm)


