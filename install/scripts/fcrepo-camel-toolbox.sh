#!/bin/bash
echo "Installing Fcrepo-Camel-Toolbox"

HOME_DIR=$1

cd $HOME_DIR

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -f "$DOWNLOAD_DIR/fcrepo-camel-toolbox.tar.gz" ]; then
  echo "Downloading fcrepo-camel-toolbox"
  wget -O "$DOWNLOAD_DIR/fcrepo-camel-toolbox.tar.gz" https://github.com/fcrepo4-exts/fcrepo-camel-toolbox/archive/fcrepo-camel-toolbox-"$FCREPO_CAMEL_VERSION".tar.gz
fi

cd "$DOWNLOAD_DIR"
tar -xzvf fcrepo-camel-toolbox.tar.gz
cd fcrepo-camel-toolbox-fcrepo-camel-toolbox-"$FCREPO_CAMEL_VERSION"
MAVEN_OPTS="-Xmx1024m" sudo -u vagrant mvn install

"$KARAF_CLIENT" < "$KARAF_CONFIGS/fcrepo-camel-toolbox.script"

if [ $(grep -c '\-Dtriplestore.baseUrl=' /etc/default/tomcat7) -eq 0 ]; then
  echo "JAVA_OPTS=\"\$JAVA_OPTS -Dtriplestore.baseUrl=localhost:8080/bigdata/sparql\"" >> /etc/default/tomcat7
fi

service tomcat7 restart
