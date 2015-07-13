#!/bin/bash
mysql -u root -e 'create database drupal;'
mysql -u root -e "GRANT ALL PRIVILEGES ON drupal.* To 'drupal'@'localhost' IDENTIFIED BY 'drupal';"

# Java (Oracle)
sudo apt-get install -y software-properties-common
sudo apt-get install -y python-software-properties
sudo add-apt-repository -y ppa:webupd8team/java
sudo apt-get update
echo debconf shared/accepted-oracle-license-v1-1 select true | sudo debconf-set-selections
echo debconf shared/accepted-oracle-license-v1-1 seen true | sudo debconf-set-selections
sudo apt-get install -y oracle-java8-installer
sudo update-java-alternatives -s java-8-oracle
sudo apt-get install -y oracle-java8-set-default
export JAVA_HOME=/usr/lib/jvm/java-8-oracle

cd $HOME
pear channel-discover pear.drush.org
pear upgrade --force Console_Getopt
pear upgrade --force pear
pear channel-discover pear.drush.org

wget http://alpha.library.yorku.ca/drush-6.3.tar.gz
tar xf drush-6.3.tar.gz
sudo mv drush-6.3 /opt/
sudo ln -s /opt/drush-6.3/drush /usr/bin/drush

wget http://alpha.library.yorku.ca/PHP_CodeSniffer-1.5.6.tgz
pear install PHP_CodeSniffer-1.5.6.tgz

wget http://alpha.library.yorku.ca/phpcpd.phar
sudo mv phpcpd.phar /usr/local/bin/phpcpd
sudo chmod +x /usr/local/bin/phpcpd

phpenv rehash
drush dl --yes drupal
cd drupal-*
drush si minimal --db-url=mysql://drupal:drupal@localhost/drupal --yes
drush runserver --php-cgi=$HOME/.phpenv/shims/php-cgi localhost:8081 &>/tmp/drush_webserver.log &
ln -s $ISLANDORA_DIR sites/all/modules/islandora
drush dl --yes coder-7.x-2.4
drush dl --yes potx-7.x-1.0
drush en --yes coder_review
drush en --yes simpletest
drush en --yes potx
drush dl --user=1 services
drush en --user=1 --yes rest_server
drush en --user=1 --yes islandora
drush en --user=1 --yes islandora_apachesolr
drush en --user=1 --yes islandora_basic_image
drush en --user=1 --yes islandora_collection
drush en --user=1 --yes islandora_dc
drush en --user=1 --yes islandora_mods
drush en --user=1 --yes islandora_rdf_mapping_service
drush cc all
# The shebang in this file is a bogeyman that is haunting the web test cases.
rm /home/travis/.phpenv/rbenv.d/exec/hhvm-switcher.bash
sleep 20
