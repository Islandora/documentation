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

cd /opt/islandora/services
php -r "readfile('https://getcomposer.org/installer');" > composer-setup.php
php -r "if (hash('SHA384', file_get_contents('composer-setup.php')) === '41e71d86b40f28e771d4bb662b997f79625196afcca95a5abf44391188c695c6c1456e16154c75a211d238cc3bc5cb47') { echo 'Installer verified'; } else { echo 'Installer corrupt'; unlink('composer-setup.php'); } echo PHP_EOL;"
php composer-setup.php
php -r "unlink('composer-setup.php');"
php composer.phar update
