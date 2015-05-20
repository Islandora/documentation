echo "Installing Islandora Services"

HOME_DIR=$1

cd "$HOME_DIR"/islandora/camel/services
mvn
cp target/services-0.0-SNAPSHOT.war /var/lib/tomcat7/webapps/islandora-services.war
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/islandora-services.war
