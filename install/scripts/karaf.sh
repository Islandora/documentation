#!/bin/bash
echo "Installing Karaf"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -f "$DOWNLOAD_DIR/apache-karaf-$KARAF_VERSION.tar.gz" ]; then
  echo "Downloading Karaf"
  wget -q -O "$DOWNLOAD_DIR/apache-karaf-$KARAF_VERSION.tar.gz" "http://mirror.csclub.uwaterloo.ca/apache/karaf/$KARAF_VERSION/apache-karaf-$KARAF_VERSION.tar.gz"
fi

cd "$HOME_DIR"

if [ ! -d "/opt/apache-karaf-$KARAF_VERSION" ]; then
    if [ ! -d "$DOWNLOAD_DIR/apache-karaf-$KARAF_VERSION" ]; then
        echo -n "Extracting Apache Karaf..."
        tar zxf "$DOWNLOAD_DIR/apache-karaf-$KARAF_VERSION.tar.gz"
        echo " done"
    fi
    if [ ! -d "/opt/apache-karaf-$KARAF_VERSION" ]; then
        echo "Deploying Apache Karaf... "
        cp -R "$DOWNLOAD_DIR/apache-karaf-$KARAF_VERSION" "$HOME_DIR"
        mv apache-karaf-"$KARAF_VERSION" /opt
        echo " done"
    fi
fi

if [ -L "/opt/karaf" ]; then
    rm /opt/karaf
fi

echo "Symlinking Apache Karaf... "
ln -s "/opt/apache-karaf-$KARAF_VERSION" /opt/karaf
echo " done"


if [ ! -L "/etc/init.d/karaf-service" ]; then
    echo "Installing Karaf as a service... "
    # Run a setup script to add some feature repos and prepare it for running as a service
    /opt/karaf/bin/start
    sleep 60
    "$KARAF_CLIENT" < "$KARAF_CONFIGS/karaf_service.script"
    /opt/karaf/bin/stop

    # Add it as a Linux service
    ln -s /opt/karaf/bin/karaf-service /etc/init.d/
    update-rc.d karaf-service defaults
    echo " done"
fi

# Add the vagrant user's maven repository
if ! grep -q "$HOME_DIR/.m2/repository" /opt/karaf/etc/org.ops4j.pax.url.mvn.cfg ; then
    echo "Adding vagrant user's Maven repository... "
    sed -i "s|#org.ops4j.pax.url.mvn.localRepository=|org.ops4j.pax.url.mvn.localRepository=$HOME_DIR/.m2/repository|" /opt/karaf/etc/org.ops4j.pax.url.mvn.cfg
    echo " done"
fi

# Start it
echo "Starting Karaf as a service... "
service karaf-service start
sleep 60
echo "done"
