# Updating Drupal

## Introduction

This section describes how to update Drupal and its modules using Composer. If you installed Islandora using the Islandora Playbook, then your Drupal was installed by Composer, so it is considered best practice to continue using Composer for updates. The method on this section is not specific to Islandora, and does not (yet) include how to update Islandora Features.

### What is Composer
It is recommended by Drupal.org and the Islandora community to use Composer with Drupal 8 for various tasks.

"[Composer](https://getcomposer.org/) is a [dependency manager](https://en.wikipedia.org/wiki/Package_manager) for PHP. Drupal core uses Composer to manage core dependencies like Symfony components and Guzzle." [[Source](https://www.drupal.org/docs/develop/using-composer/using-composer-with-drupal)]

## Always create backs ups (DB and files) before updating

**Before updating either Drupal core or Drupal modules:**
* Back up both your files and database. Having a complete backup makes it easy to revert to the prior version if the update fails.
* Optionally, if you made manual modifications to files like .htaccess, composer.json, or robots.txt, copy them somewhere easy to find. Because after you've installed the new Drupal core, you will need to re-apply the changes. For example, Acquia Dev Desktop places a .htaccess file in the top-level directory and without it, only the homepage on your site will work.

**Warning:** Always revert to a backup if you get a fatal error in the update process.

## Updating Drupal Core
Over time new versions of Drupal “core” are released, and Islandora users are encouraged to install official Drupal core updates and security patches. On the other hand “alpha” and “beta" versions of Drupal core should only be installed by advanced users for testing purposes.

The Islandora community STRONGLY recommends that the "Composer" method of upgrading Drupal core be used with Islandora as mentioned [here](https://www.drupal.org/docs/8/update/update-core-via-composer).

### Here is an overview of the steps for updating Drupal core using Composer

!!! note "Back Up" 
    First make sure you have made database and file back ups.

1) First, verify that an update of Drupal core actually is available:

`composer outdated "drupal/*"`

If there is no line starting with drupal/core, Composer isn't aware of any update. If there is an update, continue with the commands below.


2) Assuming you are used to updating Drupal and know all the precautions that you should take, the update is as simple as:

`composer update drupal/core webflo/drupal-core-require-dev "symfony/*" --with-dependencies`

If you want to know all packages that will be updated by the update command, use the --dry-run option first.

!!! note "Alternate syntax needed"
    Islandora is configured to use a fork of drupal-composer/drupal-project which requires a specific composer syntax used above compared to other Drupal 8 sites. In addition, if you are upgrading from 8.5 to 8.7, you need to replace "~8.5.x" with "^8.7.0" for drupal/core and webflo/drupal-core-require-dev in composer.json. [[Source](https://www.drupal.org/docs/8/update/update-core-via-composer#s-one-step-update-instruction)]

3) Apply any required database updates using ``drush updatedb``, or use the web admin user interface.  

`drush updatedb`

4) Clear the cache using drush ``cache:rebuild``, or use the web admin user interface.

`drush cache:rebuild`

For stepwise update instructions visit this page:
https://www.drupal.org/docs/8/update/update-core-via-composer#s-stepwise-update-instructions

## Updating Drupal Modules

Islandora uses several general Drupal modules and some specialized Islandora Drupal modules, and over time new versions of these modules are released. There are two approaches to updating Drupal modules in Islandora: using Composer or updating modules individually. Islandora uses Composer to determine which Drupal module versions should be installed for each release of Islandora. Therefore if you update the Islandora specific Drupal modules using Composer you will also update any dependent general Drupal modules as well. The second method is to individually update Drupal modules.

For more information about how to update Drupal modules visit:

https://www.drupal.org/docs/8/extending-drupal-8/updating-modules

!!! note "Back Up" 
    First make sure you have made database and file back ups.

