# Installing Karaf and Alpaca

## In this section, we will install:

- [Apache ActiveMQ](https://activemq.apache.org/), a messaging server that will be used to handle communication between Alpaca and other components
- [Apache Karaf](https://karaf.apache.org/), the Java application runtime that Alpaca will be deployed in
- [Islandora/Alpaca](https://github.com/Islandora/Alpaca), a suite of Java middleware applications that will handle communication between various components of Islandora.

## ActiveMQ 5

### Installing ActiveMQ

In our case, the default installation method for ActiveMQ via `apt-get` will suffice.

```bash
sudo apt-get -y install activemq
```

This will give us:

- A base configuration at `/var/lib/activemq/conf`
- A data storage directory at `/var/lib/activemq/data`
- The base ActiveMQ installation at `/usr/share/activemq`
- An `activemq` service that will be run on boot
- A user, `activemq`, who will be in charge of the ActiveMQ service

Take note of the version of ActiveMQ we're going to be installing. It needs to match a Karaf blueprint we'll create later. Check the version with

```bash
sudo apt-cache policy activemq
```

Write down the version listed under `Installed: `.

## Karaf 4

## Creating a Karaf User

Karaf, as well as its processes and service, will be owned by a user in charge of ensuring this portion of the stack is segregated and that the service is running.

```bash
sudo addgroup karaf
sudo adduser karaf --ingroup karaf --home /opt/karaf --shell /usr/bin
```

As always, you will be prompted for a password, which you should create at this time. All other options can be left blank.

### Downloading and Placing Karaf

Since there’s no `apt-get` installer for Karaf, we’re going to manually download and install it directly from its binary installer.

```bash
cd /opt
sudo wget -O karaf.tar.gz KARAF_TARBALL_LINK
sudo tar -xzvf karaf.tar.gz
sudo chown -R karaf:karaf KARAF_DIRECTORY
sudo mv KARAF_DIRECTORY/* /opt/karaf
```
- `KARAF_TARBALL_LINK`: It’s recommended to get the most recent version of Karaf 4.2.x. This will depend on the current version of Karaf, which can be found on the [Karaf downloads page](https://karaf.apache.org/download.html) under “Karaf Runtime”. Like Solr, you can’t directly `wget` these links, but clicking on the `.tar.gz` link for the binary distribution will bring you to a list of mirrors, as well as provide you with a recommended mirror you can use here.
- `KARAF_DIRECTORY`: This will depend on the exact version being used, but will likely be `/opt/apache-karaf-VERSION`, where `VERSION` is the current Karaf version number.

### Configuring Karaf Logging

We’re going to apply some basic logging to our Karaf installation that should suffice for an example. In a production installation, you may want to play around with some of these values for more personally useful logging.

```bash
sudo mkdir /var/log/karaf
sudo chown karaf:karaf /var/log/karaf
```

`/opt/karaf/etc/org.pos4j.pax.logging.cfg | karaf:karaf/644`
```
# Root logger
log4j.rootLogger=INFO, out, osgi:*
log4j.throwableRenderer=org.apache.log4j.OsgiThrowableRenderer

# File appender
log4j.appender.out=org.apache.log4j.RollingFileAppender
log4j.appender.out.layout=org.apache.log4j.PatternLayout
log4j.appender.out.layout.ConversionPattern=%d{ISO8601} | %-5.5p | %-16.16t | %-32.32c{1} | %X{bundle.id} - %X{bundle.name    } - %X{bundle.version} | %m%n
log4j.appender.out.file=/var/log/karaf/karaf.log
log4j.appender.out.append=true
log4j.appender.out.maxFileSize=1MB
log4j.appender.out.maxBackupIndex=10

# Camel Logger
log4j.appender.camel=org.apache.log4j.RollingFileAppender
log4j.appender.camel.layout=org.apache.log4j.PatternLayout
log4j.appender.camel.layout.ConversionPattern=%d{ISO8601} | %-5.5p | %-16.16t | %-32.32c{1} | %X{bundle.id} - %X{bundle.na    me} - %X{bundle.version} | %m%n
log4j.appender.camel.file=/var/log/karaf/camel.log
log4j.appender.camel.append=false
log4j.appender.camel.maxFileSize=1MB
log4j.appender.camel.maxBackupIndex=10

log4j.logger.org.apache.camel=INFO, camel

# Islandora Logger
log4j.appender.islandora=org.apache.log4j.RollingFileAppender
log4j.appender.islandora.layout=org.apache.log4j.PatternLayout
log4j.appender.islandora.layout.ConversionPattern=%d{ISO8601} | %-5.5p | %-16.16t | %-32.32c{1} | %X{bundle.id} - %X{bundl    e.name} - %X{bundle.version} | %m%n
log4j.appender.islandora.file=/var/log/karaf/islandora.log
log4j.appender.islandora.append=false
log4j.appender.islandora.maxFileSize=1MB
log4j.appender.islandora.maxBackupIndex=10

log4j.logger.ca.islandora.camel=INFO, islandora
```

### Creating a `setenv.sh` Script for Karaf

Similar to Tomcat, our Karaf service is going to rely on a `setenv` shell script to determine environment variables Karaf needs in place when running. For now, this will simply be the path to `JAVA_HOME`, but this also accepts many other parameters you can find in the default `setenv` script.

`/opt/karaf/bin/setenv | karaf:karaf/755`
```
#!/bin/sh
export JAVA_HOME="PATH_TO_JAVA_HOME"
```
- `PATH_TO_JAVA_HOME`: This will be the same `JAVA_HOME` we used when installing Tomcat , and can be found using the same method (i.e., still `/usr/lib/jvm/java-11-openjdk-amd64` if that's what it was before).

### Initializing Karaf

We’re going to start Karaf, then run the installer to put our configurations in place and generate a Karaf service. Once these are installed, we’re going to stop Karaf, as from there on out its start/stop management should be handled via that service.

First we need to enable the default Karaf user in `/opt/karaf/etc/users.properties`:

**Before**:
> 32 | # karaf = karaf,\_g\_:admingroup

> 33 | # \_g\_\\:admingroup = group,admin,manager,viewer,systembundles,ssh

**After**:
> 32 | karaf = karaf,\_g\_:admingroup

> 33 | \_g\_\\:admingroup = group,admin,manager,viewer,systembundles,ssh

Save the file and close it, then:

```bash
sudo -u karaf /opt/karaf/bin/start
# You may want to wait a bit for Karaf to start.
# If you're not sure whether or not it's running, you can always run:
# ps aux | grep karaf
# to see if the server is up and running.
/opt/karaf/bin/client feature:install wrapper
/opt/karaf/bin/client wrapper:install
/opt/karaf/bin/stop
```

### Creating and Starting the Karaf Service

Installing the Karaf wrapper generates several service files that can be used on different types of systems. For Debian and Ubuntu installation we want to enable the `karaf.service` service so that Karaf is properly started on boot.

```bash
sudo systemctl enable /opt/karaf/bin/karaf.service
sudo systemctl start karaf
```

We can check if the service started correctly with:

```bash
sudo systemctl status karaf
```

Press Q to close the status.

## Alpaca 1.0.x

### Adding the Required Karaf Repositories

Karaf features can be installed from several different types of sources, but the fastest and easiest way to do so is from existing repository URLs that we can just plug into Karaf to provide us feature lists prepared and ready for installation. Like most interactions with Karaf, we can add these repositories using its built-in `client`.

!!! notice
    These repositories are updated consistently, and their updates include revised dependency lists. Commonly, when repositories are out of date or otherwise mismatched, feature installation can result in an `Unable to resolve root: missing requirement` error; for this reason, this guide recommends using recently-updated versions of these repositories. That being said, if such errors occur despite installing the latest versions of these features, the maintainer of the features repository should be informed.

For the Karaf features we’re going to install, we need a few different repositories to be added to the list:

```bash
/opt/karaf/bin/client repo-add mvn:org.apache.activemq/activemq-karaf/ACTIVEMQ_KARAF_VERSION/xml/features
/opt/karaf/bin/client repo-add mvn:org.apache.camel.karaf/apache-camel/APACHE_CAMEL_VERSION/xml/features
/opt/karaf/bin/client repo-add mvn:ca.islandora.alpaca/islandora-karaf/ISLANDORA_KARAF_VERSION/xml/features
# XXX: This shouldn't be strictly necessary, but appears to be a missing
# upstream dependency for some fcrepo features.
/opt/karaf/bin/client repo-add mvn:org.apache.jena/jena-osgi-features/JENA_OSGI_VERSION/xml/features
```
- `ACTIVEMQ_KARAF_VERSION`: The version of ActiveMQ we wrote down at the beginning of this chapter when installing ActiveMQ via `apt-get`
- `APACHE_CAMEL_VERSION`: The latest version of Apache Camel 2.x.x; you can find this listed at the [apache-camel repository page](https://mvnrepository.com/artifact/org.apache.camel.karaf/apache-camel) (e.g., 2.25.4 at the time of writing)
- `ISLANDORA_KARAF_VERSION`: The latest version of Islandora Karaf 1.x; you can find this listed at the [islandora-karaf repository page](https://mvnrepository.com/artifact/ca.islandora.alpaca/islandora-karaf) (e.g., 1.0.5 at the time of writing)
- `JENA_OSGI_VERSION`: The latest version of the Apache Jena 3.x OSGi features; you can find this listed at the [jena-osgi-features repository page](https://mvnrepository.com/artifact/org.apache.jena/jena-osgi-features) (e.g., 3.17.0 at the time of writing)

### Configuring Karaf Features

Our installed Karaf features require configuration files to know exactly where to route things coming and going from them.

`/opt/karaf/etc/ca.islandora.alpaca.http.client.cfg | karaf:karaf/644`
```
token.value=ISLANDORA_SYN_TOKEN
```
- `ISLANDORA_SYN_TOKEN`: This should be the same token that was established during the installation of Syn in your `syn-settings.xml` file

`/opt/karaf/etc/org.fcrepo.camel.indexing.triplestore.cfg | karaf:karaf/644`
```
input.stream=activemq:topic:fedora
triplestore.reindex.stream=activemq:queue:triplestore.reindex
triplestore.baseUrl=http://localhost:8080/blazegraph/namespace/islandora/sparql
```

`/opt/karaf/etc/ca.islandora.alpaca.indexing.triplestore.cfg | karaf:karaf/644`
```
error.maxRedeliveries=10
index.stream=activemq:queue:islandora-indexing-triplestore-index
delete.stream=activemq:queue:islandora-indexing-triplestore-delete
triplestore.baseUrl=http://localhost:8080/blazegraph/namespace/islandora/sparql
```

`/opt/karaf/etc/ca.islandora.alpaca.indexing.fcrepo.cfg | karaf:karaf/644`
```
error.maxRedeliveries=5
node.stream=activemq:queue:islandora-indexing-fcrepo-content
node.delete.stream=activemq:queue:islandora-indexing-fcrepo-delete
media.stream=activemq:queue:islandora-indexing-fcrepo-media
file.stream=activemq:queue:islandora-indexing-fcrepo-file
file.delete.stream=activemq:queue:islandora-indexing-fcrepo-file-delete
milliner.baseUrl=http://localhost/milliner
```

### Blueprinting Karaf Derivative Connectors

For those services in Crayfish we have set up to provide derivatives to Islandora resources, we need connector blueprints to tell the derivative connector how to route incoming requests, run conversions, and return outgoing derivatives.

Our blueprints are going to look largely similar between services, with only a few properties changing between them. Largely, these mainly just need to match the ActiveMQ queues we established in the previous configuration, and route to the correct Crayfish service.

`/opt/karaf/deploy/ca.islandora.alpaca.connector.ocr.blueprint.xml | karaf:karaf/644`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<blueprint xmlns="http://www.osgi.org/xmlns/blueprint/v1.0.0"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:cm="http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0"
       xsi:schemaLocation="
       http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0 http://aries.apache.org/schemas/blueprint-cm/blueprint-cm-1.1.0.xsd
       http://www.osgi.org/xmlns/blueprint/v1.0.0 http://www.osgi.org/xmlns/blueprint/v1.0.0/blueprint.xsd
       http://camel.apache.org/schema/blueprint http://camel.apache.org/schema/blueprint/camel-blueprint.xsd">

  <cm:property-placeholder id="properties" persistent-id="ca.islandora.alpaca.connector.ocr" update-strategy="reload" >
    <cm:default-properties>
      <cm:property name="error.maxRedeliveries" value="5"/>
      <cm:property name="in.stream" value="activemq:queue:islandora-connector-ocr"/>
      <cm:property name="derivative.service.url" value="http://localhost/hypercube"/>
    </cm:default-properties>
  </cm:property-placeholder>

  <reference id="broker" interface="org.apache.camel.Component" filter="(osgi.jndi.service.name=fcrepo/Broker)"/>

  <bean id="http" class="org.apache.camel.component.http4.HttpComponent"/>
  <bean id="https" class="org.apache.camel.component.http4.HttpComponent"/>

  <camelContext id="IslandoraConnectorOCR" xmlns="http://camel.apache.org/schema/blueprint">
    <package>ca.islandora.alpaca.connector.derivative</package>
  </camelContext>
  
</blueprint>
```

`/opt/karaf/deploy/ca.islandora.alpaca.connector.houdini.blueprint.xml | karaf:karaf/644`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<blueprint xmlns="http://www.osgi.org/xmlns/blueprint/v1.0.0"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:cm="http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0"
       xsi:schemaLocation="
       http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0 http://aries.apache.org/schemas/blueprint-cm/blueprint-cm-1.1.0.xsd
       http://www.osgi.org/xmlns/blueprint/v1.0.0 http://www.osgi.org/xmlns/blueprint/v1.0.0/blueprint.xsd
       http://camel.apache.org/schema/blueprint http://camel.apache.org/schema/blueprint/camel-blueprint.xsd">

  <cm:property-placeholder id="properties" persistent-id="ca.islandora.alpaca.connector.houdini" update-strategy="reload" >
    <cm:default-properties>
      <cm:property name="error.maxRedeliveries" value="5"/>
      <cm:property name="in.stream" value="activemq:queue:islandora-connector-houdini"/>
      <cm:property name="derivative.service.url" value="http://localhost/houdini/convert"/>
    </cm:default-properties>
  </cm:property-placeholder>

  <reference id="broker" interface="org.apache.camel.Component" filter="(osgi.jndi.service.name=fcrepo/Broker)"/>

  <bean id="http" class="org.apache.camel.component.http4.HttpComponent"/>
  <bean id="https" class="org.apache.camel.component.http4.HttpComponent"/>

  <camelContext id="IslandoraConnectorHoudini" xmlns="http://camel.apache.org/schema/blueprint">
    <package>ca.islandora.alpaca.connector.derivative</package>
  </camelContext>
  
</blueprint>
```

`/opt/karaf/deploy/ca.islandora.alpaca.connector.homarus.blueprint.xml | karaf:karaf/644`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<blueprint xmlns="http://www.osgi.org/xmlns/blueprint/v1.0.0"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:cm="http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0"
       xsi:schemaLocation="
       http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0 http://aries.apache.org/schemas/blueprint-cm/blueprint-cm-1.1.0.xsd
       http://www.osgi.org/xmlns/blueprint/v1.0.0 http://www.osgi.org/xmlns/blueprint/v1.0.0/blueprint.xsd
       http://camel.apache.org/schema/blueprint http://camel.apache.org/schema/blueprint/camel-blueprint.xsd">

  <cm:property-placeholder id="properties" persistent-id="ca.islandora.alpaca.connector.homarus" update-strategy="reload" >
    <cm:default-properties>
      <cm:property name="error.maxRedeliveries" value="5"/>
      <cm:property name="in.stream" value="activemq:queue:islandora-connector-homarus"/>
      <cm:property name="derivative.service.url" value="http://localhost/homarus/convert"/>
    </cm:default-properties>
  </cm:property-placeholder>

  <reference id="broker" interface="org.apache.camel.Component" filter="(osgi.jndi.service.name=fcrepo/Broker)"/>

  <bean id="http" class="org.apache.camel.component.http4.HttpComponent"/>
  <bean id="https" class="org.apache.camel.component.http4.HttpComponent"/>

  <camelContext id="IslandoraConnectorHomarus" xmlns="http://camel.apache.org/schema/blueprint">
    <package>ca.islandora.alpaca.connector.derivative</package>
  </camelContext>
  
</blueprint>
```

`/opt/karaf/deploy/ca.islandora.alpaca.connector.fits.blueprint.xml | karaf:karaf/644`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<blueprint xmlns="http://www.osgi.org/xmlns/blueprint/v1.0.0"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:cm="http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0"
       xsi:schemaLocation="
       http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.1.0 http://aries.apache.org/schemas/blueprint-cm/blueprint-cm-1.1.0.xsd
       http://www.osgi.org/xmlns/blueprint/v1.0.0 http://www.osgi.org/xmlns/blueprint/v1.0.0/blueprint.xsd
       http://camel.apache.org/schema/blueprint http://camel.apache.org/schema/blueprint/camel-blueprint.xsd">

  <cm:property-placeholder id="properties" persistent-id="ca.islandora.alpaca.connector.fits" update-strategy="reload" >
    <cm:default-properties>
      <cm:property name="error.maxRedeliveries" value="5"/>
      <cm:property name="in.stream" value="activemq:queue:islandora-connector-fits"/>
      <cm:property name="derivative.service.url" value="http://localhost/crayfits"/>
    </cm:default-properties>
  </cm:property-placeholder>

  <reference id="broker" interface="org.apache.camel.Component" filter="(osgi.jndi.service.name=fcrepo/Broker)"/>

  <bean id="http" class="org.apache.camel.component.http4.HttpComponent"/>
  <bean id="https" class="org.apache.camel.component.http4.HttpComponent"/>

  <camelContext id="IslandoraConnectorfits" xmlns="http://camel.apache.org/schema/blueprint">
    <package>ca.islandora.alpaca.connector.derivative</package>
  </camelContext>
  
</blueprint>
```

### Installing the Required Karaf Features

Before we can configure the features we’re going to use, they need to be installed. Some of these installations may take some time.

```bash
/opt/karaf/bin/client feature:install camel-blueprint
/opt/karaf/bin/client feature:install activemq-blueprint
/opt/karaf/bin/client feature:install fcrepo-service-activemq
# This again should not be strictly necessary, since this isn't the triplestore
# we're using, but is being included here to resolve the aforementioned
# missing link in the dependency chain.
/opt/karaf/bin/client feature:install jena
/opt/karaf/bin/client feature:install fcrepo-camel
/opt/karaf/bin/client feature:install fcrepo-indexing-triplestore
/opt/karaf/bin/client feature:install islandora-http-client
/opt/karaf/bin/client feature:install islandora-indexing-triplestore
/opt/karaf/bin/client feature:install islandora-indexing-fcrepo
/opt/karaf/bin/client feature:install islandora-connector-derivative
```

### Verifying Karaf Components are Running (Optional But Recommended)

At this point, Karaf components should be up and running, but it's a good idea to double-check that this is the case. We can do this from within the Karaf client by taking a look at its component list.

```bash
# Until this point, we've been running Karaf commands from outside; we can hop
# into the client, however, and run commands from directly within.
/opt/karaf/bin/client
# This takes us into the Karaf client so we can run commands.
la | grep islandora
la | grep fcrepo
# It may be a good idea to use this to look up to the other components we
# installed.
logout
```

For the above `la | grep` commands, components that are running should be listed as `Active`.
