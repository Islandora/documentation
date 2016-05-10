#!/bin/sh
echo "Installing Composer and Islandora Microservices"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

ln -s "$HOME_DIR/islandora" "/opt/islandora"

cp "$HOME_DIR/islandora/install/configs/001-microservices.conf" "/etc/apache2/sites-enabled/"

if [ $(grep -c 'Listen 8282' /etc/apache2/ports.conf) != 1 ]; then
  echo "Adding Listen 8282 to Apache ports.conf"
  sed -i '/Listen 80$/a \Listen 8282' /etc/apache2/ports.conf
fi

/etc/init.d/apache2 restart

cp "$HOME_DIR/islandora/install/scripts/align_branches.sh" "/opt/islandora/services"
cd /opt/islandora/services
./align_branches.sh
cp "/opt/islandora/services/config/example.settings.yml" "/opt/services/config/settings.dev.yml"

cp "$HOME_DIR/islandora/install/composer-setup.php" "/opt/islandora/services"
cp "$HOME_DIR/islandora/install/composer.sha384sum" "/opt/islandora/services"
cd "/opt/islandora/services"

sha384sum -c composer.sha384sum
if [ "$?" != "0" ]; then
  echo "Composer-setup.php did not match the expected SHA-384 hash, did you update the version of composer and not the stored hash?"
  exit
fi

php composer-setup.php
php -r "unlink('composer-setup.php');"
php composer.phar update

