# Installing Tomcat and Cantaloupe

## In this section, we will install:
- [Tomcat 8](https://tomcat.apache.org/download-80.cgi), the Java servlet container that will serve up some Java applications on various endpoints, including, importantly, Fedora
- [Cantaloupe 4](https://cantaloupe-project.github.io/), the image tileserver - running in Tomcat - that will be used to serve up large images in a web-accessible fashion

## Tomcat 8

### Installing OpenJDK 8

Tomcat runs in a Java runtime environment, so we'll need one to continue. In our case, OpenJDK 8 is open-source, free to use, and can fairly simply be installed using `apt-get`:

```bash
sudo apt-get -y install openjdk-8-jdk openjdk-8-jre
```

The installation of OpenJDK via `apt-get` establishes it as the de-facto Java runtime environment to be used on the system, so no further configuration is required.

The resultant location of the java JRE binary (and therefore, the correct value of `JAVA_HOME` when it’s referenced) will vary based on the specifics of the machine it’s being installed on; that being said, you can find its exact location using `update-alternatives`:

```bash
update-alternatives --list java
```

### Creating a `tomcat` User

Apache Tomcat, and all its processes, will be owned and managed by a specific user for the purposes of keeping parts of the stack segregated and accountable.

```bash
sudo addgroup tomcat
sudo adduser tomcat --ingroup tomcat --home /opt/tomcat --shell /usr/bin
```

You will be prompted to create a password for the `tomcat` user; all the other information as part of the `adduser` command can be ignored.

### Downloading and Placing Tomcat 8

Tomcat 8 itself can be installed in several different ways; while it’s possible to install via `apt-get`, this doesn’t give us a great deal of control over exactly how we’re going to run and manage it; as a critical part of the stack, it is beneficial for our purposes to have a good frame of reference for the inner workings of Tomcat.

We’re going to download the latest version of Tomcat to `/opt` and set it up so that it runs automatically. Bear in mind that with the following commands, this is going to be entirely relative to the current version of Tomcat 8, which we’ll try to mitigate as we go.

```bash
cd /opt
sudo wget -O tomcat.tar.gz TOMCAT_TARBALL_LINK
sudo tar -zxvf tomcat.tar.gz
sudo mv /opt/TOMCAT_DIRECTORY/* /opt/tomcat
sudo chown -R tomcat:tomcat /opt/TOMCAT_DIRECTORY
```
- `TOMCAT_TARBALL_LINK`: No default can be provided here; you should navigate to the [Tomcat 8 downloads page](https://tomcat.apache.org/download-80.cgi) and grab the link to the latest `.tar.gz` file under the “Core” section of “Binary Distributions”. It is highly recommended to grab the latest version of Tomcat 8, as it will come with associated security patches and fixes.
- `TOMCAT_DIRECTORY`: This will also depend entirely on the exact version of tomcat downloaded - for example, `apache-tomcat-8.5.47`. Again, `ls /opt` can be used to find this.

### Creating a setenv.sh Script

When Tomcat runs, some configuration needs to be pre-established as a series of environment variables that will be used by the script that runs it.

`/opt/tomcat/bin/setenv.sh | tomcat:tomcat/755`
```
export CATALINA_HOME="/opt/tomcat"
export JAVA_HOME="PATH_TO_JAVA_HOME"
export JAVA_OPTS="-Djava.awt.headless=true -server -Xmx1500m -Xms1000m"
```
- `PATH_TO_JAVA_HOME`: This will vary a bit depending on the environment, but will likely live in `/usr/lib/jvm` somewhere (e.g., `/usr/lib/jvm/java-8-openjdk-amd64` for an installation on a machine with an AMD processor); again, in an Ubunutu environment you can check a part of this using `update-alternatives --list java`, which will give you the path to the JRE binary within the Java home

### Creating the Tomcat Service

Tomcat includes two shell scripts we’re going to make use of - `startup.sh` and `shutdown.sh` - which are light wrappers on top of a third script, `catalina.sh`, which manages spinning up and shutting down the Tomcat server.

Ubuntu 18.04 uses `systemctl` to manage services; we’re going to create a .service file that can run these shell scripts.

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

We can check that Tomcat has started by running `systemctl status tomcat | grep Active`; we should see that Tomcat is `active (running)`, which is the correct result of startup.sh finishing its run successfully.

## Installing Cantaloupe 4

### Stopping the Tomcat service

Before we start working with Cantaloupe, we should `stop` Tomcat; otherwise, Cantaloupe will automatically be deployed from its .war file, and we’d like everything to be in place before the deployment.

```bash
sudo systemctl stop tomcat
```

### Downloading and Placing the Cantaloupe WAR

Releases of Cantaloupe live on the [Cantaloupe release page](https://github.com/cantaloupe-project/cantaloupe/releases); the latest version can be found here as a `.zip` file.

```bash
sudo wget -O /opt/cantaloupe.zip CANTALOUPE_RELEASE_URL
sudo unzip /opt/cantaloupe.zip
sudo cp CANTALOUPE_DIR/CANTALOUPE_WAR /opt/tomcat/webapps/cantaloupe.war
sudo chown tomcat:tomcat /opt/tomcat/webapps/cantaloupe.war
```
- `CANTALOUPE_RELEASE_URL`: It’s recommended we grab the latest version of Cantaloupe 4. This can be found on the above-linked release page, as the `.zip` version; for example, https://github.com/cantaloupe-project/cantaloupe/releases/download/v4.1.4/cantaloupe-4.1.4.zip 
- `CANTALOUPE_DIR`: This will depend on the exact version of Cantaloupe downloaded; in the above example release, this would be `cantaloupe-4.1.4`
- `CANTALOUPE_WAR`: This will also depend on the exact version of Cantaloupe downloaded; in the above example release, this would be `cantaloupe-4.1.4.war`

### Creating a Cantaloupe Configuration

Cantaloupe pulls its configuration from a file called `cantaloupe.properties`; there are also some other files that can contain instructions for Cantaloupe while it’s running; specifically, we’re going to copy over the `delegates.rb` file, which can also contain custom configuration. We won’t make use of this file; we’re just copying it over for demonstration purposes.

Creating these files from scratch is *not* recommended; rather, we’re going to take the default cantaloupe configurations and plop them into their own folder so we can work with them.

```bash
sudo mkdir /opt/cantaloupe_config
sudo cp CANTALOUPE_DIR/cantaloupe.properties.sample /opt/cantaloupe_config/cantaloupe.properties
sudo cp CANTALOUPE_DIR/delegates.rb.sample /opt/cantaloupe_config/delegates.rb
```

The out-of-the-box configuration will work fine for our purposes, but it’s highly recommended that you take a look through the `cantaloupe.properties` and see what changes can be made; specifically, logging to actual logfiles isn’t set up by default, so you may want to take a peek at the `log.application.SyslogAppender` or `log.application.RollingFileAppender`, as well as changing the logging level.

### Defining the Cantaloupe Configuration Location

Now that we have a Cantaloupe configuration, we need to make a change to Tomcat’s `JAVA_OPTS` so that its location can be referenced when Tomcat spins it up. This will involve changing the `setenv.sh` created when setting up Tomcat.

`/opt/tomcat/bin/setenv.sh`

**Before**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -server -Xmx1500m -Xms1000m"

**After**:
> 3 | export JAVA_OPTS="-Djava.awt.headless=true -Dcantaloupe.config=/opt/cantaloupe_config/cantaloupe.properties -server -Xmx1500m -Xms1000m"

### Starting the Tomcat Service

After Cantaloupe has been completely provisioned, we’re ready to switch Tomcat back on so that Cantaloupe automatically deploys with the established configuration.

```bash
sudo systemctl start tomcat
```
