# Installing Fedora, Syn, and Blazegraph

## In this section, we will install:

- [Fedora 5](https://duraspace.org/fedora/), the back-end repository that Islandora will use
- [Syn](https://github.com/Islandora/Syn), the authentication broker that will manage communication with Fedora
- [Blazegraph](https://blazegraph.com/), the resource index layer on top of Fedora for managing discoverability via RDF

## Fedora 5

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
create database FEDORA_DB;
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

The basics of these configuration files have been pulled largely from the templates in [Islandora-Devops/ansible-role-fcrepo](https://github.com/islandora-devops/ansible-role-fcrepo); you may consider referencing the playbook’s templates directory for more details.

`i8_namespaces.cnd` is a list of namespaces used by Islandora 8 that may not necessarily be present in Fedora; we add them here to ensure we can use them in queries.

`/opt/fcrepo/config/i8_namespaces.cnd | tomcat:tomcat/644`
```
<acl = 'http://www.w3.org/ns/auth/acl#'>
<bf = 'http://id.loc.gov/ontologies/bibframe/'>
<cc = 'http://creativecommons.org/ns#'>
<dcterms = 'http://purl.org/dc/terms/'>
<dwc = 'http://rs.tdwg.org/dwc/terms/'>
<exif = 'http://www.w3.org/2003/12/exif/ns#'>
<fedoramodel = 'info:fedora/fedora-system:def/model#'>
<geo = 'http://www.w3.org/2003/01/geo/wgs84_pos#'>
<gn = 'http://www.geonames.org/ontology#'>
<iana = 'http://www.iana.org/assignments/relation/'>
<islandorarelsext = 'http://islandora.ca/ontology/relsext#'>
<islandorarelsint = 'http://islandora.ca/ontology/relsint#'>
<ldp = 'http://www.w3.org/ns/ldp#'>
<nfo = 'http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#'>
<ore = 'http://www.openarchives.org/ore/terms/'>
<owl = 'http://www.w3.org/2002/07/owl#'>
<pcdm = 'http://pcdm.org/models#'>
<pcdmfmt = 'http://pcdm.org/file-format-types#'>
<pcdmrts = 'http://pcdm.org/rights#'>
<pcdmuse = 'http://pcdm.org/use#'>
<pcdmwrks = 'http://pcdm.org/works#'>
<prov = 'http://www.w3.org/ns/prov#'>
<rdf = 'http://www.w3.org/1999/02/22-rdf-syntax-ns#'>
<rdfs = 'http://www.w3.org/2000/01/rdf-schema#'>
<rel = 'http://id.loc.gov/vocabulary/relators/'>
<schema = 'http://schema.org/'>
<skos = 'http://www.w3.org/2004/02/skos/core#'>
<xml = 'http://www.w3.org/XML/1998/namespace'>
<xmlns = 'http://www.w3.org/2000/xmlns/'>
<xs = 'http://www.w3.org/2001/XMLSchema'>
<xsi = 'http://www.w3.org/2001/XMLSchema-instance'>
```

We intend to have Crayfish installed later. Since Fedora needs to be able to read data from Crayfish, we need to tell Fedora that the Crayfish endpoint is a valid data source.

`/opt/fcrepo/config/allowed_hosts.txt | tomcat:tomcat/644`
```
http://localhost:CRAYFISH_PORT/
```
- `CRAYFISH_PORT`: 80
    - This guide will install Crayfish on the same port that Drupal is installed on. This may not be desirable, and if Crayfish is installed on a different port later, that change should be reflected here.

The next part of the configuration defines where the pieces of the actual repository will live. Note that this file contains some of the defined `FEDORA_DB` variables from earlier.

`/opt/fcrepo/config/repository.json | tomcat:tomcat/644`
```json
{
    "name" : "repo",
    "jndiName" : "",
    "workspaces" : {
        "predefined" : ["default"],
        "default" : "default",
        "allowCreation" : true,
        "cacheSize" : 10000
    },
    "storage" : {
        "persistence": {
            "type" : "db",
            "connectionUrl": "jdbc:postgresql://localhost:5432/FEDORA_DB",
            "driver" : "org.postgresql.Driver",
            "username" : "FEDORA_DB_USER",
            "password" : "FEDORA_DB_PASSWORD"
        },
        "binaryStorage" : {
            "type" : "file",
            "directory" : "/opt/fcrepo/data/binaries",
            "minimumBinarySizeInBytes" : 4096
        }
    },
    "security" : {
        "anonymous" : {
            "roles" : ["readonly","readwrite","admin"],
            "useOnFailedLogin" : false
        },
        "providers" : [
            { "classname" : "org.fcrepo.auth.common.BypassSecurityServletAuthenticationProvider" }
        ]
    },
    "garbageCollection" : {
        "threadPool" : "modeshape-gc",
        "initialTime" : "00:00",
        "intervalInHours" : 24
    },
    "node-types" : ["fedora-node-types.cnd", "file:/opt/fcrepo/config/i8_namespaces.cnd"]
}
```

Finally, we need an actual `fcrepo-config.xml` to pull this configuration into place.

`/opt/fcrepo/config/fcrepo-config.xml | tomcat:tomcat/644`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:task="http://www.springframework.org/schema/task"
    xmlns:p="http://www.springframework.org/schema/p"
    xmlns:util="http://www.springframework.org/schema/util"
    xsi:schemaLocation="
    http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
    http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd
    http://www.springframework.org/schema/task http://www.springframework.org/schema/task/spring-task.xsd
    http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util.xsd">

    <context:property-placeholder/>
    <context:annotation-config/>
    <context:component-scan base-package="org.fcrepo"/>

    <bean name="modeshapeRepofactory"
        class="org.fcrepo.kernel.modeshape.spring.ModeShapeRepositoryFactoryBean"
        p:repositoryConfiguration="file:///opt/fcrepo/config/repository.json"
        depends-on="authenticationProvider"/>
    <bean name="authenticationProvider" class="org.fcrepo.auth.common.ShiroAuthenticationProvider"/>
    <bean name="headerProvider" class="org.fcrepo.auth.common.HttpHeaderPrincipalProvider">
        <property name="headerName" value="X-Islandora"/>
        <property name="separator" value=","/>
    </bean>
    <bean name="delegatedPrincipalProvider" class="org.fcrepo.auth.common.DelegateHeaderPrincipalProvider"/>
    <bean name="accessRolesProvider" class="org.fcrepo.auth.webac.WebACRolesProvider"/>
    <bean id="webACAuthorizingRealm" class="org.fcrepo.auth.webac.WebACAuthorizingRealm" />
    <bean id="servletContainerAuthenticatingRealm" class="org.fcrepo.auth.common.ServletContainerAuthenticatingRealm" />
    <bean id="securityManager" class="org.apache.shiro.web.mgt.DefaultWebSecurityManager">
      <property name="realms">
        <util:set set-class="java.util.HashSet">
          <ref bean="webACAuthorizingRealm"/>
          <ref bean="servletContainerAuthenticatingRealm"/>
        </util:set>
      </property>
    </bean>
    <bean id="lifecycleBeanPostProcessor" class="org.apache.shiro.spring.LifecycleBeanPostProcessor"/>
    <bean id="servletContainerAuthFilter" class="org.fcrepo.auth.common.ServletContainerAuthFilter"/>
    <bean id="webACFilter" class="org.fcrepo.auth.webac.WebACFilter"/>
    <bean id="shiroFilter" class="org.apache.shiro.spring.web.ShiroFilterFactoryBean">
      <property name="securityManager" ref="securityManager"/>
      <property name="filterChainDefinitions">
        <value>
          /** = servletContainerAuthFilter,headerProvider,delegatedPrincipalProvider,webACFilter
        </value>
      </property>
    </bean>

    <util:list id="translationChain" value-type="org.fcrepo.kernel.api.identifiers.InternalIdentifierConverter">
        <bean class="org.fcrepo.kernel.modeshape.identifiers.HashConverter"/>
        <bean class="org.fcrepo.kernel.modeshape.identifiers.NamespaceConverter"/>
    </util:list>

    <bean class="org.fcrepo.jms.JMSTopicPublisher">
      <constructor-arg value="fedora"/>
    </bean>
    <bean id="connectionFactory"
        class="org.apache.activemq.ActiveMQConnectionFactory" depends-on="jmsBroker"
        p:brokerURL="vm://${fcrepo.jms.host:localhost}:${fcrepo.dynamic.jms.port:61616}?create=false"/>
    <bean name="jmsBroker" class="org.apache.activemq.xbean.BrokerFactoryBean"
      p:config="${fcrepo.activemq.configuration:classpath:/config/activemq.xml}" p:start="true"/>
    <bean class="org.fcrepo.jms.DefaultMessageFactory"/>
    <bean class="org.fcrepo.kernel.modeshape.observer.SimpleObserver"/>
    <bean name="fedoraEventFilter" class="org.fcrepo.kernel.modeshape.observer.DefaultFilter"/>
    <bean name="fedoraEventMapper" class="org.fcrepo.kernel.modeshape.observer.eventmappings.AllNodeEventsOneEvent"/>
    <bean name="fedoraInternalEventBus" class="com.google.common.eventbus.EventBus"/>
    <bean name="rdfNamespaceRegistry" class="org.fcrepo.kernel.api.rdf.RdfNamespaceRegistry"
        init-method="init" destroy-method="shutdown">
      <property name="configPath" value="${fcrepo.namespace.registry:classpath:/namespaces.yml}" />
      <property name="monitorForChanges" value="true" />
    </bean>
    <bean name="externalContentPathValidator" class="org.fcrepo.http.api.ExternalContentPathValidator"
        init-method="init" destroy-method="shutdown">
        <property name="configPath" value="${fcrepo.external.content.allowed:#{null}}" />
        <property name="monitorForChanges" value="true" />
    </bean>
    <bean name="externalContentHandlerFactory" class="org.fcrepo.http.api.ExternalContentHandlerFactory">
        <property name="validator" ref="externalContentPathValidator" />
    </bean>
    <task:scheduler id="taskScheduler" />
    <task:executor id="taskExecutor" pool-size="1" />
    <task:annotation-driven executor="taskExecutor" scheduler="taskScheduler" />
    <bean class="org.modeshape.jcr.ModeShapeEngine" init-method="start"/>
    <bean id="connectionManager" class="org.apache.http.impl.conn.PoolingHttpClientConnectionManager" />
    <bean class="org.fcrepo.http.commons.session.SessionFactory"/>
</beans>
```

### Adding the Fedora Variables to `JAVA_OPTS`

We need our Tomcat `JAVA_OPTS` to include references to our repository configuration.

`/opt/tomcat/bin/setenv.sh`

**Before**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dcantaloupe.config=/opt/cantaloupe_config/cantaloupe.properties -server -Xmx1500m -Xms1000m"

**After**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dcantaloupe.config=/opt/cantaloupe_config/cantaloupe.properties -Dfcrepo.modeshape.configuration=file:///opt/fcrepo/config/repository.json -Dfcrepo.home=/opt/fcrepo/data -Dfcrepo.spring.configuration=file:///opt/fcrepo/config/fcrepo-config.xml -server -Xmx1500m -Xms1000m"

### Ensuring Tomcat Users Are In Place

While not strictly necessary, we can use the `tomcat-users.xml` file to give us direct access to the Fedora endpoint. Fedora defines, out of the box, a `fedoraAdmin` and `fedoraUser` role that can be reflected in the users list for access. The following file will also include the base `tomcat` user. As always, these default passwords should likely not stay as the defaults.

`/opt/tomcat/conf/tomcat-users.xml | tomcat:tomcat/600`
```xml
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

Fedora `.war` files are packaged up as releases on the official GitHub repository; you can find the latest version at the releases page; the official GitHub repository is labelled as fcrepo4 but does actually contain more recent versions than 4. You should download the most recent stable release.

```bash
wget -O fcrepo.war FCREPO_WAR_URL
sudo mv fcrepo.war /opt/tomcat/webapps
sudo chown tomcat:tomcat /opt/tomcat/webapps/fcrepo.war
```
- `FCREPO_WAR_URL`: This can be found at the [fcrepo downloads page](https://wiki.duraspace.org/display/FF/Downloads); you should download the WAR for the most recent 5.x web application.

#3# Restarting the Tomcat Service

As before, restart the Tomcat service to get Fedora up and running.

```bash
sudo systemctl restart tomcat
```

## Syn

### Downloading the Syn JAR File

A compiled JAR of Syn can be found on the [Syn releases page](https://github.com/Islandora/Syn/releases). We’re going to add this to the list libraries accessible to Tomcat.

```
sudo wget -P /opt/tomcat/lib SYN_JAR_URL
# Ensure the library has the correct permissions.
sudo chown tomcat:tomcat /opt/tomcat/lib/*.jar
sudo chmod 640 /opt/tomcat/lib/*.jar
```
- `SYN_JAR_URL`: The latest stable release of the Syn JAR from the releases page. Specifically, the JAR compiled as `-all.jar` is required.

### Generating an SSL Key for Syn

For Islandora and Fedora to talk to each other, an SSL key needs to be generated for use with Syn. We’re going to make a spot where such keys can live, and generate one.

```bash
sudo mkdir /opt/keys
sudo openssl genrsa -out "/opt/keys/syn_private.key" 2048
sudo openssl rsa -pubout -in "/opt/keys/syn_private.key" -out "/opt/keys/syn_public.key"
```

### Placing the Syn Settings

Syn sites and tokens belong in a settings file that we’re going to reference in Tomcat.

`/opt/fcrepo/config/syn-settings.xml | tomcat:tomcat/600`
```xml
<config version='1' header='X-Islandora'>
  <site algorithm='RS256' encoding='PEM' anonymous='true' default='true' path='/opt/keys/syn_public.key'/>
  <token user='islandora' roles='fedoraAdmin'>ISLANDORA_SYN_TOKEN</token>
</config>
```
- `ISLANDORA_SYN_TOKEN`: `islandora`
    - This should be a secure generated token rather than this default; it will be configured on the Drupal side later.

### Adding the Syn Valve to Tomcat

Referencing the valve we’ve created in our `syn-settings.xml` involves creating a `<Valve>` entry in Tomcat’s `context.xml`:

`/opt/tomcat/conf/context.xml`

**Before**:
> 29 |     -->
> 30 | </Context>

**After**:
> 29 |    -->
> 30 |    <Valve className="ca.islandora.syn.valve.SynValve" pathname="/opt/fcrepo/config/syn-settings.xml"/>
> 31 | </Context>

### Restarting Tomcat

Finally, restart tomcat to apply the new configurations.

```bash
sudo systemctl restart tomcat
```

## Blazegraph 2

### Creating a Working Space for Blazegraph

Blazegraph needs a space for configurations and data; we’re going to create this space in `/opt`.

```bash
sudo mkdir -p /opt/blazegraph/data
sudo mkdir /opt/blazegraph/conf
sudo chown -R tomcat:tomcat /opt/blazegraph
```

### Downloading and Placing the Blazegraph WAR

The Blazegraph `.war` file can be found in a few different places, but to ensure we’re able to easily `wget` it, we’re going to use the [maven.org](http://maven.org/) repository link to grab it.

```bash
cd /opt
sudo -u tomcat wget -P /opt/tomcat/webapps -O blazegraph.war BLAZEGRAPH_WARFILE_LINK
```
- BLAZEGRAPH_WAR_URL: You can find a link to this at the [Maven repository for Blazegraph](http://repo1.maven.org/maven2/com/blazegraph/bigdata-war/); you’ll want to click the link for the latest version of Blazegraph 2.1.x, then get the link to the `.war` file within that version folder.

Once this is downloaded, give it a moment to expand before moving on to the next step.

### Configuring Logging

We would like to have an appropriate logging configuration for Blazegraph, which can be useful for looking at incoming traffic and determining if anything has gone wrong with Blazegraph. Our logger isn’t going to be much different than the default logger; it can be made more or less verbose by changing the default `WARN` levels. There are several other loggers that can be enabled, like a SPARQL query trace or summary query evaluation log; if these are desired they should be added in. Consult the Blazegraph documentation for more details.

`/var/log/blazegraph/log4j.properties | tomcat:tomcat/644`
```
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
```
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
```
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
```
<http://pcdm.org/models#memberOf> <http://www.w3.org/2002/07/owl#inverseOf> <http://pcdm.org/models#hasMember> .
<http://pcdm.org/models#fileOf> <http://www.w3.org/2002/07/owl#inverseOf> <http://pcdm.org/models#hasFile> .
```

### Specifying the `RWStore.properties` in `JAVA_OPTS`

In order to enable our configuration when Tomcat starts, we need to reference the location of `RWStore.properties` in the `JAVA_OPTS` environment variable that Tomcat uses.

`/opt/tomcat/bin/setenv.sh`

**Before**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dcantaloupe.config=/opt/cantaloupe_config/cantaloupe.properties -Dfcrepo.modeshape.configuration=file:///opt/fcrepo/config/repository.json -Dfcrepo.home=/opt/fcrepo/data -Dfcrepo.spring.configuration=file:///opt/fcrepo/config/fcrepo-config.xml -server -Xmx1500m -Xms1000m"

**After**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dcantaloupe.config=/opt/cantaloupe_config/cantaloupe.properties -Dfcrepo.modeshape.configuration=file:///opt/fcrepo/config/repository.json -Dfcrepo.home=/opt/fcrepo/data -Dfcrepo.spring.configuration=file:///opt/fcrepo/config/fcrepo-config.xml -Dcom.bigdata.rdf.sail.webapp.ConfigParams.propertyFile=/opt/blazegraph/conf/RWStore.properties -server -Xmx1500m -Xms1000m"

### Restarting Tomcat

Finally, restart Tomcat to pick up the changes we’ve made.

```bash
sudo systemctl restart tomcat
```

### Installing Blazegraph Namespaces and Inference

The two other files we created, `blazegraph.properties` and `inference.nt`, contain information that Blazegraph requires in order to establish and correctly use the datasets Islandora will send to it. First, we need to create a dataset - contained in `blazegraph.properties` - and then we need to inform that dataset of the inference set we have contained in `inference.nt`.

```bash
curl -X POST -H "Content-Type: text/plain" --data-binary @/opt/blazegraph/conf/blazegraph.properties http://localhost:8080/blazegraph/namespace
curl -X POST -H "Content-Type: text/plain" --data-binary @/opt/blazegraph/conf/inference.nt http://localhost:8080/blazegraph/namespace/islandora/sparql
```
