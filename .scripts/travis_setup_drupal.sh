#!/bin/bash
echo "Setup database for Drupal"
mysql -u root -e 'create database drupal;'
mysql -u root -e "GRANT ALL PRIVILEGES ON drupal.* To 'drupal'@'127.0.0.1' IDENTIFIED BY 'drupal';"

echo "Install utilities needed for testing"
mkdir /opt/utils
cd /opt/utils
if [ -z "$COMPOSER_PATH" ]; then
  composer require drupal/coder
  composer require sebastian/phpcpd
else
  php -dmemory_limit=-1 $COMPOSER_PATH require drupal/coder
  php -dmemory_limit=-1 $COMPOSER_PATH require sebastian/phpcpd
fi
sudo ln -s /opt/utils/vendor/bin/phpcs /usr/bin/phpcs
sudo ln -s /opt/utils/vendor/bin/phpcpd /usr/bin/phpcpd
phpenv rehash
phpcs --config-set installed_paths /opt/utils/vendor/drupal/coder/coder_sniffer

echo "Composer install drupal site"
cd /opt
git clone https://github.com/Islandora-CLAW/drupal-project.git drupal
cd drupal
if [ -z "$COMPOSER_PATH" ]; then
  composer install
else
  php -dmemory_limit=-1 $COMPOSER_PATH install
fi

echo "Setup Drush"
sudo ln -s /opt/drupal/vendor/bin/drush /usr/bin/drush
phpenv rehash

echo "Drush setup drupal site"
cd web
drush si --db-url=mysql://drupal:drupal@127.0.0.1/drupal --yes
drush runserver 127.0.0.1:8282 &
until curl -s 127.0.0.1:8282; do true; done > /dev/null
echo "Enable simpletest module"
drush --uri=127.0.0.1:8282 en -y simpletest

# Set default theme to carapace (and download dependencies, will composer-ize later)
cd /opt/drupal
if [ -z "$COMPOSER_PATH" ]; then
  composer require "drupal/adaptivetheme:^2.0" "drupal/at_tools:^2.0" "drupal/layout_plugin:^1.0@alpha"
else
  php -dmemory_limit=-1 $COMPOSER_PATH require "drupal/adaptivetheme:^2.0" "drupal/at_tools:^2.0" "drupal/layout_plugin:^1.0@alpha"
fi
cd web
drush en -y at_tools
drush en -y layout_plugin
mkdir /opt/drupal/web/themes/custom
git clone https://github.com/Islandora-CLAW/carapace /opt/drupal/web/themes/custom/carapace
drush then -y carapace
drush -y config-set system.theme default carapace

mkdir libraries
cd libraries
wget "https://github.com/mozilla/pdf.js/releases/download/v2.0.943/pdfjs-2.0.943-dist.zip"
mkdir pdf.js
unzip pdfjs-2.0.943-dist.zip -d pdf.js
rm pdfjs-2.0.943-dist.zip

# Get the pdf module
if [ -z "$COMPOSER_PATH" ]; then
  composer require "drupal/pdf:1.x-dev"
else
  php -dmemory_limit=-1 $COMPOSER_PATH require "drupal/pdf:1.x-dev"
fi
drush -y en pdf

echo "Setup ActiveMQ"
cd /opt
wget "http://archive.apache.org/dist/activemq/5.14.3/apache-activemq-5.14.3-bin.tar.gz"
tar -xzf apache-activemq-5.14.3-bin.tar.gz
apache-activemq-5.14.3/bin/activemq start
