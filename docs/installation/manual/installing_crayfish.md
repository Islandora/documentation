# Installing Crayfish

## In this section, we will install:
- [Islandora/Crayfish](https://github.com/islandora/crayfish), the suite of microservices that power the backend of Islandora 8
- Indvidual microservices underneath Crayfish

## Crayfish 1.1

### Installing Prerequisites

Some packages need to be installed before we can proceed with installing Crayfish; these packages are used by the microservices within Crayfish. These include:

- Imagemagick, which will be used for image processing. We'll be using the LYRASIS build of imagemagick here, which supports JP2 files.
- Tesseract, which will be used for optical character recognition; note that by default Tesseract can only understand English; several other individual Tesseract language packs can be installed using `apt-get`, and a list of available packs can be procured with `sudo apt-cache search tesseract-ocr`
- FFMPEG, which will be used for video processing
- Poppler, which will be used for generating PDFs

```bash
sudo add-apt-repository -y ppa:lyrasis/imagemagick-jp2
sudo apt-get update
sudo apt-get -y install imagemagick tesseract-ocr ffmpeg poppler-utils
```

### Preparing a Gemini Database

This database will be set up (and function) mostly the same as the other databases we’ve previously installed.

```bash
sudo -u postgres psql
create database CRAYFISH_DB;
create user CRAYFISH_DB_USER with encrypted password 'CRAYFISH_DB_PASSWORD';
grant all privileges on database CRAYFISH_DB to CRAYFISH_DB_USER;
\q
```
- `CRAYFISH_DB`: `gemini`
- `CRAYFISH_DB_USER`: `gemini`
- `CRAYFISH_DB_PASSWORD`: `gemini`
    - As always, this should be a secure password of some kind, and not this default.

### Cloning and Installing Crayfish

We’re going to clone Crayfish to `/opt`, and individually run `composer install` against each of the microservice subdirectories.

```bash
cd /opt
sudo git clone https://github.com/Islandora/Crayfish.git crayfish
sudo chown -R www-data:www-data crayfish
sudo -u www-data composer install -d crayfish/Gemini
sudo -u www-data composer install -d crayfish/Homarus
sudo -u www-data composer install -d crayfish/Houdini
sudo -u www-data composer install -d crayfish/Hypercube
sudo -u www-data composer install -d crayfish/Milliner
sudo -u www-data composer install -d crayfish/Recast
```

### Preparing Logging

Not much needs to happen here; Crayfish opts for a simple logging approach, with one `.log` file for each component. We’ll create a folder where each logfile can live.

```bash
sudo mkdir /var/log/islandora
sudo chown www-data:www-data /var/log/islandora
```

### Configuring Crayfish Components

Each Crayfish component requires one or more `.yaml` file(s) to ensure everything is wired up correctly.

!!! notice
    The following configuration files represent somewhat sensible defaults; you should take consideration of the logging levels in use, as this can vary in desirability from installation to installation. Also note that in all cases, `http` URLs are being used, as this guide does not deal with setting up https support. In a production installation, this should not be the case. These files also assume a connection to a PostgreSQL database; use a `pdo_mysql` driver and the appropriate `3306` port if using MySQL.


#### Gemini

`/opt/crayfish/Gemini/cfg/config.yaml | www-data:www-data/644`
```yaml
---
debug: false
fedora_base_url: http://localhost:8080/fcrepo/rest
db.options:
  driver: pdo_pgsql
  host: 127.0.0.1
  port: 5432
  dbname: CRAYFISH_DB
  user: CRAYFISH_DB_USER
  password: CRAYFISH_DB_PASSWORD
log:
  level: NOTICE
  file: /var/log/islandora/gemini.log
syn:
  enable: true
  config: /opt/fcrepo/config/syn-settings.xml
```

#### Homarus (Audio/Video derivatives)

`/opt/crayfish/Homarus/cfg/config.yaml | www-data:www-data/644`
```yaml
---
homarus:
  executable: ffmpeg
  mime_types:
    valid:
      - video/mp4
      - video/x-msvideo
      - video/ogg
      - audio/x-wav
      - audio/mpeg
      - audio/aac
      - image/jpeg
      - image/png
    default: video/mp4
  mime_to_format:
    valid:
      - video/mp4_mp4
      - video/x-msvideo_avi
      - video/ogg_ogg
      - audio/x-wav_wav
      - audio/mpeg_mp3
      - audio/aac_m4a
      - image/jpeg_image2pipe
      - image/png_image2pipe
    default: mp4
fedora_resource:
  base_url: http://localhost:8080/fcrepo/rest
log:
  level: NOTICE
  file: /var/log/islandora/homarus.log
syn:
  enable: true
  config: /opt/fcrepo/config/syn-settings.xml
```

#### Houdini (Image derivatives)

Currently the Houdini microservice uses a different system (Symfony) than the other microservices, this requires different configuration.

`/opt/crayfish/Houdini/config/services.yaml | www-data:www-data/644`
```yaml
\# This file is the entry point to configure your own services.
\# Files in the packages/ subdirectory configure your dependencies.
\# Put parameters here that don't need to change on each machine where the app is deployed
\# https://symfony.com/doc/current/best_practices/configuration.html#application-related-configuration
parameters:
    app.executable: /usr/local/bin/convert
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

`/opt/crayfish/Houdini/config/packages/crayfish_commons.yml | www-data:www-data/644`
```yaml
crayfish_commons:
  fedora_base_uri: 'http://localhost:8080/fcrepo/rest'
  gemini_base_uri: 'http://localhost:9000/gemini'
  syn_config: '/opt/fcrepo/config/syn-settings.xml'
```

`/opt/crayfish/Houdini/config/packages/monolog.yml | www-data:www-data/644`
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

`/opt/crayfish/Houdini/config/packages/security.yml | www-data:www-data/644`

Enabled JWT token authentication:
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
            anonymous: false
            # Need stateless or it reloads the User based on a token.
            stateless: true

            provider: jwt_user_provider
            guard:
                authenticators:
                    - Islandora\Crayfish\Commons\Syn\JwtAuthenticator

            # activate different ways to authenticate
            # https://symfony.com/doc/current/security.html#firewalls-authentication

            # https://symfony.com/doc/current/security/impersonating_user.html
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

`/opt/crayfish/Hypercube/cfg/config.yaml | www-data:www-data/644`
```yaml
---
hypercube:
  tesseract_executable: tesseract
  pdftotext_executable: pdftotext
fedora_resource:
  base_url: http://localhost:8080/fcrepo/rest
log:
  level: NOTICE
  file: /var/log/islandora/hypercube.log
syn:
  enable: true
  config: /opt/fcrepo/config/syn-settings.xml
```

#### Milliner (Fedora indexing)

`/opt/crayfish/Milliner/cfg/config.yaml | www-data:www-data/644`
```yaml
---
fedora_base_url: http://localhost:8080/fcrepo/rest
drupal_base_url: http://localhost
gemini_base_uri: http://localhost/gemini
modified_date_predicate: http://schema.org/dateModified
strip_format_jsonld: true
debug: false
db.options:
  driver: pdo_pgsql
  host: 127.0.0.1
  port: 5432
  dbname: CRAYFISH_DB
  user: CRAYFISH_DB_USER
  password: CRAYFISH_DB_PASSWORD
log:
  level: NOTICE
  file: /var/log/islandora/milliner.log
syn:
  enable: true
  config: /opt/fcrepo/config/syn-settings.xml
```

#### Recast (Drupal to Fedora URI re-writing)

`/opt/crayfish/Recast/cfg/config.yaml | www-data:www-data/644`
```yaml
---
fedora_resource:
  base_url: http://localhost:8080/fcrepo/rest
gemini_base_url: http://localhost/gemini
drupal_base_url: http://localhost
debug: false
log:
  level: NOTICE
  file: /var/log/islandora/recast.log
syn:
  enable: true
  config: /opt/fcrepo/config/syn-settings.xml
namespaces:
-
  acl: "http://www.w3.org/ns/auth/acl#"
  fedora: "http://fedora.info/definitions/v4/repository#"
  ldp: "http://www.w3.org/ns/ldp#"
  memento: "http://mementoweb.org/ns#"
  pcdm: "http://pcdm.org/models#"
  pcdmuse: "http://pcdm.org/use#"
  webac: "http://fedora.info/definitions/v4/webac#"
  vcard: "http://www.w3.org/2006/vcard/ns#"
```

### Installing the Gemini Database

Our Gemini database is unusable until it's installed.

```bash
cd /opt/crayfish/Gemini
php bin/console --no-interaction migrations:migrate
```

### Creating Apache Configurations for Crayfish Components

Finally, we need appropriate Apache configurations for Crayfish; these will allow other services to connect to Crayfish components via their HTTP endpoints.

Each endpoint we need to be able to connect to will get its own `.conf` file, which we will then enable.

!!! notice
    These configurations would potentially have collisions with Drupal routes, if any are created in Drupal with the same name. If this is a concern, it would likely be better to reserve a subdomain or another port specifically for Crayfish. For the purposes of this installation guide, these endpoints will suffice.

`/etc/apache2/conf-available/Gemini.conf | root:root/644`
```
Alias "/gemini" "/opt/crayfish/Gemini/src"
<Directory "/opt/crayfish/Gemini/src">
  FallbackResource /gemini/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/Homarus.conf | root:root/644`
```
Alias "/homarus" "/opt/crayfish/Homarus/src"
<Directory "/opt/crayfish/Homarus/src">
  FallbackResource /homarus/index.php
  Require all granted
  DirectoryIndex index.php
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
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/Hypercube.conf | root:root/644`
```
Alias "/hypercube" "/opt/crayfish/Hypercube/src"
<Directory "/opt/crayfish/Hypercube/src">
  FallbackResource /hypercube/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/Milliner.conf | root:root/644`
```
Alias "/milliner" "/opt/crayfish/Milliner/src"
<Directory "/opt/crayfish/Milliner/src">
  FallbackResource /milliner/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

`/etc/apache2/conf-available/Recast.conf | root:root/644`
```
Alias "/recast" "/opt/crayfish/Recast/src"
<Directory "/opt/crayfish/Recast/src">
  FallbackResource /recast/index.php
  Require all granted
  DirectoryIndex index.php
  SetEnvIf Authorization "(.*)" HTTP_AUTHORIZATION=$1
</Directory>
```

### Enabling Each Crayfish Component Apache Configuration

Enabling each of these configurations involves creating a symlink to them in the `conf-enabled` directory; the standardized method of doing this in Apache is with `a2enconf`.

```bash
sudo a2enconf Gemini Homarus Houdini Hypercube Milliner Recast
```

### Restarting the Apache Service

Finally, to get these new endpoints up and running, we need to restart the Apache service.

```
sudo systemctl restart apache2
```
