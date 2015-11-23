#!/bin/bash
echo "Installing Cxf"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

"$KARAF_CLIENT" -u karaf -h localhost -a 8101 -f "$KARAF_CONFIGS/cxf.script"
