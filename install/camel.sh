echo "Installing Apache Camel"

CAMEL_VERSION=2.14.1
CAMEL_PATH=/usr/share/camel

mkdir $CAMEL_PATH
cd /tmp
wget http://mirror.csclub.uwaterloo.ca/apache/camel/apache-camel/"$CAMEL_VERSION"/apache-camel-"$CAMEL_VERSION".tar.gz
tar -xzvf apache-camel-"$CAMEL_VERSION".tar.gz
cd apache-camel-"$CAMEL_VERSION"
mv -v * $CAMEL_PATH
chown -hR tomcat7:tomcat7 $CAMEL_PATH
