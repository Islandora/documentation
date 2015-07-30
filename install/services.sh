echo "Installing Islandora Services"

HOME_DIR=$1

# Chown everything over to the vagrant user just in case
chown -R vagrant:vagrant $HOME_DIR/.m2

cd "$HOME_DIR"/islandora/camel/services

cd collection-service
sudo -u vagrant mvn install

cd "$HOME_DIR"/islandora/camel/services

cd basic-image-service
sudo -u vagrant mvn install

/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/services.script
