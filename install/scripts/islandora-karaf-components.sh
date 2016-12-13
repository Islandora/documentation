#!/bin/bash
echo "Installing Islandora Karaf Components"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

echo "Bootstrapping ID Mapping Database"
mysql -u "$MYSQL_USER" -p"$MYSQL_PASS" -e 'CREATE DATABASE idiomatic;'
mysql -u "$MYSQL_USER" -p"$MYSQL_PASS" -e 'CREATE TABLE idiomatic.uris ( public VARCHAR(1024), fedora VARCHAR(1024) NOT NULL, PRIMARY KEY (public)  );'

echo "Installing mysql-connector"
cd "$HOME_DIR"
wget http://downloads.mysql.com/archives/get/file/mysql-connector-java-"$MYSQL_CONNECTOR_VERSION".tar.gz
tar zxvf mysql-connector-java-"$MYSQL_CONNECTOR_VERSION".tar.gz
cp mysql-connector-java-"$MYSQL_CONNECTOR_VERSION"/mysql-connector-java-"$MYSQL_CONNECTOR_VERSION"-bin.jar "$KARAF_DIR"/deploy

echo "Installing ID Mapping Service"
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_id_mapping_service.script

echo "Installing Alpaca"
$KARAF_CLIENT -f $KARAF_CONFIGS/alpaca.script
