# Running Automated Tests

See the [Drupal Documentation on PHPUnit in Drupal](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal).

# Setting up PhpUnit in ISLE

Before you can use phpunit, you must first install the following: 

`composer require --dev -W phpspec/prophecy-phpunit drupal/core-dev`

After that, you need to make the database port available to PHPUnit. To do that, edit `docker-compose.yml` and find the section including `image: islandora/mariadb[version number]`. Shortly below is a `labels:` heading; set the value of the `traefik.enable: ` to `"true"`.

Apply the changes made to the `docker_compose.yml` using `docker compose up -d`.

## Running PHPUnit in Isle

Follow the `Configure PHPUnit` and `Create a directory for HTML output` sections in [Drupal Documentation on running phpunit tests](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal/running-phpunit-tests) to make a phpunit.xml.

phpunit tag's 'bootstrap' attribute default value should be changed if it is placed in any directory other than `codebase/web/core`.

In ISLE, the value of `SIMPLETEST_DB` variable should look like `mysql://username:db_password@islandora.traefik.me:3306/db_name`.

you can find your db_password in `codebase/web/sites/default/settings.php`.

the default username and db_name is `drupal_default`.

if your current directory is the same as the phpunit.xml use the following command to run phpunit:

`vendor/bin/phpunit web/modules/contrib/islandora/tests/src/Functional/DeleteNodeWithMediaAndFile.php`

_Directories are relative, this assumes you're in the `codebase` directory._

or if your phpunit.xml is in a different directory, then use the -c flag to specify the path to the directory containing phpunit.xml:

`vendor/bin/phpunit -c web/core web/modules/contrib/islandora/tests/src/Functional/DeleteNodeWithMediaAndFile.php`

## Setting up PHPUnit in PHPStorm

* [Drupal Documentation on running phpunit tests](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal/running-phpunit-tests-within-phpstorm)


