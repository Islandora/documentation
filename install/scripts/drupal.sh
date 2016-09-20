#!/bin/bash

echo "Installing Drupal."

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cd "$HOME_DIR"

# Drush and drupal deps
apt-get -y -qq install php7.0-gd php7.0-xml php7.0-mysql php7.0-curl php7.0-json php-stomp
cd /tmp && wget http://files.drush.org/drush.phar
chmod +x drush.phar
mv drush.phar /usr/local/bin/drush
a2enmod rewrite
service apache2 reload
cd /var/www/html

# Download Drupal
drush dl drupal-8 --drupal-project-rename=drupal

# Permissions
chown -R www-data:www-data drupal
chmod -R g+w drupal

# Do the install
cd "$DRUPAL_HOME"
drush si -y --db-url=mysql://root:islandora@localhost/drupal8 --site-name=Islandora-CLAW
drush user-password admin --password=islandora

# Set document root
sed -i 's|DocumentRoot /var/www/html$|DocumentRoot /var/www/html/drupal|' /etc/apache2/sites-enabled/000-default.conf

# Set override for drupal directory
# TODO Don't do this in main apache conf
sed -i '$i<Directory /var/www/html/drupal>' /etc/apache2/apache2.conf
sed -i '$i\\tOptions Indexes FollowSymLinks' /etc/apache2/apache2.conf
sed -i '$i\\tAllowOverride All' /etc/apache2/apache2.conf
sed -i '$i\\tRequire all granted' /etc/apache2/apache2.conf
sed -i '$i</Directory>' /etc/apache2/apache2.conf

# Torch the default index.html
rm /var/www/html/index.html

## Trusted Host Settings
cat >> "$DRUPAL_HOME"/sites/default/settings.php <<EOF
\$settings['trusted_host_patterns'] = array(
'^localhost$',
);
EOF

## The always_populate_raw_post_data PHP setting should be set to -1 in PHP version 5.6
sed -i 's|#;always_populate_raw_post_data = -1|always_populate_raw_post_data = -1|g' /etc/php/5.6/apache2/php.ini
sed -i 's|#;always_populate_raw_post_data = -1|always_populate_raw_post_data = -1|g' /etc/php/5.6/cli/php.ini

# Cycle apache
service apache2 restart

#Enable Core modules
drush en -y responsive_image
drush en -y syslog
drush en -y serialization
drush en -y basic_auth
drush en -y rest

# Islandora dependencies
## HAS NOT BEEN PORTED TO DRUPAL 8
#drush dl httprl

## Drupal 8 Alpha
drush dl services
drush -y en services

## Drupal 8 Alpha
drush dl libraries
drush -y en libraries

## HAS NOT BEEN PORTED TO DRUPAL 8
#drush dl field_permissions

## HAS NOT BEEN PORTED TO DRUPAL 8
#drush dl field_readonly

## INCLUDED IN DRUPAL CORE
#drush dl views

## HAS NOT BEEN PORTED TO DRUPAL 8
#drush dl rdfx

## Drupal 8 Alpha
drush dl entity
drush -y en entity

## HAS NOT BEEN PORTED TO DRUPAL 8
#drush dl uuid

## HAS NOT BEEN PORTED TO DRUPAL 8
#drush dl xml_field

## INCLUDED IN DRUPAL CORE
#drush dl jquery_update

#git clone https://github.com/Islandora-Labs/xpath_field.git

## Drupal 8 Beta
drush dl hook_post_action
drush -y en hook_post_action

# Devel
## Drupal 8 Alpha
drush dl devel
drush -y en devel

# Undocumented dependency for rdfx on ARC2 for RDF generation, and spyc.
#cd "$DRUPAL_HOME/sites/all/libraries"
#git clone https://github.com/mustangostang/spyc.git
#mkdir ARC2
#cd ARC2
#git clone https://github.com/semsol/arc2.git
#mv arc2 arc
#cd "$DRUPAL_HOME/sites/all/modules"

# Apache Solr
## https://www.drupal.org/node/2613470
#drush dl apachesolr
#drush en -y apachesolr
drush dl search_api
drush -y pm-uninstall search
drush en -y search_api

# Copy new schema files and restart Tomcat
#cp -a "$DRUPAL_HOME"/sites/all/modules/apachesolr/solr-conf/solr-4.x/. "$SOLR_HOME"/collection1/conf/
#service tomcat7 restart

#cd "$DRUPAL_HOME/sites/all/modules"
#git clone https://github.com/Islandora-CLAW/islandora.git
#drush -y en islandora
#drush -y en islandora_dc
#drush -y en islandora_mods
#drush -y en islandora_basic_image
#drush -y en islandora_collection
#drush -y en islandora_apachesolr
#drush -y en islandora_delete_by_fedora_uri_service
#drush -y en islandora_medium_size_service
#drush -y en islandora_tn_service

# Set default theme to bootstrap
drush -y dl bootstrap
drush -y en bootstrap
drush -y config-set system.theme default bootstrap

# Permissions
cd /var/www/html
chown -R www-data:www-data drupal
chmod -R g+w drupal
