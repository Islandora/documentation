if [ $TRAVIS_PHP_VERSION = "5.6" ]; then
  phpenv config-add $SCRIPT_DIR/php56.ini
fi
