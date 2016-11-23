#!/bin/bash
echo "Installing Islandora Karaf Components"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

echo "Bootstrapping ID Mapping Database"
mysql -u root -pislandora -e 'CREATE DATABASE idiomatic;'
mysql -u root -pislandora -e 'CREATE TABLE idiomatic.uris ( public VARCHAR(1024), fedora VARCHAR(1024) NOT NULL, PRIMARY KEY (public)  );'

echo "Installing mysql-connector"
cd "$HOME_DIR"
wget http://downloads.mysql.com/archives/get/file/mysql-connector-java-6.0.4.tar.gz
tar zxvf mysql-connector-java-6.0.4.tar.gz
cp mysql-connector-java-6.0.4/mysql-connector-java-6.0.4-bin.jar "$KARAF_DIR"/deploy

echo "Installing ID Mapping Service"
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_id_mapping_service.script

echo "Installing Islandora Triplestore Indexer"
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_indexing_triplestore.script
