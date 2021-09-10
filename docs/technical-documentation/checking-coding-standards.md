
# Checking Code Style

Before opening a pull request, you should check your code style using PHP_CodeSniffer (phpcs). We use the Drupal coding standards provided by [Drupal coder](https://www.drupal.org/project/coder). If you are using the [Ansible Playbook](https://github.com/Islandora-Devops/islandora-playbook), phpcs and Drupal Coder are installed and phpcs can be run as follows:

* from within Drupal's root directory: `./vendor/bin/phpcs --standard=./vendor/drupal/coder/coder_sniffer/Drupal --ignore=*.md --extensions=php,module,inc,install,test,profile,theme,css,info web/modules/contrib/my_module`
* from within Drupal's `web` directory: `../vendor/bin/phpcs --standard=../vendor/drupal/coder/coder_sniffer/Drupal --ignore=*.md --extensions=php,module,inc,install,test,profile,theme,css,info modules/contrib/my_module`

In both cases:

* `modules/contrib/my_module` is the relative or full path to the PHP file or directory you want to check.
* the path to the coding standard file can be relative to where you are running it from, e.g. when in `web`: `--standard=../vendor/drupal/coder/coder_sniffer/Drupal`
* the options `--ignore=*.md --extensions=php,module,inc,install,test,profile,theme,css,info` are what is used in our Travis CI environment (specified in the [Islandora CI](https://github.com/Islandora/islandora_ci/blob/main/travis_scripts.sh) shared module), which will cause your Pull Request to pass or fail its checks.

If using a non-Ansible method of deployment, you may need to install phpcs and coder yourself, and the paths may vary. 
