#!/bin/bash
echo "Installing Fcrepo-Camel-Toolbox"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -f "$DOWNLOAD_DIR/fcrepo-camel-toolbox.war" ]; then
  echo "Downloading fcrepo-camel-toolbox"
  wget -O "$DOWNLOAD_DIR/fcrepo-camel-toolbox.war" "https://github.com/fcrepo4-labs/fcrepo-camel-toolbox/releases/download/fcrepo-camel-toolbox-$FCREPO_CAMEL_VERSION/fcrepo-camel-webapp-at-is-it-$FCREPO_CAMEL_VERSION.war"
fi

cd /var/lib/tomcat7/webapps
cp -v "$DOWNLOAD_DIR/fcrepo-camel-toolbox.war" "/var/lib/tomcat7/webapps"
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fcrepo-camel-toolbox.war

if [ $(grep -c '\-Dtriplestore.baseUrl=' /etc/default/tomcat7) -eq 0 ]; then
  echo "JAVA_OPTS=\"\$JAVA_OPTS -Dtriplestore.baseUrl=localhost:8080/bigdata/sparql\"" >> /etc/default/tomcat7
fi

service tomcat7 restart
