echo "Installing Islandora Component"

HOME_DIR=$1

cd "$HOME_DIR"/islandora/camel/component

sudo -u vagrant mvn install
/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/component.script
