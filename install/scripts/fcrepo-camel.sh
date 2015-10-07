#!/bin/bash
echo "Installing fcrepo-camel"

sleep 15

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

"$KARAF_CLIENT" < "$KARAF_CONFIGS/fcrepo_camel.script"
