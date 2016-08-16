#!/bin/bash
echo "Installing Islandora Karaf Components"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

echo "Installing Islandora Sync Gateway"
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_sync_gateway.script
sleep 10
echo "Installing Islandora Triplestore Indexer"
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_indexing_triplestore.script
