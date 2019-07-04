# Checking Code Style

Before opening a pull request, you should check your code style. If you are running the Vagrant, you can run `phpcs` within the Drupal installation directory (on the Vagrant, that is `/var/www/html/drupal`) as follows:

* From within the Drupal root directory, `./vendor/bin/phpcs --standard=/var/www/html/drupal/vendor/drupal/coder/coder_sniffer/Drupal yourfile`, where `yourfile` is the path to the PHP file you want to check.
* From within the Drupal root directory, you can use a relative path to the standard file, e.g. `./vendor/bin/phpcs --standard=/vendor/drupal/coder/coder_sniffer/Drupal yourfile`, where `yourfile` is the path to the PHP file you want to check.
* From within Drupal's `web` directory, you can use a relative path to the standard file, e.g. `../vendor/bin/phpcs --standard=/vendor/drupal/coder/coder_sniffer/Drupal yourfile`, where `yourfile` is the path to the PHP file you want to check.

In all cases, if you specify a path to a directory instead of a single file, all files in that directory will be checked.

