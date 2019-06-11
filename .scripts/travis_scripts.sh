#!/bin/bash

# Common checks to get run during the 'script' section in Travis.
OUTPUT=0

# Make OUTPUT equal return code if return code is not 0
function checkReturn {
  if [ $1 -ne 0 ]; then
    OUTPUT=$1
  fi
}

$SCRIPT_DIR/line_endings.sh $TRAVIS_BUILD_DIR
checkReturn $?

phpcs --standard=Drupal --ignore=*.md --extensions=php,module,inc,install,test,profile,theme,css,info $TRAVIS_BUILD_DIR
checkReturn $?

phpcpd --names *.module,*.inc,*.test,*.php $TRAVIS_BUILD_DIR
checkReturn $?

exit $OUTPUT
