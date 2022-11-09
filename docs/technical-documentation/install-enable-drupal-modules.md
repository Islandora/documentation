## Downloading and Enabling Drupal Modules and Themes

Islandora can make use of the majority of Drupal [modules](https://www.drupal.org/project/project_module) and [themes](https://www.drupal.org/project/project_theme). Common use cases have been documented in the [Islandora Cookbook](/documentation/user-documentation/extending/). There are several ways to download and install Drupal modules. Please refer to [this guide](https://www.drupal.org/docs/extending-drupal) on Drupal.org.

[Composer](https://www.drupal.org/docs/develop/using-composer/using-composer-to-install-drupal-and-manage-dependencies) is the recommended method to install and update drupal modules and themes in Islandora.
```shell
$ composer require "<vendor>/<package>:<version>"

# Example
$ composer require "islandora/jsonld:^2"
```

In the [Islandora playbook](https://github.com/Islandora-Devops/islandora-playbook), you can add a Drupal module's or theme's machine name to the `drupal_composer_dependencies` variable [here](https://github.com/Islandora-Devops/islandora-playbook/blob/dev/inventory/vagrant/group_vars/webserver/drupal.yml).
To enable the Drupal module or theme, add the module machine name to the `drupal_enable_modules` variable as well.

![alt text](../assets/install-enable-drupal-modules_drupal_composer_dependencies.png?raw=true "drupal_composer_dependencies Screenshot")

For modules that require additional steps, additional tasks may need to be added to the Ansible playbook. Re-provisioning your instance via Ansible will install the module.

## Video Walkthroughs: Modules & Installing modules with Composer

Click the image below to open the **introduction to Modules** video tutorial on the Islandora Youtube channel. 

[![Drupal 101: Modules](https://img.youtube.com/vi/mvX3cnNeOns/0.jpg)](https://www.youtube.com/watch?v=mvX3cnNeOns)

Click the image below to open the **Installing modules with Composer** video tutorial on the Islandora Youtube channel. 

[![Drupal 101: Installing modules with Composer](https://img.youtube.com/vi/otl-pPPGdR8/0.jpg)](https://www.youtube.com/watch?v=otl-pPPGdR8)

See more videos from the Drupal 101 series [here.](https://www.youtube.com/watch?v=meRNdBxaiTE&list=PL4seFC7ELUtogpsYoN8WZLLOjJVRZFGTZ)
