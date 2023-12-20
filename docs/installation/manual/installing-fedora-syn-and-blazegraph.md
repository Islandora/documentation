# Installing Fedora, Syn, and Blazegraph

## In this section, we will install:

- [Fedora 6](https://fedora.lyrasis.org/), the back-end repository that Islandora will use
- [Syn](https://github.com/Islandora/Syn), the authentication broker that will manage communication with Fedora
- [Blazegraph](https://blazegraph.com/), the resource index layer on top of Fedora for managing discoverability via RDF

## Fedora 6

### Stop the Tomcat Service

We're going to stop the Tomcat service while working on setting up Fedora to prevent any autodeploy misconfigurations.

```bash
sudo systemctl stop tomcat
```

### Creating a Working Space for Fedora

Fedora’s configuration and data won’t live with Tomcat itself; rather, we’re going to prepare a space for them to make them easier to manage.

```bash
sudo mkdir -p /opt/fcrepo/data/objects
sudo mkdir /opt/fcrepo/config
sudo chown -R tomcat:tomcat /opt/fcrepo
```

### Creating a Database for Fedora

The method for creating the database here will closely mimic the method we used to create our database for Drupal.

```bash
sudo -u postgres psql
create database FEDORA_DB encoding 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8' TEMPLATE template0;
create user FEDORA_DB_USER with encrypted password 'FEDORA_DB_PASSWORD';
grant all privileges on database FEDORA_DB to FEDORA_DB_USER;
\q
```

- `FEDORA_DB`: `fcrepo`
    - This will be used as the database Fedora will store the repository in.
- `FEDORA_DB_USER`: `fedora`
- `FEDORA_DB_PASSWORD`: `fedora`
    - Again, this should be a secure password of some kind; leaving it as `fedora` is not recommended.

### Adding a Fedora Configuration

The Fedora configuration is going to come in a few different chunks that need to be in place before Fedora will be functional. We’re going to place several files outright, with mildly modified parameters according to our configuration.

The basics of these configuration files have been pulled largely from the templates in Islandora-Devops/islandora-playbook [internal Fedora role](https://github.com/Islandora-Devops/islandora-playbook/tree/dev/roles/internal/Islandora-Devops.fcrepo); you may consider referencing the playbook’s templates directory for more details.

#### Namespace prefixes

`i8_namespaces.yml` is a list of namespaces used by Islandora that may not necessarily be present in Fedora; we add them here to ensure we can use them in queries.

`/opt/fcrepo/config/i8_namespaces.yml | tomcat:tomcat/644`
```{ .yaml .copy }
# Islandora 8/Fedora namespaces
#
# This file contains ALL the prefix mappings, if a URI
# does not appear in this file it will be displayed as 
# the full URI in Fedora. 
acl: http://www.w3.org/ns/auth/acl#
bf: http://id.loc.gov/ontologies/bibframe/
cc: http://creativecommons.org/ns#
dc: http://purl.org/dc/elements/1.1/
dcterms: http://purl.org/dc/terms/
dwc: http://rs.tdwg.org/dwc/terms/
ebucore: http://www.ebu.ch/metadata/ontologies/ebucore/ebucore#
exif: http://www.w3.org/2003/12/exif/ns#
fedoraconfig: http://fedora.info/definitions/v4/config#
fedoramodel: info:fedora/fedora-system:def/model#
foaf: http://xmlns.com/foaf/0.1/
geo: http://www.w3.org/2003/01/geo/wgs84_pos#
gn: http://www.geonames.org/ontology#
iana: http://www.iana.org/assignments/relation/
islandorarelsext: http://islandora.ca/ontology/relsext#
islandorarelsint: http://islandora.ca/ontology/relsint#
ldp: http://www.w3.org/ns/ldp#
memento: http://mementoweb.org/ns#
nfo: http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#
ore: http://www.openarchives.org/ore/terms/
owl: http://www.w3.org/2002/07/owl#
premis: http://www.loc.gov/premis/rdf/v1#
prov: http://www.w3.org/ns/prov#
rdf: http://www.w3.org/1999/02/22-rdf-syntax-ns#
rdfs: http://www.w3.org/2000/01/rdf-schema#
rel: http://id.loc.gov/vocabulary/relators/
schema: http://schema.org/
skos: http://www.w3.org/2004/02/skos/core#
test: info:fedora/test/
vcard: http://www.w3.org/2006/vcard/ns#
webac: http://fedora.info/definitions/v4/webac#
xml: http://www.w3.org/XML/1998/namespace
xmlns: http://www.w3.org/2000/xmlns/
xs: http://www.w3.org/2001/XMLSchema
xsi: http://www.w3.org/2001/XMLSchema-instance
```

#### Allowed External Content Hosts

We have Fedora provide metadata for some resources that are contained in Drupal. Fedora needs to know to allow access to these External Content hosts.

We create a file `/opt/fcrepo/config/allowed_external_hosts.txt | tomcat:tomcat/644`
```
http://localhost:8000/
```

**Note**: the trailing backslash is important here. For more information on Fedora's External Content and configuring it, see the [Fedora Wiki pages](https://wiki.lyrasis.org/display/FEDORA6x/External+Content)

#### Fedora configuration properties file

Fedora 6 now allows you to put all your configuration properties into a single file. We use `0640` permissions as you will want to put your database credentials in here.

`/opt/fcrepo/config/fcrepo.properties | tomcat:tomcat/640`
```{ .text .copy }
fcrepo.home=FCREPO_HOME
# External content using path defined above.
fcrepo.external.content.allowed=/opt/fcrepo/config/allowed_external_hosts.txt
# Namespace registry using path defined above.
fcrepo.namespace.registry=/opt/fcrepo/config/i8_namespaces.yml
fcrepo.auth.principal.header.enabled=true
# The principal header is the syn-setting.xml "config" element's "header" attribute
fcrepo.auth.principal.header.name=X-Islandora
# false to use manual versioning, true to create a version on each change
fcrepo.autoversioning.enabled=true
fcrepo.db.url=FCREPO_DB_URL
fcrepo.db.user=FCREPO_DB_USERNAME
fcrepo.db.password=FCREPO_DB_PASSWORD
fcrepo.ocfl.root=FCREPO_OCFL_ROOT
fcrepo.ocfl.temp=FCREPO_TEMP_ROOT
fcrepo.ocfl.staging=FCREPO_STAGING_ROOT
# Can be sha512 or sha256
fcrepo.persistence.defaultDigestAlgorithm=sha512
# Jms moved from 61616 to allow external ActiveMQ to use that port
fcrepo.dynamic.jms.port=61626
# Same as above
fcrepo.dynamic.stomp.port=61623
fcrepo.velocity.runtime.log=FCREPO_VELOCITY_LOG
fcrepo.jms.baseUrl=FCREPO_JMS_BASE
```

* `FCREPO_HOME` - The home directory for all Fedora generated output and state.  Unless otherwise specified, all logs, metadata, binaries, and internally generated indexes, etc. It would default to the Tomcat starting directory. A good default would be `/opt/fcrepo`
* `FCREPO_DB_URL` - This parameter allows you to set the database connection url. In general the format is as follows:

	 `jdbc:<database_type>://<database_host>:<database_port>/<database_name>` 

     Fedora currently supports H2, PostgresQL 12.3, MariaDB 10.5.3, and MySQL 8.0

     So using the default ports for the supported databases here are the values we typically use:

	 * PostgresQL: `jdbc:postgresql://localhost:5432/fcrepo`
	 * MariaDB:  `jdbc:mariadb://localhost:3306/fcrepo`
	 * MySQL:  `jdbc:mysql://localhost:3306/fcrepo`

* `FCREPO_DB_USERNAME` - The database username
* `FCREPO_DB_PASSWORD` - The database password
* `FCREPO_OCFL_ROOT` - Sets the root directory of the OCFL. Defaults to `FCREPO_HOME/data/ocfl-root` if not set.
* `FCREPO_TEMP_ROOT` - Sets the temp directory used by OCFL. Defaults to `FCREPO_HOME/data/temp` if not set.
* `FCREPO_STAGING_ROOT` - Sets the staging directory used by OCFL. Defaults to `FCREPO_HOME/data/staging` if not set.
* `FCREPO_VELOCITY_LOG` - The Fedora HTML template code uses Apache Velocity, which generates a runtime log called velocity.log. Defaults to `FCREPO_HOME/logs/velocity`. A good choice might be /opt/tomcat/logs/velocity.log
* `FCREPO_JMS_BASE` - This specifies the baseUrl to use when generating JMS messages. You can specify the hostname with or without port and with or without path. If your system is behind a NAT firewall you may need this to avoid your message consumers trying to access the system on an invalid port. If this system property is not set, the host, port and context from the user's request will be used in the emitted JMS messages. If your Alpaca is on the same machine as your Fedora and you use the `islandora-indexing-fcrepo`, you could use http://localhost:8080/fcrepo/rest. 


Check the Lyrasis Wiki to find all of [Fedora's properties](https://wiki.lyrasis.org/display/FEDORA6x/Properties)

### Adding the Fedora Variables to `JAVA_OPTS`

We need our Tomcat `JAVA_OPTS` to include references to our repository configuration.

`/opt/tomcat/bin/setenv.sh`

**Before**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -server -Xmx1500m -Xms1000m"

**After**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dfcrepo.config.file=/opt/fcrepo/config/fcrepo.properties -DconnectionTimeout=-1 -server -Xmx1500m -Xms1000m"

### Ensuring Tomcat Users Are In Place

While not strictly necessary, we can use the `tomcat-users.xml` file to give us direct access to the Fedora endpoint. Fedora defines, out of the box, a `fedoraAdmin` and `fedoraUser` role that can be reflected in the users list for access. The following file will also include the base `tomcat` user. As always, these default passwords should likely not stay as the defaults.

`/opt/tomcat/conf/tomcat-users.xml | tomcat:tomcat/600`
```{ .xml .copy }
<?xml version="1.0" encoding="UTF-8"?>
<tomcat-users xmlns="http://tomcat.apache.org/xml"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://tomcat.apache.org/xml tomcat-users.xsd"
              version="1.0">
  <role rolename="tomcat"/>
  <role rolename="fedoraAdmin"/>
  <role rolename="fedoraUser"/>
  <user username="tomcat" password="TOMCAT_PASSWORD" roles="tomcat"/>
  <user username="fedoraAdmin" password="FEDORA_ADMIN_PASSWORD" roles="fedoraAdmin"/>
  <user username="fedoraUser" password="FEDORA_USER_PASSWORD" roles="fedoraUser"/>
</tomcat-users>
```

- `TOMCAT_PASSWORD`: `tomcat`
- `FEDORA_ADMIN_PASSWORD`: `islandora`
- `FEDORA_USER_PASSWORD`: `islandora`

### Downloading and Placing the Latest Release

Fedora `.war` files are packaged up as releases on the official GitHub repository. You should download the most recent stable release.

```bash
sudo wget -O fcrepo.war FCREPO_WAR_URL
sudo mv fcrepo.war /opt/tomcat/webapps
sudo chown tomcat:tomcat /opt/tomcat/webapps/fcrepo.war
```

- `FCREPO_WAR_URL`: This can be found at the [fcrepo downloads page](https://github.com/fcrepo/fcrepo/releases); the file you're looking for is:
    - Tagged in green as the 'Latest release'
    - Named "fcrepo-webapp-VERSION.war"

### Start the Tomcat Service

As before, start the Tomcat service to get Fedora up and running.

```bash
sudo systemctl start tomcat
```

**Note:** sometimes it takes a while for Fedora and Tomcat to start up, usually it shouldn't take longer than 5 minutes.

Once it starts up, Fedora REST API should be available at http://localhost:8080/fcrepo/rest. The username is fedoraAdmin and we defined the password before as `FEDORA_ADMIN_PASSWORD` (default: "islandora").

## Syn

### Downloading the Syn JAR File

A compiled JAR of Syn can be found on the [Syn releases page](https://github.com/Islandora/Syn/releases). We’re going to add this to the list libraries accessible to Tomcat.

```
sudo wget -P /opt/tomcat/lib SYN_JAR_URL
# Ensure the library has the correct permissions.
sudo chown -R tomcat:tomcat /opt/tomcat/lib
sudo chmod -R 640 /opt/tomcat/lib
```

- `SYN_JAR_URL`: The latest stable release of the Syn JAR from the [releases page](https://github.com/Islandora/Syn/releases). Specifically, the JAR compiled as `-all.jar` is required.

### Generating an SSL Key for Syn

For Islandora and Fedora to talk to each other, an SSL key needs to be generated for use with Syn. We’re going to make a spot where such keys can live, and generate one.

```bash
sudo mkdir /opt/keys
sudo openssl genrsa -out "/opt/keys/syn_private.key" 2048
sudo openssl rsa -pubout -in "/opt/keys/syn_private.key" -out "/opt/keys/syn_public.key"
sudo chown www-data:www-data /opt/keys/syn*
```

### Placing the Syn Settings

Syn sites and tokens belong in a settings file that we’re going to reference in Tomcat.

`/opt/fcrepo/config/syn-settings.xml | tomcat:tomcat/600`
```{ .xml .copy }
<config version='1' header='X-Islandora'>
  <site algorithm='RS256' encoding='PEM' anonymous='true' default='true' path='/opt/keys/syn_public.key'/>
  <token user='islandora' roles='fedoraAdmin'>ISLANDORA_SYN_TOKEN</token>
</config>
```

- `ISLANDORA_SYN_TOKEN`: `islandora`
    - This should be a secure generated token rather than this default; it will be configured on the Drupal side later.

### Adding the Syn Valve to Tomcat

Referencing the valve we’ve created in our `syn-settings.xml` involves creating a `<Valve>` entry in Tomcat’s `context.xml`:

There are two options here: 

#### 1. Enable the Syn Valve for all of Tomcat.

`/opt/tomcat/conf/context.xml`

**Before**:
> 29 |     `-->`

> 30 | `</Context>`

**After**:
> 29 |    `-->`

> 30 |    `<Valve className="ca.islandora.syn.valve.SynValve" pathname="/opt/fcrepo/config/syn-settings.xml"/>`

> 31 | `</Context>`

#### 2. Enable the Syn Valve for only Fedora.

Create a new file at

`/opt/tomcat/conf/Catalina/localhost/fcrepo.xml`

```{ .xml .copy }
<Context>
	<Valve className="ca.islandora.syn.valve.SynValve" pathname="/opt/fcrepo/config/syn-settings.xml"/>
</Context>
```

Your Fedora web application needs to be deployed in Tomcat with the name `fcrepo.war`. Otherwise, change the name of the above XML file to match the deployed web application's name.

### Restarting Tomcat

Finally, restart tomcat to apply the new configurations.

```bash
sudo systemctl restart tomcat
```

**Note:** sometimes it takes a while for Fedora and Tomcat to start up, usually it shouldn't take longer than 5 minutes.

**Note:** after installing the Syn valve, you'll no longer be able to manually create/edit or delete objects via Fedora Web UI. All communication with Fedora will now be handled from the Islandora module in Drupal.

### Redhat logging

Redhat systems have stopped generating an all inclusive `catalina.out`, the `catalina.<date>.log` does not include web application's log statements. To get Fedora log statements flowing, you can create your own [LogBack](https://logback.qos.ch/) configuration file and point to it.
	
`/opt/fcrepo/config/fcrepo-logback.xml | tomcat:tomcat/644`
```{ .xml .copy }
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration>
<configuration>
  <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <encoder>
      <pattern>%p %d{HH:mm:ss.SSS} [%thread] \(%c{0}\) %m%n</pattern>
    </encoder>
  </appender>

  <appender name="FILE" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <file>${catalina.base}/logs/fcrepo.log</file>
    <append>true</append>
    <rollingPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedRollingPolicy">
      <fileNamePattern>${catalina.base}/logs/fcrepo.%d{yyyy-MM-dd}.log.%i</fileNamePattern>
      <maxFileSize>10MB</maxFileSize>
      <maxHistory>30</maxHistory>
      <totalSizeCap>2GB</totalSizeCap>
    </rollingPolicy>
    <encoder>
      <pattern>%p %d{HH:mm:ss.SSS} [%thread] \(%c{0}\) %m%n</pattern>
    </encoder>
  </appender>

  <logger name="org.fcrepo.auth" additivity="false" level="${fcrepo.log.auth:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.config" additivity="false" level="${fcrepo.log.config:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.event" additivity="false" level="${fcrepo.log.event:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.http.api" additivity="false" level="${fcrepo.log.http.api:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.http.commons" additivity="false" level="${fcrepo.log.http.commons:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.jms" additivity="false" level="${fcrepo.log.jms:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.kernel" additivity="false" level="${fcrepo.log.kernel:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.persistence" additivity="false" level="${fcrepo.log.persistence:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.search" additivity="false" level="${fcrepo.log.search:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo.storage" additivity="false" level="${fcrepo.log.storage:-null}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>
  <logger name="org.fcrepo" additivity="false" level="${fcrepo.log:-INFO}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </logger>

  <root level="${fcrepo.log.root:-WARN}">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
  </root>
</configuration>
```

Then alter your `$JAVA_OPTS` like [above](#adding-the-fedora-variables-to-java_opts) to include 
```
-Dlogback.configurationFile=/opt/fcrepo/config/fcrepo-logback.xml
```

This will generate a log file at `${catalina.base}/logs/fcrepo.log` and will rotate each day or if the logs reaches 10MB. It will maintain 30 days of old logs, or 2GB whichever comes first.

## Blazegraph 2

### Creating a Working Space for Blazegraph

Blazegraph needs a space for configurations and data; we’re going to create this space in `/opt`.

```bash
sudo mkdir -p /opt/blazegraph/data
sudo mkdir /opt/blazegraph/conf
sudo chown -R tomcat:tomcat /opt/blazegraph
```

### Downloading and Placing the Blazegraph WAR

The Blazegraph `.war` file can be found in a few different places, but to ensure we’re able to easily `wget` it, we’re going to use the [maven.org](https://search.maven.org/) repository link to grab it.

```bash
cd /opt
sudo wget -O blazegraph.war BLAZEGRAPH_WARFILE_LINK
sudo mv blazegraph.war /opt/tomcat/webapps
sudo chown tomcat:tomcat /opt/tomcat/webapps/blazegraph.war
```

- BLAZEGRAPH_WAR_URL: You can find a link to this at the [Maven repository for Blazegraph](https://repo1.maven.org/maven2/com/blazegraph/bigdata-war/); you’ll want to click the link for the latest version of Blazegraph 2.1.x, then get the link to the `.war` file within that version folder.

Once this is downloaded, give it a moment to expand before moving on to the next step.

### Configuring Logging

We would like to have an appropriate logging configuration for Blazegraph, which can be useful for looking at incoming traffic and determining if anything has gone wrong with Blazegraph. Our logger isn’t going to be much different than the default logger; it can be made more or less verbose by changing the default `WARN` levels. There are several other loggers that can be enabled, like a SPARQL query trace or summary query evaluation log; if these are desired they should be added in. Consult the Blazegraph documentation for more details.

`/opt/blazegraph/conf/log4j.properties | tomcat:tomcat/644`
```{ .text .copy }
log4j.rootCategory=WARN, dest1

# Loggers.
log4j.logger.com.bigdata=WARN
log4j.logger.com.bigdata.btree=WARN

# Normal data loader (single threaded).
#log4j.logger.com.bigdata.rdf.store.DataLoader=INFO

# dest1
log4j.appender.dest1=org.apache.log4j.ConsoleAppender
log4j.appender.dest1.layout=org.apache.log4j.PatternLayout
log4j.appender.dest1.layout.ConversionPattern=%-5p: %F:%L: %m%n
#log4j.appender.dest1.layout.ConversionPattern=%-5p: %r %l: %m%n
#log4j.appender.dest1.layout.ConversionPattern=%-5p: %m%n
#log4j.appender.dest1.layout.ConversionPattern=%-4r [%t] %-5p %c %x - %m%n
#log4j.appender.dest1.layout.ConversionPattern=%-4r(%d) [%t] %-5p %c(%l:%M) %x - %m%n

# Rule execution log. This is a formatted log file (comma delimited).
log4j.logger.com.bigdata.relation.rule.eval.RuleLog=INFO,ruleLog
log4j.additivity.com.bigdata.relation.rule.eval.RuleLog=false
log4j.appender.ruleLog=org.apache.log4j.FileAppender
log4j.appender.ruleLog.Threshold=ALL
log4j.appender.ruleLog.File=/var/log/blazegraph/rules.log
log4j.appender.ruleLog.Append=true
log4j.appender.ruleLog.BufferedIO=false
log4j.appender.ruleLog.layout=org.apache.log4j.PatternLayout
log4j.appender.ruleLog.layout.ConversionPattern=%m
```

### Adding a Blazegraph Configuration

Our configuration will be built from a few different files that we will eventually reference in `JAVA_OPTS` and directly apply to Blazegraph; these include most of the functional pieces Blazegraph requires, as well as a generalized configuration for the `islandora` namespace it will use. As with most large configurations like this, these should likely be tuned to your preferences, and the following files only represent sensible defaults.

`/opt/blazegraph/conf/RWStore.properties | tomcat:tomcat/644`
``` { .text .copy }
com.bigdata.journal.AbstractJournal.file=/opt/blazegraph/data/blazegraph.jnl
com.bigdata.journal.AbstractJournal.bufferMode=DiskRW
com.bigdata.service.AbstractTransactionService.minReleaseAge=1
com.bigdata.journal.Journal.groupCommit=false
com.bigdata.btree.writeRetentionQueue.capacity=4000
com.bigdata.btree.BTree.branchingFactor=128
com.bigdata.journal.AbstractJournal.initialExtent=209715200
com.bigdata.journal.AbstractJournal.maximumExtent=209715200
com.bigdata.rdf.sail.truthMaintenance=false
com.bigdata.rdf.store.AbstractTripleStore.quads=true
com.bigdata.rdf.store.AbstractTripleStore.statementIdentifiers=false
com.bigdata.rdf.store.AbstractTripleStore.textIndex=false
com.bigdata.rdf.store.AbstractTripleStore.axiomsClass=com.bigdata.rdf.axioms.NoAxioms
com.bigdata.namespace.kb.lex.com.bigdata.btree.BTree.branchingFactor=400
com.bigdata.namespace.kb.spo.com.bigdata.btree.BTree.branchingFactor=1024
com.bigdata.journal.Journal.collectPlatformStatistics=false
```

`/opt/blazegraph/conf/blazegraph.properties | tomcat:tomcat/644`
```{ .text .copy }
com.bigdata.rdf.store.AbstractTripleStore.textIndex=false
com.bigdata.rdf.store.AbstractTripleStore.axiomsClass=com.bigdata.rdf.axioms.OwlAxioms
com.bigdata.rdf.sail.isolatableIndices=false
com.bigdata.rdf.store.AbstractTripleStore.justify=true
com.bigdata.rdf.sail.truthMaintenance=true
com.bigdata.rdf.sail.namespace=islandora
com.bigdata.rdf.store.AbstractTripleStore.quads=false
com.bigdata.namespace.islandora.lex.com.bigdata.btree.BTree.branchingFactor=400
com.bigdata.journal.Journal.groupCommit=false
com.bigdata.namespace.islandora.spo.com.bigdata.btree.BTree.branchingFactor=1024
com.bigdata.rdf.store.AbstractTripleStore.geoSpatial=false
com.bigdata.rdf.store.AbstractTripleStore.statementIdentifiers=false
```

`/opt/blazegraph/conf/inference.nt | tomcat:tomcat/644`
```{ .text .copy }
<http://pcdm.org/models#memberOf> <http://www.w3.org/2002/07/owl#inverseOf> <http://pcdm.org/models#hasMember> .
<http://pcdm.org/models#fileOf> <http://www.w3.org/2002/07/owl#inverseOf> <http://pcdm.org/models#hasFile> .
```

### Specifying the `RWStore.properties` in `JAVA_OPTS`

In order to enable our configuration when Tomcat starts, we need to reference the location of `RWStore.properties` in the `JAVA_OPTS` environment variable that Tomcat uses.

`/opt/tomcat/bin/setenv.sh`

**Before**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dfcrepo.config.file=/opt/fcrepo/config/fcrepo.properties -DconnectionTimeout=-1 -server -Xmx1500m -Xms1000m"

**After**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dfcrepo.config.file=/opt/fcrepo/config/fcrepo.properties -DconnectionTimeout=-1 -Dcom.bigdata.rdf.sail.webapp.ConfigParams.propertyFile=/opt/blazegraph/conf/RWStore.properties -Dlog4j.configuration=file:/opt/blazegraph/conf/log4j.properties -server -Xmx1500m -Xms1000m"


### Restarting Tomcat

Finally, restart Tomcat to pick up the changes we’ve made.

```bash
sudo systemctl restart tomcat
```

### Installing Blazegraph Namespaces and Inference

The two other files we created, `blazegraph.properties` and `inference.nt`, contain information that Blazegraph requires in order to establish and correctly use the datasets Islandora will send to it. First, we need to create a dataset - contained in `blazegraph.properties` - and then we need to inform that dataset of the inference set we have contained in `inference.nt`.

``` { .bash .copy }
curl -X POST -H "Content-Type: text/plain" --data-binary @/opt/blazegraph/conf/blazegraph.properties http://localhost:8080/blazegraph/namespace
```
If this worked correctly, Blazegraph should respond with "CREATED: islandora" to let us know it created the islandora namespace.
``` { .bash .copy }
curl -X POST -H "Content-Type: text/plain" --data-binary @/opt/blazegraph/conf/inference.nt http://localhost:8080/blazegraph/namespace/islandora/sparql
```
If this worked correctly, Blazegraph should respond with some XML letting us know it added the 2 entries from inference.nt to the namespace.
