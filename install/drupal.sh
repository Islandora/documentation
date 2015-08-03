echo "Installing Drupal."

HOME_DIR=$1

cd "$HOME_DIR"

# Drush and drupal deps
apt-get -y install php5-gd
apt-get -y install drush
a2enmod rewrite
service apache2 reload
cd /var/www/html

# Download Drupal
drush dl drupal --drupal-project-rename=drupal

# Permissions
chown -R www-data:www-data drupal
chmod -R g+w drupal

# Do the install
cd drupal
drush si -y --db-url=mysql://root:islandora@localhost/drupal7 --site-name=islandora-fedora4.org
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

# Cycle apache
service apache2 restart

# Make the modules and libraries directories
mkdir -p sites/all/modules
mkdir -p sites/all/libraries

cd sites/all/modules

# Islandora dependencies
drush dl httprl
drush -y en httprl
drush dl services
drush -y en services
drush dl field_permissions
drush -y en field_permissions
drush dl field_readonly
drush -y en field_readonly
drush dl views
drush -y en views
drush dl rdfx
drush -y en rdfx
drush dl entity
drush -y en entity
drush dl uuid
drush -y en uuid
drush dl xml_field
drush -y en xml_field
drush dl jquery_update
drush -y en jquery_update
git clone https://github.com/Islandora-Labs/xpath_field.git
drush -y en xpath_field
drush dl hook_post_action
drush -y en hook_post_action

# Devel!
drush dl devel
drush -y en devel

# Undocumented dependency for rdfx on ARC2 for RDF generation.
cd ../libraries
mkdir ARC2
cd ARC2
git clone https://github.com/semsol/arc2.git
mv arc2 arc
cd ../../modules

# Apache Solr
drush dl apachesolr
drush en -y apachesolr

# Copy new schema files and restart Tomcat
cp -f apachesolr/solr-conf/solr-4.x/* /var/lib/tomcat7/solr/collection1/conf/
service tomcat7 restart

# Islandora modules
ln -s "$HOME_DIR"/islandora/drupal islandora
drush -y en islandora
drush -y en islandora_dc
drush -y en islandora_mods
drush -y en islandora_basic_image
drush -y en islandora_collection
drush -y en islandora_apachesolr
drush -y en islandora_delete_by_fedora_uri_service
drush -y en islandora_medium_size_service
drush -y en islandora_tn_service

# Set default theme to bootstrap
cd ../themes
drush -y dl bootstrap
drush -y en bootstrap
drush vset theme_default bootstrap

# Coder & Code Sniffer
pear install PHP_CodeSniffer
cd /tmp
wget http://ftp.drupal.org/files/projects/coder-8.x-2.1.tar.gz
tar -xzvf coder-8.x-2.1.tar.gz
mv -v coder /usr/share
chown -hR vagrant:vagrant /usr/share/coder
ln -sv /usr/share/coder/coder_sniffer/Drupal /usr/share/php/PHP/CodeSniffer/Standards
