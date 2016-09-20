#!/bin/bash
echo "Installing fcrepo-camel-toolbox"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

echo "Installing fcrepo-indexing-triplestore"
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
$KARAF_CLIENT -f $KARAF_CONFIGS/fcrepo_camel_toolbox.script
