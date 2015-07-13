echo "Installing Camel"

HOME_DIR=$1

/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/camel.script
