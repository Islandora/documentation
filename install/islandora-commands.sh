echo "Installing Commands"

HOME_DIR=$1

cd "$HOME_DIR"/islandora/camel/commands

curl -sS https://getcomposer.org/installer | php
php composer.phar install
