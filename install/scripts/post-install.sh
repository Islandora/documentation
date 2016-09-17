#!/bin/bash
echo "RUNNING POST-INSTALL COMMANDS"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

# Chown and chmod tomcat directory
chown -R tomcat7:tomcat7 /var/lib/tomcat7
chown -R tomcat7:tomcat7 /var/log/tomcat7
chmod -R g+w /var/lib/tomcat7

# Chown and chmod apache directory
chown -R www-data:www-data /var/www/html
chmod -R g+w /var/www/html

# Chown the home directory for good measure
chown -R vagrant:vagrant "$HOME_DIR"

# Fix FITS log
sed -i 's|log4j.appender.FILE.File=${catalina.home}/logs/fits-service.log|log4j.appender.FILE.File=/var/log/tomcat7/fits-service.log|g' /var/lib/tomcat7/webapps/fits/WEB-INF/classes/log4j.properties

# Cycle tomcat
cd /var/lib/tomcat7
service tomcat7 restart

# Cycle karaf and watch the maven bundles
service karaf-service restart
sleep 60
