echo "Installing ActiveMQ"

HOME_DIR=$1

/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/activemq.script
