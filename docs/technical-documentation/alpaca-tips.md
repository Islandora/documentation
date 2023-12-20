# Alpaca Tips

[Alpaca](https://github.com/Islandora/Alpaca) is event-driven middleware based on [Apache Camel](https://camel.apache.org/) for Islandora

Currently, Alpaca ships with four event-driven components

- [islandora-connector-derivative](#islandora-connector-derivative)
- [islandora-http-client](#islandora-http-client)
- [islandora-indexing-fcrepo](#islandora-indexing-fcrepo)
- [islandora-indexing-triplestore](#islandora-indexing-triplestore)

## islandora-connector-derivative
This service receives requests from Drupal when it wants to create derivatives and passes that request along to a microservice in [Crayfish](https://github.com/Islandora/Crayfish). When it receives the derivative file back from the microservice, it passes the file back to Drupal.

## islandora-http-client
This service overrides the default http client with Islandora specific configuration.

## islandora-indexing-fcrepo
This service receives requests from Drupal in response to write operations on entities. These requests are passed along to [Milliner](https://github.com/Islandora/Crayfish/tree/dev/Milliner) microservice in [Crayfish](https://github.com/Islandora/Crayfish) to convert Drupal entities into Fedora resources and communicate with Fedora (via [Chullo](https://github.com/Islandora/chullo)).

## islandora-indexing-triplestore
This service receives requests from Drupal on indexing and deleting in order to persist/delete content in the triplestore.


## Steps for developing with Alpaca
Alpaca now runs as a single executable jar which can enable none, some or all of the available services.

To develop your own module, start by cloning the Alpaca code base. 

Then create a new directory (for example `my-new-module`) along side the `islandora-indexing-fcrepo`, `islandora-indexing-triplestore` directories

Add your new directory to the `settings.gradle` file, following the pattern of the others.
```shell
  include ':islandora-support'
  include ':islandora-indexing-triplestore'
  include ':islandora-indexing-fcrepo'
  include ':islandora-connector-derivative'
  include ':islandora-http-client'
  include ':islandora-alpaca-app'
+ include ':my-new-module'

  project(':islandora-alpaca-app').setProjectDir("$rootDir/islandora-alpaca-app" as File)
  project(':islandora-support').setProjectDir("$rootDir/islandora-support" as File)
  project(':islandora-indexing-triplestore').setProjectDir("$rootDir/islandora-indexing-triplestore" as File)
  project(':islandora-indexing-fcrepo').setProjectDir("$rootDir/islandora-indexing-fcrepo" as File)
  project(':islandora-connector-derivative').setProjectDir("$rootDir/islandora-connector-derivative" as File)
  project(':islandora-http-client').setProjectDir("$rootDir/islandora-http-client" as File)
+ project(':my-new-module').setProjectDir("$rootDir/my-new-module" as File)
```

You can explore the `islandora-indexing-fcrepo` module to see the pattern to develop your own module.

This module contains three classes. 

You can ignore the `CommonProcessor` class, that is just some processing that is split out for reusability.

The first class is the `FcrepoIndexer`, this class extends the Apache Camel `RouteBuilder` and requires a `configure` method which defines the processing elements of your workflow. This is the Camel "route".

The second class is the `FcrepoIndexerOptions`, this class extends the Alpaca `PropertyConfig` base class which gets common configuration parameters into your module. It also contains any custom configuration parameters needed for your route. 

Lastly it uses the `@Conditional(FcrepoIndexerOptions.FcrepoIndexerEnabled.class)` to define when this module is enabled. 

`FcrepoIndexerOptions.FcrepoIndexerEnabled.class` refers to the static inner class.

This class is inside of `FcrepoIndexerOptions` and works like this:
```
[1]  static class FcrepoIndexerEnabled extends ConditionOnPropertyTrue { 
[2]    FcrepoIndexerEnabled() {
[3]      super(FcrepoIndexerOptions.FCREPO_INDEXER_ENABLED, false);
[4]    }
[5]  }
```
Line 1 extends the class that will register (enable) this module when a defined property is "TRUE"

Line 2 is the constructor for this static class

Line 3 passes to the parent constructor two things.

1. the property name to check for enabling this module.
2. the default value to use if the property (above) is not found.

So in this case we check for the property `fcrepo.indexer.enabled` and if we don't find it, we pass `false`. So this module is assumed to be "off" unless the property `fcrepo.indexer.enabled=true` is located.

The last thing is to add your new module to the `islandora-alpaca-app` `build.gradle` file as a dependencies, like the existing modules.
i.e.
```
dependencies {
    implementation "info.picocli:picocli:${versions.picocli}"
    implementation "org.apache.camel:camel-spring-javaconfig:${versions.camel}"
    implementation "org.slf4j:slf4j-api:${versions.slf4j}"
    implementation "org.springframework:spring-context:${versions.spring}"
    implementation project(':islandora-support')
    implementation project(':islandora-connector-derivative')
    implementation project(':islandora-indexing-fcrepo')
    implementation project(':islandora-indexing-triplestore')
+   implementation project(':my-new-module')

    runtimeOnly "ch.qos.logback:logback-classic:${versions.logback}"

}
```

Finally from the top-level directory of Alpaca execute
```
./gradlew clean build shadowJar
```

This tells Gradle to clean the modules, then build the modules and finally create a single jar with all needed code (the shadow jar).

The final executable jar is:
```
<alpaca directory>/islandora-alpaca-app/build/libs/islandora-alpaca-<version>-all.jar
```
