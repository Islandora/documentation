echo "Installing Fcrepo Camel Component"

FCREPO_CAMEL_VERSION=4.1.0
FCREPO_CAMEL_HOME=/usr/share/fcrepo-camel

mkdir $FCREPO_CAMEL_HOME
cd /tmp
wget https://github.com/fcrepo4/fcrepo-camel/releases/download/fcrepo-camel-"$FCREPO_CAMEL_VERSION"/fcrepo-camel-"$FCREPO_CAMEL_VERSION".jar
mv -v fcrepo-camel-"$FCREPO_CAMEL_VERSION".jar $FCREPO_CAMEL_HOME
chown -hR tomcat7:tomcat7 $FCREPO_CAMEL_HOME
