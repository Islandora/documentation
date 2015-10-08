#!/bin/bash
echo "Installing Fcrepo-Camel-Toolbox"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

"$KARAF_CLIENT" -f "$KARAF_CONFIGS/fcrepo_camel_toolbox.script"
