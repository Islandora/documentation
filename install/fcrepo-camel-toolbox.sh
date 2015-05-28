echo "Installing Fcrepo-Camel-Toolbox"

CAMEL_VERSION=4.2.0
TRIPLESTORE=$1

cd /var/lib/tomcat7/webapps
wget -O fcrepo-camel-toolbox.war "https://github.com/fcrepo4-labs/fcrepo-camel-toolbox/releases/download/fcrepo-camel-toolbox-$CAMEL_VERSION/fcrepo-camel-webapp-at-is-it-$CAMEL_VERSION.war"
chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fcrepo-camel-toolbox.war

if [ $(grep -c '\-Dtriplestore.baseUrl=' /etc/default/tomcat7) -eq 0 ]; then
  if [ "$TRIPLESTORE" == 'fuseki' ]; then
  	echo "JAVA_OPTS=\"\$JAVA_OPTS -Dtriplestore.baseUrl=localhost:8080/fuseki/test/update\"" >> /etc/default/tomcat7
  elif [ "$TRIPLESTORE" == 'blazegraph' ]; then
  	echo "JAVA_OPTS=\"\$JAVA_OPTS -Dtriplestore.baseUrl=localhost:8080/bigdata/sparql\"" >> /etc/default/tomcat7
  fi
fi
/etc/init.d/tomcat7 restart
