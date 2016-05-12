#!/bin/bash
echo "Installing Karaf"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -f "$DOWNLOAD_DIR/apache-karaf-${KARAF_VERSION}.tar.gz" ]; then
  echo "Downloading Karaf version $KARAF_VERSION"
  wget -q -O "$DOWNLOAD_DIR/apache-karaf-${KARAF_VERSION}.tar.gz" "http://archive.apache.org/dist/karaf/$KARAF_VERSION/apache-karaf-${KARAF_VERSION}.tar.gz"
fi

cd "$HOME_DIR"

if [ ! -d "/opt/apache-karaf-$KARAF_VERSION" ]; then
    if [ ! -d "$DOWNLOAD_DIR/apache-karaf-$KARAF_VERSION" ]; then
        cd "$DOWNLOAD_DIR"
        echo -n "Extracting Apache Karaf..."
        tar xf "$DOWNLOAD_DIR/apache-karaf-${KARAF_VERSION}.tar.gz"
        echo " done"
    fi
    if [ ! -d "/opt/apache-karaf-$KARAF_VERSION" ]; then
        echo "Deploying Apache Karaf... "
        cp -R "$DOWNLOAD_DIR/apache-karaf-$KARAF_VERSION" "$HOME_DIR"
        mv apache-karaf-"$KARAF_VERSION" /opt
        echo " done"
    fi
fi

if [ -L "$KARAF_DIR" ]; then
    rm "$KARAF_DIR"
fi

echo "Symlinking Apache Karaf... "
ln -s "/opt/apache-karaf-$KARAF_VERSION" $KARAF_DIR
echo " done"


if [ ! -L "/etc/init.d/karaf-service" ]; then
    echo "Installing Karaf as a service... "
    # Run a setup script to add some feature repos and prepare it for running as a service
    $KARAF_DIR/bin/start
    sleep 60
    `${KARAF_CLIENT} -f ${KARAF_CONFIGS}/karaf_service.script`
    $KARAF_DIR/bin/stop

    # Add it as a Linux service
    ln -s $KARAF_DIR/bin/karaf-service /etc/init.d/
    update-rc.d karaf-service defaults
    echo " done"
fi

# Add the vagrant user's maven repository
if ! grep -q "$HOME_DIR/.m2/repository" $KARAF_DIR/etc/org.ops4j.pax.url.mvn.cfg ; then
    echo "Adding vagrant user's Maven repository... "
    sed -i "s|#org.ops4j.pax.url.mvn.localRepository=|org.ops4j.pax.url.mvn.localRepository=$HOME_DIR/.m2/repository|" $KARAF_DIR/etc/org.ops4j.pax.url.mvn.cfg
    echo " done"
fi

# Copy modified karaf logging
cp "$KARAF_CONFIGS/org.ops4j.pax.logging.cfg" $KARAF_DIR/etc/

# Delete the data directory to get a fresh start
if [ -d "$KARAF_DIR/data" ]; then
  rm -rf "$KARAF_DIR/data"
fi

# Start it
echo "Starting Karaf as a service... "
service karaf-service start
sleep 60
echo "done"


