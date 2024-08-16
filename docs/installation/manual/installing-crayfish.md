# Installing Crayfish

!!! warning "Needs Maintenance"
    The manual installation documentation is in need of attention. We are aware that some components no longer work as documented here. If you are interested in helping us improve the documentation, please see [Contributing](../../contributing/CONTRIBUTING.md).

## In this section, we will install:
- [FITS Web Service](https://projects.iq.harvard.edu/fits), a webservice for identifying file metadata
- [Islandora/Crayfish](https://github.com/islandora/crayfish), the suite of microservices that power the backend of Islandora 2.0
- Indvidual microservices underneath Crayfish

## FITS Web Service

The FITS Web Service is used to extract file metadata from files. The Crayfish microservice CrayFits will use this service to push FITS metadata back to Drupal. It comes in two pieces, the actual FITS tool and the FITS Webservice which runs in Tomcat.

FITS itself wraps other file identification and metadata tools which may require installing additional libraries. On Ububtu 20.04, the version this guide is using, we will install a few:

```bash
sudo apt install mediainfo python3-jpylyzer
```

To set up the FITS application, first find the [latest FITS version on GitHub](https://github.com/harvard-lts/fits/releases) to replace the `[FITS_VERSION_NUMBER]` and then run the following commands:

```bash
cd /opt
sudo wget https://github.com/harvard-lts/fits/releases/download/[FITS_VERSION_NUMBER]/fits-[FITS_VERSION_NUMBER].zip
sudo unzip /opt/fits-[FITS_VERSION_NUMBER].zip -d /opt/fits
```

Similarly with the FITS webservice, [get the current service version number](https://github.com/harvard-lts/FITSservlet/releases) to replace `[FITS_SERVICE_WAR_VERSION_NUMBER]`:

Download the FITS webservice:

```bash
sudo -u tomcat wget -O /opt/tomcat/webapps/fits.war https://github.com/harvard-lts/FITSservlet/releases/download/[FITS_SERVICE_WAR_VERSION_NUMBER]/fits-service-[FITS_SERVICE_WAR_VERSION_NUMBER].war
```

Configure the webservice but adding the following lines to the bottom of `/opt/tomcat/conf/catalina.properties`:

```
fits.home=/opt/fits
shared.loader=/opt/fits/lib/*.jar
```

Restart Tomcat:

```
sudo systemctl restart tomcat
```

Wait for a few minutes to let the service start up the first time and then visit `http://localhost:8080/fits/` to ensure it is working. You can also follow the catalina logs to see how tomcat is progressing in setting up each service it is running: `sudo tail -f /opt/tomcat/logs/catalina.out`. To stop following the logs, hit control-C.

## Crayfish 2.0

### Installing Prerequisites

Some packages need to be installed before we can proceed with installing Crayfish; these packages are used by the microservices within Crayfish. These include:

- Imagemagick, which will be used for image processing. We'll be using the LYRASIS build of imagemagick here, which supports JP2 files.
- Tesseract, which will be used for optical character recognition; note that by default Tesseract can only understand English; several other individual Tesseract language packs can be installed using `apt-get`, and a list of available packs can be procured with `sudo apt-cache search tesseract-ocr`
- FFMPEG, which will be used for video processing
- Poppler, which will be used for generating PDFs

```bash
sudo apt-get install software-properties-common
sudo add-apt-repository -y ppa:lyrasis/imagemagick-jp2
sudo apt-get update
sudo apt-get -y install imagemagick tesseract-ocr ffmpeg poppler-utils
```

### Cloning and Installing Crayfish

We’re going to clone Crayfish to `/opt`, and individually run `composer install` against each of the microservice subdirectories.

```bash
cd /opt
sudo git clone https://github.com/Islandora/Crayfish.git crayfish
sudo chown -R www-data:www-data crayfish
sudo -u www-data composer install -d crayfish/Homarus
sudo -u www-data composer install -d crayfish/Houdini
sudo -u www-data composer install -d crayfish/Hypercube
sudo -u www-data composer install -d crayfish/Milliner
sudo -u www-data composer install -d crayfish/Recast
sudo -u www-data composer install -d crayfish/CrayFits
```

### Preparing Logging

Not much needs to happen here; Crayfish opts for a simple logging approach, with one `.log` file for each component. We’ll create a folder where each logfile can live.

```bash
sudo mkdir /var/log/islandora
sudo chown www-data:www-data /var/log/islandora
```

### Configuring Crayfish Components

Each Crayfish component requires one or more `.yaml` file(s) to ensure everything is wired up correctly.

!!! note "Update the defaults to meet your needs"

The following configuration files represent somewhat sensible defaults; you should take consideration of the logging levels in use, as this can vary in desirability from installation to installation. Also note that in all cases, `http` URLs are being used, as this guide does not deal with setting up https support. In a production installation, this should not be the case. These files also assume a connection to a PostgreSQL database; use a `pdo_mysql` driver and the appropriate `3306` port if using MySQL.

!!! note "Using JWT for Crayfish Authentication"
    For Crayfish microservices use the `lexik_jwt_authentication` package. They are configured to use the `JWT_PUBLIC_KEY` environment variable to find the public key we created earlier (`/opt/keys/syn_public.key`). Later on in this guide we will add the environment variable to the Apache configs, but you may alternatively write the path to the key in the `lexik_jwt_authentication.yaml` file that resides along-side the `security.yaml` files we edit in this section.

#### Homarus (Audio/Video derivatives)

Enable JSON Web Token (JWT) based access to the service by updating the security settings. Edit `/opt/crayfish/Homarus/config/packages/security.yaml` to set firewalls: main: anonymous to `false` and uncomment the `provider` and `jwt` lines further down in that section.

Edit `/opt/crayfish/Homarus/config/packages/monolog.yaml` to point to the new logging directory:

```yml
        homarus:
            type: rotating_file
            path: /var/logs/islandora/Homarus.log
```

Edit the commons config to update it with Fedora's location (if necessary) and enable the apix middleware in `/opt/crayfish/Homarus/config/packages/crayfish_commons.yaml`:

```yml
crayfish_commons:
  fedora_base_uri: 'http://localhost:8080/fcrepo/rest'
  apix_middleware_enabled: true
```

#### Houdini (Image derivatives)

Currently the Houdini microservice uses a different system (Symfony) than the other microservices, this requires different configuration.

`/opt/crayfish/Houdini/config/services.yaml | www-data:www-data/644`
```yaml
# This file is the entry point to configure your own services.
# Files in the packages/ subdirectory configure your dependencies.
# Put parameters here that don't need to change on each machine where the app is deployed
# https://symfony.com/doc/current/best_practices/configuration.html#application-related-configuration
parameters:
    app.executable: /usr/bin/convert
    app.formats.valid:
        - image/jpeg
        - image/png
        - image/tiff
        - image/jp2
    app.formats.default: image/jpeg

services:
    # default configuration for services in *this* file
    _defaults:
        autowire: true      # Automatically injects dependencies in your services.
        autoconfigure: true # Automatically registers your services as commands, event subscribers, etc.

    # makes classes in src/ available to be used as services
    # this creates a service per class whose id is the fully-qualified class name
    App\Islandora\Houdini\:
        resource: '../src/*'
        exclude: '../src/{DependencyInjection,Entity,Migrations,Tests,Kernel.php}'

    # controllers are imported separately to make sure services can be injected
    # as action arguments even if you don't extend any base controller class
    App\Islandora\Houdini\Controller\HoudiniController:
        public: false
        bind:
            $formats: '%app.formats.valid%'
            $default_format: '%app.formats.default%'
            $executable: '%app.executable%'
        tags: ['controller.service_arguments']

    # add more service definitions when explicit configuration is needed
    # please note that last definitions always *replace* previous ones
```

`/opt/crayfish/Houdini/config/packages/crayfish_commons.yaml | www-data:www-data/644`
```yaml
crayfish_commons:
  fedora_base_uri: 'http://localhost:8080/fcrepo/rest'
  syn_config: /opt/fcrepo/config/syn-settings.xml
  syn_enabled: True
```

`/opt/crayfish/Houdini/config/packages/monolog.yaml | www-data:www-data/644`
```yaml
monolog:

  handlers:

    houdini:
      type: rotating_file
      path: /var/log/islandora/Houdini.log
      level: DEBUG
      max_files: 1
```

The below files are two versions of the same file to enable or disable JWT token authentication.

`/opt/crayfish/Houdini/config/packages/security.yaml | www-data:www-data/644`

Enabled JWT token authentication:
```yaml
# To disable Syn checking, set syn_enabled=false in crayfish_commons.yaml and remove this configuration file.
security:

    # https://symfony.com/doc/current/security.html#where-do-users-come-from-user-providers
    providers:
        users_in_memory: { memory: null }
        jwt:
            lexik_jwt: ~
    firewalls:
        dev:
            pattern: ^/(_(profiler|wdt)|css|images|js)/
            security: false
        main:
            # To enable Syn, change anonymous to false and uncomment the lines further below
            anonymous: false
            # Need stateless or it reloads the User based on a token.
            stateless: true

            # To enable JWT authentication, uncomment the below 2 lines and change anonymous to false above.
            provider: jwt
            jwt: ~

            # activate different ways to authenticate
            # https://symfony.com/doc/5.4/security.html#firewalls-authentication

            # https://symfony.com/doc/5.4/security/impersonating_user.html
            # switch_user: true

    # Easy way to control access for large sections of your site
    # Note: Only the *first* access control that matches will be used
    access_control:
        # - { path: ^/admin, roles: ROLE_ADMIN }
        # - { path: ^/profile, roles: ROLE_USER }
```

Disabled JWT token authentication:
```yaml
security:

    # https://symfony.com/doc/current/security.html#where-do-users-come-from-user-providers
    providers:
        jwt_user_provider:
            id: Islandora\Crayfish\Commons\Syn\JwtUserProvider

    firewalls:
        dev:
            pattern: ^/(_(profiler|wdt)|css|images|js)/
            security: false
        main:
            anonymous: true
            # Need stateless or it reloads the User based on a token.
            stateless: true
```

#### Hypercube (OCR)

Enable JSON Web Token (JWT) based access to the service by updating the security settings. Edit `/opt/crayfish/Hypercube/config/packages/security.yaml` to set firewalls: main: anonymous to `false` and uncomment the `provider` and `jwt` lines further down in that section.

Edit `/opt/crayfish/Hypercube/config/packages/monolog.yaml` to point to the new logging directory:

```yml
        hypercube:
            type: rotating_file
            path: /var/logs/islandora/Hypercube.log
```

Edit the commons config to update it with Fedora's location (if necessary) and enable the apix middleware in `/opt/crayfish/Hypercube/config/packages/crayfish_commons.yaml`:

```yml
crayfish_commons:
  fedora_base_uri: 'http://localhost:8080/fcrepo/rest'
  apix_middleware_enabled: true
```

#### Milliner (Fedora indexing)

Enable JSON Web Token (JWT) based access to the service by updating the security settings. Edit `/opt/crayfish/Milliner/config/packages/security.yaml` to set firewalls: main: anonymous to `false` and uncomment the `provider` and `jwt` lines further down in that section.

Edit `/opt/crayfish/Milliner/config/packages/monolog.yaml` to point to the new logging directory:

```yml
        milliner:
            type: rotating_file
            path: /var/logs/islandora/Milliner.log
```

Edit the commons config to update it with Fedora's location (if necessary) and enable the apix middleware in `/opt/crayfish/Milliner/config/packages/crayfish_commons.yaml`:

### Creating Apache Configurations for Crayfish Components

Finally, we need appropriate Apache configurations for Crayfish; these will allow other services to connect to Crayfish components via their HTTP endpoints.

Each endpoint we need to be able to connect to will get its own `.conf` file, which we will then enable.

!!! warning "Possible Route Collisions"
    These configurations would potentially have collisions with Drupal routes, if any are created in Drupal with the same name. If this is a concern, it would likely be better to reserve a subdomain or another port specifically for Crayfish. For the purposes of this installation guide, these endpoints will suffice.

`/etc/apache2/conf-available/Homarus.conf | root:root/644`
```
Alias "/homarus" "/opt/crayfish/Homarus/public"
<Directory "/opt/crayfish/Homarus/public">
  FallbackResource /homarus/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnv JWT_PUBLIC_KEY /opt/keys/syn_public.key
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/Houdini.conf | root:root/644`
```
Alias "/houdini" "/opt/crayfish/Houdini/public"
<Directory "/opt/crayfish/Houdini/public">
  FallbackResource /houdini/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnv JWT_PUBLIC_KEY /opt/keys/syn_public.key
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/Hypercube.conf | root:root/644`
```
Alias "/hypercube" "/opt/crayfish/Hypercube/public"
<Directory "/opt/crayfish/Hypercube/public">
  FallbackResource /hypercube/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnv JWT_PUBLIC_KEY /opt/keys/syn_public.key
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/Milliner.conf | root:root/644`
```
Alias "/milliner" "/opt/crayfish/Milliner/public"
<Directory "/opt/crayfish/Milliner/public">
  FallbackResource /milliner/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnv JWT_PUBLIC_KEY /opt/keys/syn_public.key
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/CrayFits.conf | root:root/644`
```
Alias "/crayfits" "/opt/crayfish/CrayFits/public"
<Directory "/opt/crayfish/CrayFits/public">
  FallbackResource /crayfits/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnv JWT_PUBLIC_KEY /opt/keys/syn_public.key
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

### Enabling Each Crayfish Component Apache Configuration

Enabling each of these configurations involves creating a symlink to them in the `conf-enabled` directory; the standardized method of doing this in Apache is with `a2enconf`.

```bash
sudo a2enconf Homarus Houdini Hypercube Milliner CrayFits
```

### Restarting the Apache Service

Finally, to get these new endpoints up and running, we need to restart the Apache service.

```
sudo systemctl restart apache2
```
