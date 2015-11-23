#!/bin/bash
echo "Installing Islandora Karaf Components"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

echo "Installing Islandora Sync Gateway"
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_sync_gateway.script
sleep 10
echo "Installing Islandora Basic Image Service"
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_basic_image.script
sleep 10
echo "Installing Islandora Collection Service"
$KARAF_CLIENT -f $KARAF_CONFIGS/islandora_collection.script
