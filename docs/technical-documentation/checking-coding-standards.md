# Checking Coding Standards

Islandora Drupal modules follow the [Drupal communities coding standards](https://www.drupal.org/docs/develop/standards/coding-standards). You can check your code against these standards using [PHP_Codesniffer](https://www.drupal.org/docs/8/modules/code-review-module/installing-coder-sniffer).

If you are running the Vagrant, you can run `phpcs` within the Drupal installation directory (on the Vagrant, that is `/var/www/html/drupal`) as follows:

`./vendor/bin/phpcs --standard=/var/www/html/drupal/vendor/drupal/coder/coder_sniffer/Drupal yourfile`, where `yourfile` is the path to the PHP file you want to check. If you specify a path to a directory instead of a single file, all files in that directory will be checked.


