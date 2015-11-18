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
sed -i 's#fuseki/test/update#bigdata/sparql#g' "$DOWNLOAD_DIR"/fcrepo-camel-toolbox-fcrepo-camel-toolbox-4.4.0/fcrepo-indexing-triplestore/src/main/cfg/org.fcrepo.camel.indexing.triplestore.cfg "$DOWNLOAD_DIR"/fcrepo-camel-toolbox-fcrepo-camel-toolbox-4.4.0/fcrepo-audit-triplestore/src/main/cfg/org.fcrepo.camel.audit.cfg
cd fcrepo-camel-toolbox-fcrepo-camel-toolbox-"$FCREPO_CAMEL_VERSION"
MAVEN_OPTS="-Xmx1024m" sudo -u vagrant mvn install

"$KARAF_CLIENT" < "$KARAF_CONFIGS/fcrepo-camel-toolbox.script"

sed -i "s#fuseki/test/update#bigdata/sparql#g' /opt/karaf/etc/org.fcrepo.camel.indexing.triplestore.cfg
