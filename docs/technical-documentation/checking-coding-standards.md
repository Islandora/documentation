# Checking Code Style

Before opening a pull request, you should check your code style. If you are using the [Vagrant](https://github.com/Islandora-Devops/claw-playbook), you can run `phpcs` within the Drupal installation directory (on the Vagrant, that is `/var/www/html/drupal`) or from within the `web` directory (`/var/www/html/drupal/web`) as follows:

* from within Drupal's root directory: `./vendor/bin/phpcs --standard=./vendor/drupal/coder/coder_sniffer/Drupal modules/contrib/my_module`, where `modules/contrib/my_module` is the relative or full path to the PHP file you want to check.
* from within Drupal's `web` directory: `../vendor/bin/phpcs --standard=../vendor/drupal/coder/coder_sniffer/Drupal yourfile`, where `yourfile` is the relative or full path to the PHP file you want to check.

In both cases:

* the path to the coding standard file can be relative to where you are running it from, e.g. when in `web`: `--standard=../vendor/drupal/coder/coder_sniffer/Drupal`
* you can specify a single file to check, or a directory path; in the latter case, all files in that directory will be checked.

Islandora runs `phpcs` in its Github continuous integration environment, and there, it specifies which files to ignore and which files to check. It is a good idea for developers to specify the same options when running `phpcs` locally, prior to opening a pull request. For example (running `phpcs` from the within Drupal's `web` directory), you should use the following `--ignore` and `--extensions` options:

`../vendor/bin/phpcs --standard=../vendor/drupal/coder/coder_sniffer/Drupal --ignore=*.md --extensions=php,module,inc,install,test,profile,theme,css,info modules/contrib/my_module`
