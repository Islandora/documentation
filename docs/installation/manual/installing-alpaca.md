# Installing ActiveMQ and Alpaca

!!! note "Karaf no longer needed"
    You no longer need to install Karaf. We no longer do this, we just deploy the java apps. 

## In this section, we will install:

- [Apache ActiveMQ](https://activemq.apache.org/), a messaging server that will be used to handle communication between Alpaca and other components
- [Islandora/Alpaca](https://github.com/Islandora/Alpaca), Java middleware that handle communication between various components of Islandora.

## Installing ActiveMQ

Some users have been able to install ActiveMQ from the standard package repositories. Others, however, have needed to install it manually.

### Option 1: System Provided Packages

```bash
sudo apt-get -y install activemq
```

This will give us:

- A base configuration at `/var/lib/activemq/conf`
- A data storage directory at `/var/lib/activemq/data`
- The base ActiveMQ installation at `/usr/share/activemq`
- An `activemq` service that will be run on boot
- A user, `activemq`, who will be in charge of the ActiveMQ service

Note the port used by ActiveMQ as this will be added to the JMS setting in the alpaca config below.

```bash
sudo apt-cache policy activemq
```

Write down the version listed under `Installed: `.

### Option 2: Manual Install

Git the latest ActiveMQ 5.x version number from https://archive.apache.org/dist/activemq which will be put in place of `[ACTIVEMQ_VERSION_NUMBER]`.

```bash
cd /opt
sudo wget http://archive.apache.org/dist/activemq/[ACTIVEMQ_VERSION_NUMBER]/apache-activemq-[ACTIVEMQ_VERSION_NUMBER]-bin.tar.gz
sudo tar -xvzf apache-activemq-[ACTIVEMQ_VERSION_NUMBER]-bin.tar.gz
sudo mv apache-activemq-[ACTIVEMQ_VERSION_NUMBER] /opt/activemq
sudo addgroup --quiet --system activemq
sudo adduser --quiet --system --ingroup activemq --no-create-home --disabled-password activemq
sudo chown -R activemq:activemq /opt/activemq
sudo rm -R apache-activemq-[ACTIVEMQ_VERSION_NUMBER]-bin.tar.gz
```

Add ActiveMQ as a service:
`/etc/systemd/system/activemq.service | root:root/644
```
[Unit]
Description=Apache ActiveMQ
After=network.target

[Service]
Type=forking
User=activemq
Group=activemq

ExecStart=/opt/activemq/bin/activemq start
ExecStop=/opt/activemq/bin/activemq stop

[Install]
WantedBy=multi-user.target
```

Update the WebConsolePort host property settings in `/opt/activemq/conf/jetty.xml` from `<property name="host" value="127.0.0.1"/>` to `<property name="host" value="0.0.0.0"/>` so that you can access the dashboard from outside the local machine.

Optionally, change the dashboard user credentials in `/opt/activemq/conf/users.properties`.

*Note*
> Updating the web console port and user properties are potential security holes. It is best to restrict the host setting and create a more secure username/password combination for production.

Set the service to start on machine startup and start it up:
```bash
sudo systemctl daemon-reload
sudo systemctl start activemq
sudo systemctl enable activemq
sudo systemctl status activemq
sudo systemctl restart activemq
sudo apt-cache policy activemq   # note version number
```

The service should now be available at `http://localhost:8161/`


## Installing Alpaca

Install Java 11+ if you haven't already.

Make a directory for Alpaca and download the latest version of Alpaca from the [Maven repository](https://repo1.maven.org/maven2/ca/islandora/alpaca/islandora-alpaca-app). E.g.
```
sudo mkdir /opt/alpaca
cd /opt/alpaca
sudo curl -L https://repo1.maven.org/maven2/ca/islandora/alpaca/islandora-alpaca-app/2.2.0/islandora-alpaca-app-2.2.0-all.jar -o alpaca.jar
```

### Configuration

Alpaca is made up of several services, each of these can be enabled or disabled individually.

Alpaca takes an external file to configure its behaviour.

Look at the [`example.properties`](https://github.com/Islandora/Alpaca/blob/2.x/example.properties) file to see some example settings.

The properties are:

```
# Common options
error.maxRedeliveries=5
```
This defines how many times to retry a message before failing completely.

There are also common ActiveMQ properties to setup the connection.

```
# ActiveMQ options
jms.brokerUrl=tcp://localhost:61616
```

This defines the url to the ActiveMQ broker which you installed earlier.

```
jms.username=
jms.password=
```
This defines the login credentials (if required)

```
jms.connections=10
```
This defines the pool of connections to the ActiveMQ instance.

#### islandora-indexing-fcrepo

This service manages a Drupal node into a corresponding Fedora resource.

It's properties are:

```
# Fcrepo indexer options
fcrepo.indexer.enabled=true
```

This defines whether the Fedora indexer is enabled or not.

```
fcrepo.indexer.node=queue:islandora-indexing-fcrepo-content
fcrepo.indexer.delete=queue:islandora-indexing-fcrepo-delete
fcrepo.indexer.media=queue:islandora-indexing-fcrepo-media
fcrepo.indexer.external=queue:islandora-indexing-fcrepo-file-external
```

These define the various queues to listen on for the indexing/deletion
messages. The part after `queue:` should match your Islandora instance "Actions".

```
fcrepo.indexer.milliner.baseUrl=http://localhost/milliner
```
This defines the location of your Milliner microservice.

```
fcrepo.indexer.concurrent-consumers=-1
fcrepo.indexer.max-concurrent-consumers=-1
```
These define the default number of concurrent consumers and maximum number of concurrent
consumers working off your ActiveMQ instance.
A value of `-1` means no setting is applied.

```
fcrepo.indexer.async-consumer=false
```

This property allows the concurrent consumers to process concurrently; otherwise, the consumers will wait to the previous message has been processed before executing.

#### islandora-indexing-triplestore

This service indexes the Drupal node into the configured triplestore

It's properties are:

```
# Triplestore indexer options
triplestore.indexer.enabled=true
```

This defines whether the Triplestore indexer is enabled or not.

```
triplestore.index.stream=queue:islandora-indexing-triplestore-index
triplestore.delete.stream=queue:islandora-indexing-triplestore-delete
```

These define the various queues to listen on for the indexing/deletion
messages. The part after `queue:` should match your Islandora instance "Actions".

```
triplestore.baseUrl=http://localhost:8080/bigdata/namespace/kb/sparql
```

This defines the location of your triplestore's SPARQL update endpoint.

```
triplestore.indexer.concurrent-consumers=-1
triplestore.indexer.max-concurrent-consumers=-1
```

These define the default number of concurrent consumers and maximum number of concurrent
consumers working off your ActiveMQ instance.
A value of `-1` means no setting is applied.


```
triplestore.indexer.async-consumer=false
```

This property allows the concurrent consumers to process concurrently; otherwise, the consumers will wait to the previous message has been processed before executing.

#### islandora-connector-derivative

This service is used to configure an external microservice. This service will deploy multiple copies of its routes
with different configured inputs and outputs based on properties.

The routes to be configured are defined with the property `derivative.systems.installed` which expects
a comma separated list. Each item in the list defines a new route and must also define 3 additional properties.

```
derivative.<item>.enabled=true
```

This defines if the `item` service is enabled.

```
derivative.<item>.in.stream=queue:islandora-item-connector-<item>
```

This is the input queue for the derivative microservice.
The part after `queue:` should match your Islandora instance "Actions".

```
derivative.<item>.service.url=http://example.org/derivative/convert
```

This is the microservice URL to process the request.

```
derivative.<item>.concurrent-consumers=-1
derivative.<item>.max-concurrent-consumers=-1
```

These define the default number of concurrent consumers and maximum number of concurrent
consumers working off your ActiveMQ instance.
A value of `-1` means no setting is applied.


```
derivative.<item>.async-consumer=false
```

This property allows the concurrent consumers to process concurrently; otherwise, the consumers will wait to the previous message has been processed before executing.

For example, with two services defined (houdini and crayfits) my configuration would have

```
derivative.systems.installed=houdini,fits

derivative.houdini.enabled=true
derivative.houdini.in.stream=queue:islandora-connector-houdini
derivative.houdini.service.url=http://127.0.0.1/houdini/convert
derivative.houdini.concurrent-consumers=1
derivative.houdini.max-concurrent-consumers=4
derivative.houdini.async-consumer=true

derivative.fits.enabled=true
derivative.fits.in.stream=queue:islandora-connector-fits
derivative.fits.service.url=http://127.0.0.1/crayfits
derivative.fits.concurrent-consumers=2
derivative.fits.max-concurrent-consumers=2
derivative.fits.async-consumer=false
```

#### Customizing HTTP client timeouts

You can alter the HTTP client from the defaults for its request, connection and socket timeouts.
To do this you want to enable the request configurer.

```shell
request.configurer.enabled=true
```

Then set the next 3 timeouts (measured in milliseconds) to the desired timeout.

```shell
request.timeout=-1
connection.timeout=-1
socket.timeout=-1
```

The default for all three is `-1` which indicates no timeout.

#### Alter HTTP options

By default, Alpaca uses two settings for the HTTP component, these are
* disableStreamCache=true
* connectionClose=true

If you want to send additional [configuration parameters](https://camel.apache.org/components/3.18.x/http-component.html#_query_parameters) or alter the existing defaults. You can 
add them as a comma separated list of key=value pairs.

For example
```shell
http.additional_options=authMethod=Basic,authUsername=Jim,authPassword=1234
```

These will be added to ALL http endpoint requests.

**Note**: We are currently running Camel 3.7.6, some configuration parameters on the above linked page might not be supported.

### Deploying/Running

You can see the options by passing the `-h|--help` flag

```shell
> java -jar /opt/alpaca/alpaca.jar -h
Usage: alpaca [-hV] [-c=<configurationFilePath>]
  -h, --help      Show this help message and exit.
  -V, --version   Print version information and exit.
  -c, --config=<configurationFilePath>
                  The path to the configuration file
```

Using the `-V|--version` flag will just return the current version of the application.

```shell
> java -jar /opt/alpaca/alpaca.jar -v
2.0.0
```

To start Alpaca you would pass the external property file with the `-c|--config` flag.

For example if you are using an external properties file located at `/opt/alpaca/alpaca.properties`,
you would run:

```shell
java -jar alpaca.jar -c /opt/alpaca/alpaca.properties
```

To use systemd to start and stop the service create the file `/etc/systemd/system/alpaca.service`:

```
[Unit]
Description=Alpaca service
After=network.target

[Service]
Type=simple
ExecStart=java -jar /opt/alpaca/alpaca.jar -c /opt/alpaca/alpaca.properties
ExecStop=/bin/kill -15 $MAINPID
SuccessExitStatus=143
Restart=always

[Install]
WantedBy=default.target
```

Now you can start the service by running `sudo systemctl start alpaca` and set it to come up when the system reboots with `sudo systemctl enable alpaca`. Check the status by running `sudo systemctl status alpaca`.
