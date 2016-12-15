#!/bin/sh
# Alpaca
echo "Building Alpaca"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cd "$HOME_DIR"
git clone -b broadcaster-448 https://github.com/dannylamb/Alpaca.git
cd Alpaca
chown -R ubuntu:ubuntu "$HOME_DIR/Alpaca"
sudo -u ubuntu ./gradlew clean build install

# Chown everything over to the ubuntu user just in case
chown -R ubuntu:ubuntu "$HOME_DIR/Alpaca"
