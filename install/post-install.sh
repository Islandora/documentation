echo "RUNNING POST-INSTALL COMMANDS"

# Chown and chmod tomcat directory
chown -R tomcat7:tomcat7 /var/lib/tomcat7
chown -R tomcat7:tomcat7 /var/log/tomcat7
chmod -R g+w /var/lib/tomcat7

# Chown and chmod apache directory
chown -R www-data:www-data /var/www/html
chmod -R g+w /var/www/html

# Chown islandora repo
chown -R vagrant:vagrant /home/vagrant/islandora

# Disable security for node access/management so POC works
drush -r /var/www/html/drupal scr /home/vagrant/islandora/install/disable_node_access.php
