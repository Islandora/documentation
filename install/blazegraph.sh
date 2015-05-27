BLAZEGRAPH_VERSION=1.5.1
echo "Installing BlazeGraph's NanoSparqlServer"

cd /var/lib/tomcat7/webapps
wget -q -O bigdata.war "http://sourceforge.net/projects/bigdata/files/bigdata/$BLAZEGRAPH_VERSION/bigdata.war/download"
chown tomcat7:tomcat7 bigdata.war

if [ $(grep -c 'com.bigdata.rdf.sail.webapp' /etc/default/tomcat7) -eq 0 ]; then
	echo "JAVA_OPTS=\"\$JAVA_OPTS -Dcom.bigdata.rdf.sail.webapp.ConfigParams.propertyFile=/var/lib/tomcat7/webapps/bigdata/WEB-INF/RWStore.properties\"" >> /etc/default/tomcat7
fi

service tomcat7 restart
