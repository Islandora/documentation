#!/bin/bash
echo "Installing Composer"

apt-get install -y php5.6-mbstring
curl -sS https://getcomposer.org/installer | php
php composer.phar install --no-progress
mv composer.phar /usr/local/bin/composer
