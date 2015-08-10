#!/bin/bash
echo "Installing Camel"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

"$KARAF_CLIENT" < "$KARAF_CONFIGS/camel.script"
