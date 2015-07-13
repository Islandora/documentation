echo "Installing Islandora Services"

HOME_DIR=$1

cd "$HOME_DIR"/islandora/camel/services

cd collection-service
sudo -u vagrant mvn install
/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/services.script
