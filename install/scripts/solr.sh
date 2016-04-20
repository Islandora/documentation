#!/bin/bash

echo "Installing Solr"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -f "$DOWNLOAD_DIR/solr-$SOLR_VERSION.tgz" ]; then
  echo "Downloading Solr"
  wget -q -O "$DOWNLOAD_DIR/solr-$SOLR_VERSION.tgz" "http://archive.apache.org/dist/lucene/solr/$SOLR_VERSION/solr-$SOLR_VERSION.tgz"
fi

# Prepare SOLR_HOME
if [ ! -d "$SOLR_HOME" ]; then
  mkdir "$SOLR_HOME"
fi

cp -v "$DOWNLOAD_DIR/solr-$SOLR_VERSION.tgz" /tmp
cd /tmp
tar -xzf solr-"$SOLR_VERSION".tgz
cp "solr-$SOLR_VERSION/dist/solr-$SOLR_VERSION.war" /var/lib/tomcat7/webapps/solr.war
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/solr.war

if [ ! -f "$DOWNLOAD_DIR/commons-logging-1.1.2.jar" ]; then
  echo "Downloading commons-logging-1.1.2.jar"
  wget -q -O "$DOWNLOAD_DIR/commons-logging-1.1.2.jar" "http://repo1.maven.org/maven2/commons-logging/commons-logging/1.1.2/commons-logging-1.1.2.jar"
fi
cp "$DOWNLOAD_DIR/commons-logging-1.1.2.jar" cd /usr/share/tomcat7/lib
cp /tmp/solr-"$SOLR_VERSION"/example/lib/ext/slf4j* /usr/share/tomcat7/lib
cp /tmp/solr-"$SOLR_VERSION"/example/lib/ext/log4j* /usr/share/tomcat7/lib

chown -hR tomcat7:tomcat7 /usr/share/tomcat7/lib

cp -R /tmp/solr-"$SOLR_VERSION"/example/solr/* "$SOLR_HOME"

chown -hR tomcat7:tomcat7 "$SOLR_HOME"

touch /var/lib/tomcat7/velocity.log
chown tomcat7:tomcat7 /var/lib/tomcat7/velocity.log

service tomcat7 restart
