#/bin/bash

echo "Installing Drupal Console."

HOME_DIR=$1                    
  
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cd /tmp
#curl https://drupalconsole.com/installer -L -o drupal.phar
#mv drupal.phar /usr/local/bin/drupal
#chmod +x /usr/local/bin/drupal
composer global require drupal/console:@stable
echo "PATH=\$PATH:$HOME/.config/composer/vendor/bin" >> $HOME/.bash_profile
cd $HOME
~/.config/composer/vendor/bin/drupal init
if [ ! -d ".console" ]; then
  mkdir .console
fi
if [ -f ".console/config.yml" ]; then
  rm .console/config.yml
fi
cp $HOME_DIR/islandora/install/configs/config.yml $HOME_DIR/.console/config.yml
chown -hR vagrant:vagrant /home/vagrant/.console
sed -i -e "\$asource \"$HOME/.console/console.rc\" 2>/dev/null" $HOME/.bashrc

# Fix drupal-console
cd /var/www/html/drupal
~/.config/composer/vendor/bin/drupal settings:set checked "true"
