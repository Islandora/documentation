#!/bin/sh
# Alpaca
echo "Building Alpaca"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cd "$HOME_DIR"/islandora/Alpaca
sudo -u vagrant ./gradlew build
