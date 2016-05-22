#!/bin/sh
echo "Installing Composer and Islandora Microservices"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

mkdir /opt/microservices
cd /opt/microservices
git clone https://github.com/Islandora-CLAW/Crayfish.git
git clone https://github.com/Islandora-CLAW/pdx.git

cp "$HOME_DIR/islandora/install/configs/001-microservices.conf" "/etc/apache2/sites-enabled/"

if [ $(grep -c 'Listen 8282' /etc/apache2/ports.conf) != 1 ]; then
  echo "Adding Listen 8282 to Apache ports.conf"
  sed -i '/Listen 80$/a \Listen 8282' /etc/apache2/ports.conf
fi

/etc/init.d/apache2 restart

cp "/opt/microservices/crayfish/config/example.settings.yml" "/opt/microservices/crayfish/config/settings.dev.yml"
cd "/opt/microservices/crayfish"
composer update
cp "/opt/microservices/pdx/config/example.settings.yml" "/opt/microservices/pdx/config/settings.dev.yml"
cd "/opt/microservices/pdx"
composer update
