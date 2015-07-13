KARAF_VERSION=3.0.4

HOME_DIR=$1

cd $HOME_DIR

# Download and install Karaf
wget http://mirror.csclub.uwaterloo.ca/apache/karaf/"$KARAF_VERSION"/apache-karaf-"$KARAF_VERSION".tar.gz
tar zxvf apache-karaf-"$KARAF_VERSION".tar.gz
rm apache-karaf-"$KARAF_VERSION".tar.gz
mv apache-karaf-"$KARAF_VERSION" /opt
ln -s /opt/apache-karaf-"$KARAF_VERSION" /opt/karaf

# Run a setup script to add some feature repos and prepare it for running as a service
/opt/karaf/bin/start
sleep 5
/opt/karaf/bin/client < "$HOME_DIR"/islandora/install/karaf/setup.script
/opt/karaf/bin/stop

# Add it as a Linux service
ln -s /opt/karaf/bin/karaf-service /etc/init.d/
update-rc.d karaf-service defaults

# Start it
service karaf-service start

# You can always log into the karaf console once the service is running by executing:
# /opt/karaf/bin/client
