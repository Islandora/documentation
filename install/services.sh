echo "Installing Islandora Services" 

HOME_DIR=$1

mysql --user=root --password=islandora --execute="CREATE DATABASE islandora_services"

cd "$HOME_DIR"
git clone https://code.google.com/a/apache-extras.org/p/camel-extra/
cd camel-extra
mvn install

cd "$HOME_DIR"/islandora/camel/services
mvn
cp target/services-0.0-SNAPSHOT.war /var/lib/tomcat7/webapps/islandora-services.war
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/islandora-services.war
