#!/bin/bash
echo "Deploying Karaf Configuration"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -f "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg" ]; then
  # Wait a minute for Karaf to finish starting up
  echo  "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg doesn't exist, waiting a minute"
  sleep 60
fi

if [ -f "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg" ]; then
  # Update fcrepo triplestore indexing config
  sed -i 's|triplestore.baseUrl=http://localhost:8080/fuseki/test/update|triplestore.baseUrl=http://localhost:8080/bigdata/sparql|' "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg"
else
  echo "$KARAF_DIR/etc/org.fcrepo.camel.indexing.triplestore.cfg still doesn't exist, this is an ERROR!"
fi

if [ -f "$KARAF_DIR/etc/org.ops4j.datasource-idiomatic.cfg" ]; then
  # Update fcrepo triplestore indexing config
  sed -i 's|user=|user=root|' "$KARAF_DIR/etc/org.ops4j.datasource-idiomatic.cfg"
  sed -i 's|pass=|user=islandora|' "$KARAF_DIR/etc/org.ops4j.datasource-idiomatic.cfg"
else
  echo "$KARAF_DIR/etc/org.ops4j.datasource-idiomatic.cfg still doesn't exist, this is an ERROR!"
fi

if [ -f "$KARAF_DIR/etc/edu.amherst.acdc.connector.broadcast.cfg" ]; then
  # Update fcrepo triplestore indexing config
  sed -i 's|message.recipients=|message.recipients=activemq:queue:acrepo-connector-idiomatic,activemq:queue:islandora/indexing/triplestore|' "$KARAF_DIR/etc/edu.amherst.acdc.connector.broadcast.cfg"
else
  echo "$KARAF_DIR/etc/edu.amherst.acdc.connector.broadcast.cfg still doesn't exist, this is an ERROR!"
fi

if [ -f "$KARAF_DIR/etc/edu.amherst.acdc.connector.idiomatic.cfg" ]; then
  # Update fcrepo triplestore indexing config
  sed -i 's|input.stream=broker:topic:fedora|input.stream=activemq:queue:acrepo-connector-idiomatic|' "$KARAF_DIR/etc/edu.amherst.acdc.connector.idiomatic.cfg"
  sed -i 's|id.property=dc:identifier|id.property=schema:url|' "$KARAF_DIR/etc/edu.amherst.acdc.connector.idiomatic.cfg"
  sed -i 's|id.namespace=http://purl.org/dc/elements/1.1/|id.namespace=http://schema.org/|' "$KARAF_DIR/etc/edu.amherst.acdc.connector.idiomatic.cfg"
else
  echo "$KARAF_DIR/etc/edu.amherst.acdc.connector.idiomatic.cfg still doesn't exist, this is an ERROR!"
fi
