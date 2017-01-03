#!/bin/bash

echo "Installing Drupal."

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cd "$HOME_DIR"

# Drush and drupal deps
apt-get -y -qq install php7.0-gd php7.0-xml php7.0-mysql php7.0-curl php7.0-json
a2enmod rewrite
service apache2 reload
cd /var/www/html

# Download Drupal
git clone https://github.com/Islandora-CLAW/drupal-project drupal
cd "$DRUPAL_HOME"
composer install

# Setup drush and drupal console aliases
touch "$HOME_DIR/.bash_aliases"
echo "alias drush=\"$DRUSH_CMD\"" >> "$HOME_DIR/.bash_aliases"
echo "alias drupal=\"$DRUPAL_CMD\"" >> "$HOME_DIR/.bash_aliases"

# Do the install
cd "$DRUPAL_HOME/web"
$DRUSH_CMD si -y --db-url=mysql://root:islandora@localhost/drupal8 --site-name=Islandora-CLAW
$DRUSH_CMD user-password admin --password=islandora

# Set document root
sed -i 's|DocumentRoot /var/www/html$|DocumentRoot /var/www/html/drupal/web|' /etc/apache2/sites-enabled/000-default.conf

# Set override for drupal directory
# TODO Don't do this in main apache conf
sed -i '$i<Directory /var/www/html/drupal/web>' /etc/apache2/apache2.conf
sed -i '$i\\tOptions Indexes FollowSymLinks' /etc/apache2/apache2.conf
sed -i '$i\\tAllowOverride All' /etc/apache2/apache2.conf
sed -i '$i\\tRequire all granted' /etc/apache2/apache2.conf
sed -i '$i</Directory>' /etc/apache2/apache2.conf

# Torch the default index.html
rm /var/www/html/index.html

## Trusted Host Settings
cat >> "$DRUPAL_HOME"/web/sites/default/settings.php <<EOF
\$settings['trusted_host_patterns'] = array(
'^localhost$',
);
EOF

# Cycle apache
service apache2 restart

#Enable Core modules
$DRUSH_CMD en -y rdf
$DRUSH_CMD en -y responsive_image
$DRUSH_CMD en -y syslog
$DRUSH_CMD en -y serialization
$DRUSH_CMD en -y basic_auth
$DRUSH_CMD en -y rest

# Islandora dependencies

# REST UI
$DRUSH_CMD en -y restui

# Media entity ecosystem
$DRUSH_CMD en -y media_entity

$DRUSH_CMD en -y media_entity_image

# Devel
$DRUSH_CMD -y en devel

# Web Profiler
$DRUSH_CMD en -y webprofiler

# Apache Solr
## https://www.drupal.org/node/2613470
$DRUSH_CMD -y pm-uninstall search
$DRUSH_CMD en -y search_api

$DRUSH_CMD en -y islandora
$DRUSH_CMD en -y islandora_collection

# Set default theme to bootstrap
$DRUSH_CMD -y en bootstrap
$DRUSH_CMD -y config-set system.theme default bootstrap

# Permissions
chown -R www-data:www-data "$DRUPAL_HOME"
chmod -R g+w "$DRUPAL_HOME"
