#!/bin/sh
# Alpaca
echo "Building Alpaca"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

cd "$HOME_DIR"
git clone https://github.com/Islandora-CLAW/Alpaca.git
cd Alpaca
chown -R ubuntu:ubuntu "$HOME_DIR/Alpaca"
sudo -u ubuntu ./gradlew build

# Chown everything over to the ubuntu user just in case
chown -R ubuntu:ubuntu "$HOME_DIR/Alpaca"
