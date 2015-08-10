#!/bin/bash
echo "Installing Islandora Services"

HOME_DIR=$1
if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

# Chown everything over to the vagrant user just in case
chown -R vagrant:vagrant "$HOME_DIR/.m2"

cd "$HOME_DIR"/islandora/camel/services

cd collection-service
sudo -u vagrant mvn install

cd "$HOME_DIR"/islandora/camel/services

cd basic-image-service
sudo -u vagrant mvn install

"$KARAF_CLIENT" < "$KARAF_CONFIGS/services.script"
