# The Islandora Camel Component

## Introduction
From within a Camel route, if you ever want to execute a command you’ve writtin in PHP, all you need to do is use the Islandora Camel Component.  The Islandora Camel Component is a very small Camel component that mimics/wraps/extends bits of camel-exec in order to streamline the use of Camel commands.  It also is responsible for evaluating the exit status of the command and bubbling up an exceptions that may have been thrown.

## Configuration
Before using the Islandora component, you need to configure it in the same Blueprint xml file that defines your Camel context.  Typically, this is the `src/main/resources/OSGI-INF/blueprint/blueprint.xml` for your particular Camel project.  The component accepts a single value: the absolute path to the `camel/commands/bin` directory of your project.  It is best to set this value in the `islandora.cfg` as `islandora.php.workingDir`.

Assuming you're using the `islandora.cfg` file deployed to your karaf's `etc` directory, your Blueprint file will look something like this:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<blueprint xmlns="http://www.osgi.org/xmlns/blueprint/v1.0.0"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:cm="http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.0.0"
       xsi:schemaLocation="
       http://www.osgi.org/xmlns/blueprint/v1.0.0 http://www.osgi.org/xmlns/blueprint/v1.0.0/blueprint.xsd
       http://camel.apache.org/schema/blueprint http://camel.apache.org/schema/blueprint/camel-blueprint.xsd">

  <!-- Load Configuration File -->
  <cm:property-placeholder persistent-id="islandora"/>

  <!-- Islandora Component Configuration -->
  <bean id="islandora" class="ca.islandora.camel.component.IslandoraComponent">
    <argument value="${islandora.php.workingDir}"/>
  </bean>

  <camelContext id="your_context_id" xmlns="http://camel.apache.org/schema/blueprint">
    <-- Your routes go here... -->
  </camelContext>

</blueprint>
```

## Usage
Once configured, you can use the component to execute any IslandoraCommand in PHP within your camel routes.  General usage is:
```xml
<to uri="islandora:namespace:command"/>
```
You can see a list of all available namespaces and commands at any time by going to the `camel/commands/bin` directory of your project and running `php islandora.php`.
```bash
vagrant@islandora:~$ cd ~/islandora/camel/commands/bin
vagrant@islandora:~/islandora/camel/commands/bin$ php islandora.php
Islandora Command Tool version 0.0.0-SNAPSHOT

Usage:
  command [options] [arguments]

Options:
  -h, --help            Display this help message
  -q, --quiet           Do not output any message
  -V, --version         Display this application version
      --ansi            Force ANSI output
      --no-ansi         Disable ANSI output
  -n, --no-interaction  Do not ask any interactive question
  -v|vv|vvv, --verbose  Increase the verbosity of messages: 1 for normal output, 2 for more verbose output and 3 for debug

Available commands:
  help                            Displays help for a command
  list                            Lists commands
 collectionService
  collectionService:nodeToSparql  Converts Drupal node JSON for a collection to a SPARQL Update query
 greeter
  greeter:hello                   Says hello!
  greeter:helloUsingJson          Says hello!  Accepts JSON data in the form {"name" : "your_name"}.
 rdf
  rdf:createNode                  Creates a Drupal node from Fedora RDF.
  rdf:extractContentType          Extracts a Drupal content type from Fedora RDF.
  rdf:updateNode                  Updates a Drupal node from Fedora RDF.
```
For example, to run the greeter example from the tutorial in the Commands documentation:
```xml
<to uri="islandora:greeter:hello"/>
```
Here's a small Blueprint file that will execute the `greeter:hello` command every 5 seconds and log the output.
```xml
<?xml version="1.0" encoding="UTF-8"?>
<blueprint xmlns="http://www.osgi.org/xmlns/blueprint/v1.0.0"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:cm="http://aries.apache.org/blueprint/xmlns/blueprint-cm/v1.0.0"
       xsi:schemaLocation="
       http://www.osgi.org/xmlns/blueprint/v1.0.0 http://www.osgi.org/xmlns/blueprint/v1.0.0/blueprint.xsd
       http://camel.apache.org/schema/blueprint http://camel.apache.org/schema/blueprint/camel-blueprint.xsd">

  <!-- Load Configuration File -->
  <cm:property-placeholder persistent-id="islandora"/>

  <!-- Islandora Component Configuration -->
  <bean id="islandora" class="ca.islandora.camel.component.IslandoraComponent">
    <argument value="${islandora.php.workingDir}"/>
  </bean>

  <camelContext id="your_context_id" xmlns="http://camel.apache.org/schema/blueprint">
    <route>
      <from uri="timer:foo?period=5000"/>
        <setBody><constant>World</constant></setBody>
        <to uri="islandora:greeter:hello"/>
        <log message="${body}"/>
    </route>
  </camelContext>

</blueprint>
```
You can deploy this file to your karaf's `deploy` directory and watch it go.  In your logs, you should see something like this:
```bash
vagrant@islandora:~$ /opt/karaf/bin/client
Logging in as karaf
750 [sshd-SshClient[6107227e]-nio2-thread-1] WARN org.apache.sshd.client.keyverifier.AcceptAllServerKeyVerifier - Server at [/0.0.0.0:8101, RSA, 88:ab:a4:c9:c0:bc:53:bf:bb:f8:b3:8b:80:98:26:7d] presented unverified {} key: {}
        __ __                  ____
       / //_/____ __________ _/ __/
      / ,<  / __ `/ ___/ __ `/ /_
     / /| |/ /_/ / /  / /_/ / __/
    /_/ |_|\__,_/_/   \__,_/_/

  Apache Karaf (3.0.4)

Hit '<tab>' for a list of available commands
and '[cmd] --help' for help on a specific command.
Hit 'system:shutdown' to shutdown Karaf.
Hit '<ctrl-d>' or type 'logout' to disconnect shell from current session.

karaf@root()> log:tail
2015-07-17 18:12:51,292 | INFO  | #7 - timer://foo | IslandoraProducer                | 132 - ca.islandora.camel.component.islandora-camel-component - 0.0.0.SNAPSHOT | Executing ExecCommand [args=[islandora.php, greeter:hello], executable=php, timeout=9223372036854775807, outFile=null, workingDir=/home/vagrant/islandora/camel/commands/bin, useStderrOnEmptyStdout=false]
2015-07-17 18:12:51,681 | INFO  | #7 - timer://foo | IslandoraProducer                | 132 - ca.islandora.camel.component.islandora-camel-component - 0.0.0.SNAPSHOT | The command ExecCommand [args=[islandora.php, greeter:hello], executable=php, timeout=9223372036854775807, outFile=null, workingDir=/home/vagrant/islandora/camel/commands/bin, useStderrOnEmptyStdout=false] had exit value 0
2015-07-17 18:12:51,682 | INFO  | #7 - timer://foo | route53                          | 109 - org.apache.camel.camel-core - 2.15.2 | Hello World!

2015-07-17 18:12:56,292 | INFO  | #7 - timer://foo | IslandoraProducer                | 132 - ca.islandora.camel.component.islandora-camel-component - 0.0.0.SNAPSHOT | Executing ExecCommand [args=[islandora.php, greeter:hello], executable=php, timeout=9223372036854775807, outFile=null, workingDir=/home/vagrant/islandora/camel/commands/bin, useStderrOnEmptyStdout=false]
2015-07-17 18:12:56,676 | INFO  | #7 - timer://foo | IslandoraProducer                | 132 - ca.islandora.camel.component.islandora-camel-component - 0.0.0.SNAPSHOT | The command ExecCommand [args=[islandora.php, greeter:hello], executable=php, timeout=9223372036854775807, outFile=null, workingDir=/home/vagrant/islandora/camel/commands/bin, useStderrOnEmptyStdout=false] had exit value 0
2015-07-17 18:12:56,676 | INFO  | #7 - timer://foo | route53                          | 109 - org.apache.camel.camel-core - 2.15.2 | Hello World!
```

## Errors and Exceptions in PHP
The Islandora Camel component automatically handles checking the return code on the command, and will automatically bubble up any exception from PHP by throwing an IslandoraPHPException that can be handled within your Camel context.  See the section on exception handling in Camel for more information.
