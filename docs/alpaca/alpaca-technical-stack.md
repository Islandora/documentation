# Alpaca Technical Stack
Alpaca contains several OSGI modules or bundles. They are grouped together as features and deployed to Karaf container.

## [OSGi](https://www.osgi.org/developer/architecture/)
OSGi is a specification to develop and deploy modular Java applications. It allows for dynamic deployment (hot deployment) and dependency management. 

The base unit of resources is called a bundle. Bundle is similar to a jar file, with additional information to be processed as an OSGi component. Several bundles can be grouped together into a Feature and installed together. OSGi bundles can be run on containers implementing the OSGi specification. Apache Karaf is one such container

## [Karaf](https://karaf.apache.org/manual/latest/overview.html)
Apache Karaf is a container that can be used to deploy an array of applications such servlets, apache camel components, jars etc. 

### Karaf - Bundle - Hello World 
Download a bundle to the claw vagrant (ex to /home directory). An example hello world bundle is [here](https://github.com/moghaddam/developmentor/blob/master/helloworld/target/helloworld-1.0.0.jar). 

In Islandora CLAW vagrant, you can login to Karaf using ssh. The password is karaf. You can also use the client here: /opt/karaf/bin/client. Karaf client allows the use of linux commands such as ```grep``` in addition to Karaf commands.  

```
ssh -p 8101 karaf@localhost
```

Install the bundle:
```
karaf@root()> bundle
karaf@root(bundle)> install file:///home/helloworld-1.0.0.jar
Bundle ID: 242
```

The installation will return a bundle id. You can issue the list command to verify that bundle is on the list. Initially it will have *Installed* status.

As per OSGi specification, before a bundle can be started, it has to be Resolved. To resolve, issue the following command.  
```
karaf@root(bundle)> resolve 242
```

You can start and stop the bundle as below.
```
karaf@root(bundle)> start 242
Hello World!
karaf@root(bundle)> stop 242
Goodbye World!
karaf@root(bundle)> 
```

### Karaf - Features - Hello World 
Karaf Features allows for bundles to be grouped, managed and deployed together. Features can be nested as well. Feature files of frameworks such as Apache Camel or Apache ActiveMQ can be used to deploy those services.  

A simple Features file is as below.  

```
<features>
  <feature name='greeter_server' version='1.0'>
    <bundle>file:///home/helloworld-1.0.0.jar</bundle>
    <bundle>file:///home/helloworld2-1.0.0.jar</bundle>
  </feature>
</features>
```

You can add the features to Karaf as below:
```
karaf@root()> feature:repo-add file:///home/features.xml
```
You can install the feature's bundles as below.  

```
karaf@root(feature)> install greeter_server
Hello World 2 !
Hello World!
```

Uninstalling the feature.
```
karaf@root(feature)> uninstall greeter_server
Goodbye World 2 !
Goodbye World!

```

## [Gradle](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html)
Gradle is used by Alpaca as a build and package management tool. It is similar to [Maven](https://maven.apache.org/).  


## [Apache Camel](http://camel.apache.org/book-getting-started.html)
Apache Camel is an integration framework that aids in implementing integration patterns.  

### Alpaca - Apache Camel HelloWorld
Maven has camel-archetype-blueprint which can be used to create apache-camel OSGi bundle project structure.  

#### Creating a project / bundle under Alpaca
```
mvn archetype:generate -DarchetypeGroupId=org.apache.camel.archetypes -DarchetypeArtifactId=camel-archetype-blueprint -DarchetypeVersion=2.9.0 -DarchetypeRepository=https://repository.apache.org/content/groups/snapshots-g
```

However, it is easier to copy the structure of an existing project such as islandora-connector-broadcast[](https://github.com/Islandora-CLAW/Alpaca/tree/master/islandora-connector-broadcast).

```
cp -R islandora-connector-broadcast/ islandora-connector-helloworld

```
Open the project in your IDE. In Eclipse, you can go to File | Open Projects from File System and navigate the new folder.  

We need to rename the configuration, java and build files to reflect the helloworld project.  

* ```src/main/cfg/ca.islandora.alpaca.connector.helloworld.cfg```
* ```src/main/java/ca/islandora/alpaca/connector/helloworld```
* ```src/main/java/ca/islandora/alpaca/connector/helloworld/HelloworldRouter.java```

* Also change the content of the above file to log Hello World

```
package ca.islandora.alpaca.connector.helloworld;

import static org.apache.camel.LoggingLevel.INFO;

import org.apache.camel.builder.RouteBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class HelloworldRouter extends RouteBuilder {

    private static final Logger LOGGER = LoggerFactory.getLogger(HelloworldRouter.class);

    /**
     * Configure the message route workflow.
     */
    public void configure() throws Exception {

        // Hello World.
    	from("timer:foo?period=5000").log(INFO, LOGGER, "Hello World");
    }
}

```

* ```src/main/resource/OSGI-INF/blueprint/blueprint.xml```

Change the content of this file to reflect Helloworld package.  

* ```src/build.gradle```

```
apply plugin: 'osgi'

description = 'Islandora CLAW HelloWorld'

dependencies {
    compile group: 'org.apache.camel', name: 'camel-core', version: camelVersion
    compile group: 'org.apache.camel', name: 'camel-blueprint', version: camelVersion
    compile group: 'org.apache.activemq', name: 'activemq-camel', version: activemqVersion
    compile group: 'org.slf4j', name: 'slf4j-api', version: slf4jVersion
    testCompile group: 'org.apache.camel', name: 'camel-test-blueprint', version: camelVersion
}

jar {
    manifest {
        description project.description
        docURL project.docURL
        vendor project.vendor
        license project.license

        instruction 'Import-Package', 'org.apache.activemq.camel.component,' +
                            "org.apache.camel;version=\"${camelVersionRange}\"," +
                            defaultOsgiImports
        instruction 'Export-Package', 'ca.islandora.alpaca.connector.helloworld'
    }
}

artifacts {
    archives (file('build/cfg/main/ca.islandora.alpaca.connector.helloworld.cfg')) {
        classifier 'configuration'
        type 'cfg'
    }
}
```

#### Building the bundle
To build this project, we have to update the Alpaca build settings. Include and add the project in ```Alpaca/settings.gradle```.
```
include ':islandora-connector-helloworld'
project(':islandora-connector-helloworld').projectDir = "$rootDir/islandora-connector-helloworld" as File
```

Normally, we would also have to update the ```karaf/src/main/resources/features.xml``` file to include this bundle. However, we will be deploying the bundle directly into Karaf's Hot deployment directory. Thus, not needed at this time.

To build it from command line in Linux.
```
ubuntu:~/workspace/Alpaca$ sudo gradle w build
```

#### Deploying the bundle
* Upload the bundle to the Apache Karaf deploy directory: /opt/apache-karaf-4.0.5/deploy
* Login to Karaf.  You can also use the Karaf client here: /opt/karaf/bin/client.
```
ssh -p 8101 karaf@localhost
```

* Verify that bundle is deployed
```
bundle:list
```

* See the log to confirm that the bundle is working.
```
log:tail
```

You should see a message like below:
```
2017-02-24 21:30:17,973 | INFO  | 12 - timer://foo | HelloworldRouter                 | 186 - ca.islandora.alpaca.islandora-connector-helloworld - 0.2.1.SNAPSHOT | Hello World
```

## [Apache ActiveMQ](http://activemq.apache.org/getting-started.html)
Apache ActiveMQ is a JMS compliant Messaging Queue. Messaging client can make use of JMS to send messages.

### Installing ActiveMQ
Installing ActiveMQ is relatively easy. Download the latest stable release [here](http://activemq.apache.org/download.html). Go to the activemq_install_dir/bin. Start the ActiveMQ by using the activemq script or batch file and start command.

```
ubuntu:/apps/activemq_install_dir/bin$./activemq start
```

When ActiveMQ gets started, go to http://localhost:8161/admin/. You can login using admin:admin.

Note that ActiveMQ in CLAW vagrant does not have a UI.  

## References
* [OSGI for Beginners](http://www.theserverside.com/news/1363825/OSGi-for-Beginners)
* [Playing with Apache Karaf Console](http://www.javaindeed.com/playing-with-apache-karaf-console/)
* [Karaf Quick Start](https://karaf.apache.org/manual/latest/quick-start.html)
* [ActiveMQ Introduction](http://tech.lalitbhatt.net/2014/08/activemq-introduction.html)
