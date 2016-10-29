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

cp -v "$DOWNLOAD_DIR/solr-$SOLR_VERSION.tgz" /tmp
cd /tmp
tar xzf solr-"$SOLR_VERSION".tgz solr-"$SOLR_VERSION"/bin/install_solr_service.sh --strip-components=2
bash ./install_solr_service.sh solr-"$SOLR_VERSION".tgz
cd /opt/solr
sudo -u solr bin/solr create_core -c CLAW
