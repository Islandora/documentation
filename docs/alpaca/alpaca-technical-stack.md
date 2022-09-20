# Alpaca Technical Stack
As of version 2.0.0, Alpaca contains serveral tools bundled into a single runnable [jar](https://en.wikipedia.org/wiki/JAR_(file_format)) file. The different tools can be enabled/disabled depending on the configuration you define.

## [Gradle](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html)
Gradle is used by Alpaca as a build and package management tool. It is similar to [Maven](https://maven.apache.org/).

## [Apache Camel](http://camel.apache.org/book-getting-started.html)
Apache Camel is an integration framework that aids in implementing integration patterns.

## [Apache ActiveMQ](http://activemq.apache.org/getting-started.html)
Apache ActiveMQ is a JMS compliant Messaging Queue. Messaging client can make use of JMS to send messages.

### Installing ActiveMQ
Installing ActiveMQ is relatively easy. Download the latest stable release [here](http://activemq.apache.org/download.html). Go to the `activemq_install_dir/bin`. Start the ActiveMQ by using the activemq script or batch file and start command.

```
$ cd activemq_install_dir/bin
$ ./activemq start
```

When ActiveMQ gets started, go to http://localhost:8161/admin/. You can login using admin:admin.

Note that ActiveMQ in Islandora playbook does not have a UI.

## References
* [ActiveMQ Introduction](http://tech.lalitbhatt.net/2014/08/activemq-introduction.html)
