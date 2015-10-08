#!/bin/bash
echo "Installing ActiveMQ"

#sleep 15

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

`$KARAF_CLIENT -f $KARAF_CONFIGS/activemq.script`
