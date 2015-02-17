###
# BASICS
###
cd ~

# Update
apt-get -y update && apt-get -y upgrade

# SSH
apt-get -y install openssh-server

# Build tools
apt-get -y install build-essential

# Git
apt-get -y install git

# Java
apt-get -y install openjdk-7-jdk

# Maven
apt-get -y install maven

# Tomcat
apt-get -y install tomcat7

# Set some params so it's non-interactive for the lamp-server install
debconf-set-selections <<< 'mysql-server mysql-server/root_password password islandora'
debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password islandora'
debconf-set-selections <<< "postfix postfix/mailname string islandora-fedora4.org"
debconf-set-selections <<< "postfix postfix/main_mailer_type string 'Internet Site'"

# Lamp server
tasksel install lamp-server

# Get the repo
git clone -b 7.x-2.x https://github.com/Islandora-Labs/islandora.git

###
# FEDORA
###
FEDORA_VERSION=4.1.0

cd ~

mkdir /var/lib/tomcat7/fcrepo4-data
chown tomcat7:tomcat7 /var/lib/tomcat7/fcrepo4-data
chmod g-w /var/lib/tomcat7/fcrepo4-data

wget -O fcrepo.war "https://github.com/fcrepo4/fcrepo4/releases/download/fcrepo-$FEDORA_VERSION/fcrepo-webapp-$FEDORA_VERSION.war"
mv fcrepo.war /var/lib/tomcat7/webapps

###
# DRUPAL
###
cd ~

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
cp -r ~/islandora/drupal/islandora .
drush -y en islandora
