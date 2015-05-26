echo "Installing Fedora."

HOME_DIR=$1
FEDORA_VERSION=4.2.0

cd $HOME_DIR

mkdir /var/lib/tomcat7/fcrepo4-data
chown tomcat7:tomcat7 /var/lib/tomcat7/fcrepo4-data
chmod g-w /var/lib/tomcat7/fcrepo4-data

wget -O fcrepo.war "https://github.com/fcrepo4/fcrepo4/releases/download/fcrepo-$FEDORA_VERSION/fcrepo-webapp-$FEDORA_VERSION.war"
mv fcrepo.war /var/lib/tomcat7/webapps
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fcrepo.war
sed -i 's#JAVA_OPTS="-Djava.awt.headless=true -Xmx128m -XX:+UseConcMarkSweepGC"#JAVA_OPTS="-Djava.awt.headless=true -Dfile.encoding=UTF-8 -server -Xms512m -Xmx1024m -XX:NewSize=256m -XX:MaxNewSize=256m -XX:PermSize=256m -XX:MaxPermSize=256m"#g' /etc/default/tomcat7
service tomcat7 restart
