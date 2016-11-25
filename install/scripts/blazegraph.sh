#!/bin/bash
echo "Installing BlazeGraph's NanoSparqlServer"

HOME_DIR=$1

if [ -f "$HOME_DIR/islandora/install/configs/variables" ]; then
  . "$HOME_DIR"/islandora/install/configs/variables
fi

if [ ! -f "$DOWNLOAD_DIR/bigdata-$BLAZEGRAPH_VERSION.war" ]; then
  echo "Downloading Blazegraph version $BLAZEGRAPH_VERSION"
  wget -q -O "$DOWNLOAD_DIR/bigdata-$BLAZEGRAPH_VERSION.war" "http://sourceforge.net/projects/bigdata/files/bigdata/$BLAZEGRAPH_VERSION/bigdata.war/download"
fi

cd /var/lib/tomcat7/webapps
cp -v "$DOWNLOAD_DIR/bigdata-$BLAZEGRAPH_VERSION.war" "/var/lib/tomcat7/webapps/bigdata.war"
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/bigdata.war

cd /opt
mkdir blazegraph
chown tomcat7:tomcat7 /opt/blazegraph

if [ $(grep -c 'com.bigdata.rdf.sail.webapp' /etc/default/tomcat7) -eq 0 ]; then
	echo "JAVA_OPTS=\"\$JAVA_OPTS -Dcom.bigdata.rdf.sail.webapp.ConfigParams.propertyFile=/var/lib/tomcat7/webapps/bigdata/WEB-INF/RWStore.properties\"" >> /etc/default/tomcat7
fi

service tomcat7 restart
sleep 15
sed -i 's|log4j.appender.ruleLog.File=rules.log|log4j.appender.ruleLog.File=/var/log/tomcat7/rules.log|g' /var/lib/tomcat7/webapps/bigdata/WEB-INF/classes/log4j.properties
sed -i 's|com.bigdata.journal.AbstractJournal.file=blazegraph.jnl|com.bigdata.journal.AbstractJournal.file=/opt/blazegraph/blazegraph.jnl|g' /var/lib/tomcat7/webapps/bigdata/WEB-INF/RWStore.properties
service tomcat7 restart
