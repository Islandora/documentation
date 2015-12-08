#!/bin/bash
echo "Deploying Karaf Configuration"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cp "$HOME_DIR"/islandora/camel/sync/src/main/cfg/ca.islandora.sync.cfg /opt/karaf/etc

if [ ! -f "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg" ]; then
  # Wait a minute for Karaf to finish starting up
  echo  "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg doesn't exist, waiting a minute"
  sleep 60
fi

if [ -f "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg" ]; then
  # Update fcrepo triplestore indexing config
  sed -i 's|triplestore.baseUrl=localhost:8080/fuseki/test/update|triplestore.baseUrl=localhost:8080/bigdata/sparql|' "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg"
else
  echo "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg still doesn't exist, this is an ERROR!"
fi
