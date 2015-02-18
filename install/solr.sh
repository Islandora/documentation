echo "Installing Solr"

SOLR_VERSION=4.10.3
SOLR_HOME=/var/lib/tomcat7/solr

mkdir $SOLR_HOME

cd /tmp
wget http://archive.apache.org/dist/lucene/solr/"$SOLR_VERSION"/solr-"$SOLR_VERSION".tgz
tar -xzvf solr-"$SOLR_VERSION".tgz
cp -v /tmp/solr-"$SOLR_VERSION"/dist/solr-"$SOLR_VERSION".war /var/lib/tomcat7/webapps/solr.war
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/solr.war

cd /usr/share/tomcat7/lib
wget http://repo1.maven.org/maven2/commons-logging/commons-logging/1.1.2/commons-logging-1.1.2.jar

cp /tmp/solr-"$SOLR_VERSION"/example/lib/ext/slf4j* /usr/share/tomcat7/lib
cp /tmp/solr-"$SOLR_VERSION"/example/lib/ext/log4j* /usr/share/tomcat7/lib

chown -hR tomcat7:tomcat7 /usr/share/tomcat7/lib

cp -Rv /tmp/solr-"$SOLR_VERSION"/example/solr/* $SOLR_HOME

chown -hR tomcat7:tomcat7 $SOLR_HOME

touch /var/lib/tomcat7/velocity.log
chown tomcat7:tomcat7 /var/lib/tomcat7/velocity.log

service tomcat7 restart
