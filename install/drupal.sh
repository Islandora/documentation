echo "Installing Drupal."

HOME_DIR=$1

cd $HOME_DIR

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
sed -i 's|DocumentRoot /var/www/html|DocumentRoot /var/www/html/drupal|' /etc/apache2/sites-enabled/000-default.conf

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

# Make the modules directory
mkdir -p sites/all/modules
cd sites/all/modules

# Islandora dependencies
drush dl services
drush -y en rest_server

# Islandora module
cp -r "$HOME_DIR/islandora/drupal/islandora" .
drush -y en islandora
