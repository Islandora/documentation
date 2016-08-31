#/bin/bash

echo "Installing Drupal Console."

HOME_DIR=$1                    
  
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

sed -i '$idate.timezone=America/Toronto' /etc/php/7.0/cli/php.ini
cd /tmp
curl https://drupalconsole.com/installer -L -o drupal.phar
mv drupal.phar /usr/local/bin/drupal
chmod +x /usr/local/bin/drupal
