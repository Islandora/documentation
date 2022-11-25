# Running Automated Tests

The simpletest module (formerly the subject of this page) has been deprecated.

To run tests, see [Drupal Documentation on PHPUnit in Drupal](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal).

# Setting up PhpUnit in ISLE


Before you can use phpunit, you must first install the following modules.

`composer require --dev phpspec/prophecy-phpunit drupal/core-dev`

after that you need to make the port of database available to PHPUnit, to do that find the section `image: islandora/mariadb` in the `docker-composer.yml` file and set the value of the label `traefik.enable` to true.

## Running PHPUnit in Isle

follow the `Configure PHPUnit` and `Create a directory for HTML output` sections in [Drupal Documentation on running phpunit tests](https://www.drupal.org/docs/automated-testing/phpunit-in-drupal/running-phpunit-tests) to make a phpunit.xml.

you can find your db password in `codebase/web/sites/default/settings.php`.

user_name and db_name is `drupal_default`

if your current directory is the same as the phpunit.xml use the following command to run phpunit:

`vendor/bin/phpunit codebase/web/modules/contrib/islandora/tests/src/Functional/DeleteNodeWithMediaAndFile.php`

or if your phpunit.xml is in a different directory, then use the command below:

`vendor/bin/phpunit -c web/core web/modules/contrib/islandora/tests/src/Functional/DeleteNodeWithMediaAndFile.php`

_-c specified the path of phpunit.xml from the current directory_ 

## Setting up phpunit in PHPStorm

to setup phpunit with phpstorm, use the following article

[Drupal Documentation on running phpunit tests](https://www.drupal.org/docs/automa`ted-testing/phpunit-in-drupal/running-phpunit-tests-within-phpstorm)


