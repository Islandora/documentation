#!/bin/bash
echo "Installing Commands"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cd "$HOME_DIR"/islandora/camel/commands

curl -sS https://getcomposer.org/installer | php
php composer.phar install
