echo "Installing Sync."

HOME_DIR=$1

cd "$HOME_DIR/islandora/camel/sync"

sudo -u vagrant mvn install
/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/sync.script
