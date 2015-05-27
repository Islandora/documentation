FUSEKI_VERSION=2.0.0
FUSEKI_DEPLOY=/var/lib/tomcat7/webapps
FUSEKI_BASE=/etc/fuseki

mkdir $FUSEKI_BASE
chown -hR tomcat7:tomcat7 $FUSEKI_BASE

cd /tmp
wget http://www.apache.org/dist/jena/binaries/apache-jena-fuseki-"$FUSEKI_VERSION".tar.gz
tar -xzvf apache-jena-fuseki-"$FUSEKI_VERSION".tar.gz
cd apache-jena-fuseki-"$FUSEKI_VERSION"
mv -v fuseki.war $FUSEKI_DEPLOY
chown -hR tomcat7:tomcat7 $FUSEKI_DEPLOY/fuseki.war

service tomcat7 restart
sleep 20
sed -i 's|^\/$\/\*\* = localhost|\#\/$\/\*\* = localhost|g' $FUSEKI_BASE/shiro.ini
service tomcat7 restart

