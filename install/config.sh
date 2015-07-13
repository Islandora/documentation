echo "Deploying Karaf Configuration"

HOME_DIR=$1

cp "$HOME_DIR"/islandora/camel/config/islandora.cfg /opt/karaf/etc
