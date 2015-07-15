echo "RUNNING POST-INSTALL COMMANDS"

HOME_DIR=$1

# Chown and chmod tomcat directory
chown -R tomcat7:tomcat7 /var/lib/tomcat7
chown -R tomcat7:tomcat7 /var/log/tomcat7
chmod -R g+w /var/lib/tomcat7

# Chown and chmod apache directory
chown -R www-data:www-data /var/www/html
chmod -R g+w /var/www/html

# Chown the home directory for good measure
chown -R vagrant:vagrant "$HOME_DIR"

# Cycle tomcat
service tomcat7 restart

# Cycle karaf and watch the maven bundles
service karaf-service restart
sleep 60
/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/watch.script

# Fix ApacheSolr config
drush -r /var/www/html/drupal sqlq "update apachesolr_environment set url='http://localhost:8080/solr' where url='http://localhost:8983/solr'"
drush -r /var/www/html/drupal cc all
