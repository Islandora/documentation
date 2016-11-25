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
drush dl drupal-"$DRUPAL_VERSION" --drupal-project-rename=drupal

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

# Cycle apache
service apache2 restart

#Enable Core modules
drush en -y rdf
drush en -y responsive_image
drush en -y syslog
drush en -y serialization
drush en -y basic_auth
drush en -y rest

# Islandora dependencies

# RDF UI
drush dl rdfui --dev
drush en -y rdfui
drush en -y rdf_builder

# REST UI
drush dl restui
drush en -y restui

# Inline entity form
drush dl inline_entity_form
drush en -y inline_entity_form

# Media entity ecosystem
drush dl media_entity
drush en -y media_entity

drush dl media_entity_image
drush en -y media_entity_image

# Devel
drush dl devel
drush -y en devel

# Web Profiler
drush dl webprofiler
drush en -y webprofiler

# Apache Solr
## https://www.drupal.org/node/2613470
drush dl search_api
drush -y pm-uninstall search
drush en -y search_api

# Copy new schema files and restart Tomcat
#cp -a "$DRUPAL_HOME"/sites/all/modules/apachesolr/solr-conf/solr-4.x/. "$SOLR_HOME"/collection1/conf/
#service tomcat7 restart

cd "$DRUPAL_HOME/modules"
git clone https://github.com/DiegoPino/claw-jsonld.git
drush en -y jsonld

git clone https://github.com/Islandora-CLAW/islandora.git
drush en -y islandora

# Set default theme to bootstrap
drush -y dl bootstrap
drush -y en bootstrap
drush -y config-set system.theme default bootstrap

# Permissions
cd /var/www/html
chown -R www-data:www-data drupal
chmod -R g+w drupal
