# Installing Tomcat and Cantaloupe

!!! warning "Needs Maintenance"
    The manual installation documentation is in need of attention. We are aware that some components no longer work as documented here. If you are interested in helping us improve the documentation, please see [Contributing](../../../contributing/CONTRIBUTING).

## In this section, we will install:

- [Tomcat 9](https://tomcat.apache.org/download-90.cgi), the Java servlet container that will serve up some Java applications on various endpoints, including, importantly, Fedora
- [Cantaloupe 5](https://cantaloupe-project.github.io/), the image tileserver - running in Tomcat - that will be used to serve up large images in a web-accessible fashion

## Tomcat 9

### Creating a `tomcat` User

Apache Tomcat, and all its processes, will be owned and managed by a specific user for the purposes of keeping parts of the stack segregated and accountable.

```bash
sudo addgroup tomcat
sudo adduser tomcat --ingroup tomcat --home /opt/tomcat --shell /usr/bin
```

You will be prompted to create a password for the `tomcat` user; all the other information as part of the `adduser` command can be ignored.

### Downloading and Placing Tomcat 9

Tomcat 9 itself can be installed in several different ways; while it’s possible to install via `apt-get`, this doesn’t give us a great deal of control over exactly how we’re going to run and manage it; as a critical part of the stack, it is beneficial for our purposes to have a good frame of reference for the inner workings of Tomcat.

We’re going to download the latest version of Tomcat to `/opt` and set it up so that it runs automatically. Bear in mind that with the following commands, this is going to be entirely relative to the current version of Tomcat 9, which we’ll try to mitigate as we go.

```bash
cd /opt
sudo wget -O tomcat.tar.gz TOMCAT_TARBALL_LINK
sudo tar -zxvf tomcat.tar.gz
sudo mv /opt/TOMCAT_DIRECTORY/* /opt/tomcat
sudo chown -R tomcat:tomcat /opt/tomcat
```
- `TOMCAT_TARBALL_LINK`: No default can be provided here; you should navigate to the [Tomcat 9 downloads page](https://tomcat.apache.org/download-90.cgi) and grab the link to the latest `.tar.gz` file under the “Core” section of “Binary Distributions”. It is highly recommended to grab the latest version of Tomcat 9, as it will come with associated security patches and fixes.
- `TOMCAT_DIRECTORY`: This will also depend entirely on the exact version of tomcat downloaded - for example, `apache-tomcat-9.0.50`. Again, `ls /opt` can be used to find this.

### Creating a setenv.sh Script

When Tomcat runs, some configuration needs to be pre-established as a series of environment variables that will be used by the script that runs it.

`/opt/tomcat/bin/setenv.sh | tomcat:tomcat/755`
```
export CATALINA_HOME="/opt/tomcat"
export JAVA_HOME="PATH_TO_JAVA_HOME"
export JAVA_OPTS="-Djava.awt.headless=true -server -Xmx1500m -Xms1000m"
```
- `PATH_TO_JAVA_HOME`: This will vary a bit depending on the environment, but will likely live in `/usr/lib/jvm` somewhere (e.g., `/usr/lib/jvm/java-11-openjdk-amd64`); again, in an Ubunutu environment you can check a part of this using `update-alternatives --list java`, which will give you the path to the JRE binary within the Java home. Note that `update-alternatives --list java` will give you the path to the binary, so for `PATH_TO_JAVA_HOME` delete the `/bin/java` at the end to get the Java home directory, so it should look something like this:
```
export JAVA_HOME="/usr/lib/jvm/java-11-openjdk-amd64"
```

### Creating the Tomcat Service

Tomcat includes two shell scripts we’re going to make use of - `startup.sh` and `shutdown.sh` - which are light wrappers on top of a third script, `catalina.sh`, which manages spinning up and shutting down the Tomcat server.

Debian and Ubuntu use `systemctl` to manage services; we’re going to create a .service file that can run these shell scripts.

`/etc/systemd/system/tomcat.service | root:root/755`
```
[Unit]
Description=Tomcat

[Service]
Type=forking
ExecStart=/opt/tomcat/bin/startup.sh
ExecStop=/opt/tomcat/bin/shutdown.sh
SyslogIdentifier=tomcat

[Install]
WantedBy=multi-user.target
```

### Enabling and Starting Tomcat

We’re going to both `enable` and `start` Tomcat. Enabling Tomcat will ensure that it starts on boot, the timing of which is defined by the `[Install]` section’s `WantedBy` statement, which specifies what it should start after. This is separate from starting it, which we need to do now in order to get Tomcat up and running without requiring a reboot.

```bash
sudo systemctl enable tomcat
sudo systemctl start tomcat
```

We can check that Tomcat has started by running `sudo systemctl status tomcat | grep Active`; we should see that Tomcat is `active (running)`, which is the correct result of startup.sh finishing its run successfully.

## Installing Cantaloupe 5

Since version 5, Cantaloupe is released as a standalone Java application and is no longer deployed in Tomcat via a .war file. Even so, we can still fine-tune how it runs and even install it as a service.

### Downloading Cantaloupe

Releases of Cantaloupe live on the [Cantaloupe release page](https://github.com/cantaloupe-project/cantaloupe/releases); the latest version can be found here as a `.zip` file.

```bash
cd /opt/
sudo wget -O cantaloupe.zip CANTALOUPE_RELEASE_URL
sudo unzip cantaloupe.zip
sudo mv [CANTALOUPE_VERSION] cantaloupe
sudo rm cantaloupe.zip
```
- `CANTALOUPE_RELEASE_URL`: It’s recommended we grab the latest version of Cantaloupe 5. This can be found on the above-linked release page, as the `.zip` version; for example, https://github.com/cantaloupe-project/cantaloupe/releases/download/v5.0.6/cantaloupe-5.0.6.zip - make sure **not** to download the source code zip file as that isn't compiled for running out-of-the-box.
- `CANTALOUPE_VERSION`: This will depend on the exact version of Cantaloupe downloaded; in the above example release, this would be `cantaloupe-5.0.6`

### Creating a Cantaloupe Configuration

Cantaloupe pulls its configuration from a file called `cantaloupe.properties`; there are also some other files that can contain instructions for Cantaloupe while it’s running; specifically, we’re going to copy over the `delegates.rb` file, which can also contain custom configuration. We won’t make use of this file; we’re just copying it over for demonstration purposes.

Creating these files from scratch is *not* recommended; rather, we’re going to take the default cantaloupe configurations and plop them into their own folder so we can work with them.

```bash
cd cantaloupe
sudo cp cantaloupe.properties.sample cantaloupe.properties
sudo cp delegates.rb.sample delegates.rb
```

Most of the out-of-the-box configuration will work fine for our purposes. We will change the source lookup and logging, but it’s highly recommended that you take a look through the rest of the `cantaloupe.properties` and see what changes can be made. Review the config block below and change the related portions of yours to match.

`/opt/cantaloupe/cantaloupe.properties`
```
##############
# SOURCES
##############

source.static = HttpSource

##############
# HttpSource
##############

HttpSource.BasicLookupStrategy.url_prefix =

##############
# LOGGING
##############

log.application.FileAppender.pathname = /var/log/islandora/cantaloupe-application.log

log.application.RollingFileAppender.enabled = true
log.application.RollingFileAppender.pathname = /var/log/islandora/cantaloupe-application.log
log.application.RollingFileAppender.TimeBasedRollingPolicy.filename_pattern = /var/log/islandora/cantaloupe-application-%d{yyyy-MM-dd}.log

log.error.FileAppender.pathname = /var/log/islandora/cantaloupe-error.log

log.error.RollingFileAppender.enabled = true
log.error.RollingFileAppender.pathname = /var/log/islandora/cantaloupe-error.log
log.error.RollingFileAppender.TimeBasedRollingPolicy.filename_pattern = /var/log/islandora/cantaloupe-error-%d{yyyy-MM-dd}.log

log.access.FileAppender.pathname = /var/log/islandora/cantaloupe-access.log

log.access.RollingFileAppender.enabled = true
log.access.RollingFileAppender.pathname = /var/log/islandora/cantaloupe-access.log
log.access.RollingFileAppender.TimeBasedRollingPolicy.filename_pattern = /var/log/islandora/cantaloupe-access-%d{yyyy-MM-dd}.log
```

### Installing and configuring Cantaloupe as a service

Since it is a standalone application, we can configure Cantaloupe as a systemd service like we did with Tomcat, so it can start on boot:

`/etc/systemd/system/cantaloupe.service | root:root/755`
```
[Unit]
Description=Cantaloupe

[Service]
ExecStart=java -cp /opt/cantaloupe/cantaloupe-CANTALOUPE_VER.jar -Dcantaloupe.config=/opt/cantaloupe/cantaloupe.properties -Xmx1500m -Xms1000m edu.illinois.library.cantaloupe.StandaloneEntry
SyslogIdentifier=cantaloupe

[Install]
WantedBy=multi-user.target
```
- `CANTALOUPE_VER`: This will depend on the exact version of Cantaloupe downloaded; in the above example release, this would be `cantaloupe-5.0.3`

We can now enable the service and run it:

```bash
sudo systemctl enable cantaloupe
sudo systemctl start cantaloupe
```

We can check the service status with `sudo systemctl status cantaloupe | grep Active` and the splash screen of Cantaloupe should be available at http://localhost:8182/iiif/2.

If you have trouble connecting, check the status of your port and allow it if necessary:

```bash
sudo ufw status verbose
sudo ufw allow 8182/tcp
```