# Converting to modern ISLE Site Template

This guide is intended to help convert ISLE-DC sites or ISLE Site Template sites that use docker compose profiles to manage a `dev` and `prod` profile for docker services.

Converting your docker-compose is completely optional. If you are happy with your setup, you can ignore this conversion guide. However, if you'd like to align with the supported community version, the following instructions explain how to convert your site.

## Major changes

If you're trying to decide whether you want to convert, these are the major changes from older versions of ISLE Site Template and ISLE-DC. If any of these address issues you've had with your setup, you may want to consider converting.

### docker-compose.yml

The major change from isle-dc is `docker-compose.yml` is now tracked in version control, so both development and production have the same `docker-compose.yml` and any changes are tracked in git.

This means that in ISLE-DC you had separate docker-compose.yml files for development and production, but in site template there is one file for both.

Older versions of ISLE Site Template used docker compose profiles to manage a `dev` and `prod` profile for docker services. This has been consolidated into a single set of services that can be managed with `make` commands.

### codebase now in ./drupal/rootfs/var/www/drupal

ISLE-DC had the Drupal codebase in `./codebase`. The new location is `./drupal/rootfs/var/www/drupal`

### Default to HTTP 

The default dev environment uses `http` rather than `https`. Running `https` in development environments has proven difficult to support for various host setups. For example, in production some institutions have TLS terminated in front of ISLE (having ISLE act as a backend on a frontend reverse proxy) so having better support around switching from TLS to HTTP seemed warranted. To help with this there are some new `make` commands to allow switching between http, tls-mkcert, and tls-letsencrypt easier.

### Default domain

Using `http` also forced moving away from `islandora.dev`... HSTS rules in Google Chrome in particular do not allow accessing `*.dev` domains over http. So the default development domain is now `islandora.traefik.me`

### traefik labels to YML

Traefik labels have moved from docker compose YML and into [dynamic traefik templates](https://doc.traefik.io/traefik/reference/install-configuration/providers/others/file/) to make configuring traefik easier and more centralized. The traefik configuration is found in `./conf/traefik`

### smart port allocation

In development environments (i.e. `DEVELOPMENT_ENVIRONMENT==true`) `make up` tries to bring up ISLE on port 80/443 but if those ports are bound by some other process than the compose project it tries to bind to another port. The URL is then automatically opened in the web browser. This is mostly useful in a development environment where other sites might be occupying those ports.

### make status

Added a command to help identify misconfigured docker-compose.yml + .env combinations.

### URLs

The Site Template handles the URLs for microservices differently than ISLE-DC did. You will need to follow the instructions in the Site Template README to allow access to your microservices as a subdomain. For example, `solr.mydomain.com`. 

!!! Note "Subdomains"
    If you are already using a subdomain and can’t use URLs like `solr.mysubdomain.mydomain.com`, you can change this in the Traefik section of the docker-compose.yml file, by changing the aliases from `solr.{DOMAIN}` to something else, like `solr-{DOMAIN}`. You will also need to change this in a few other spots in your docker-compose.yml. You can find them by searching for `.{DOMAIN}`

## Conversion process

!!! note
    This doc assummes you default git branch is called `main`. Adjust the commands below if that's not the case

### 1. Bring in the new config

There are two different options to convert docker-compose. If you're converting from ISLE-DC, choose 1.a. If you're converting from the older version of ISLE Site Template, follow 1.b

#### 1.a ISLE-DC

1. Bring in the latest isle-site-template:
```
git clone https://github.com/islandora-devops/isle-site-template
cd isle-site-template
```
2. Copy your drupal codebase into it
```
rm -rf drupal/rootfs/var/www/drupal
cp -r /path/to/isle/dc/codebase drupal/rootfs/var/www/drupal
```
3. Update `docker-compose.yml` doing a find replace.
Find: `drupal-public-files`
Replace: `drupal-sites-data`

Find: `DRUPAL_DEFAULT_SOLR_CORE: default`
Replace: `DRUPAL_DEFAULT_SOLR_CORE=ISLANDORA`
4. Copy `.env`
```
cp -r /path/to/isle/dc/codebase/.env .
```
5. Update `.env`
Find: `TAG=`
Replace: `ISLANDORA_TAG=`

These deprecated environment variables can be removed from your `.env` that was copied from your ISLE-DC install

??? Deprecated environment variable names
    ```
    ACME_KEY_TYPE
    ACTIVEMQ_MEMORY_LIMIT
    ACTIVEMQ_PORT
    ALPACA_FITS_TIMEOUT
    ALPACA_HOMARUS_TIMEOUT
    ALPACA_HOUDINI_TIMEOUT
    ALPACA_MEMORY_LIMIT
    ALPACA_OCR_TIMEOUT
    BLAZEGRAPH_MEMORY_LIMIT
    BLAZEGRAPH_PORT
    CANTALOUPE_DELEGATE_SCRIPT_ENABLED
    CANTALOUPE_DELEGATE_SCRIPT_PATHNAME
    CANTALOUPE_HTTPSOURCE_LOOKUP_STRATEGY
    CANTALOUPE_MEMORY_LIMIT
    CODEBASE_PACKAGE
    COMPOSE_HTTP_TIMEOUT
    CRAYFITS_MEMORY_LIMIT
    CUSTOM_IMAGE_NAME
    CUSTOM_IMAGE_NAMESPACE
    CUSTOM_IMAGE_TAG
    DISABLE_SYN
    DRUPAL_DATABASE_SERVICE
    DRUPAL_INSTALL_PROFILE
    DRUPAL_MEMORY_LIMIT
    ENVIRONMENT
    EXPOSE_ACTIVEMQ
    EXPOSE_BLAZEGRAPH
    EXPOSE_CANTALOUPE
    EXPOSE_DRUPAL
    EXPOSE_FEDORA
    EXPOSE_MYSQL
    EXPOSE_POSTGRES
    EXPOSE_SOLR
    EXPOSE_TRAEFIK_DASHBOARD
    FCREPO_DATABASE_SERVICE
    FCREPO_MEMORY_LIMIT
    FEDORA_6
    FEDORA_PORT
    FITS_MEMORY_LIMIT
    HOMARUS_MEMORY_LIMIT
    HOUDINI_MEMORY_LIMIT
    HYPERCUBE_MEMORY_LIMIT
    IDE_MEMORY_LIMIT
    INCLUDE_ETCD_SERVICE
    INCLUDE_TRAEFIK_SERVICE
    INCLUDE_WATCHTOWER_SERVICE
    INSTALL_EXISTING_CONFIG
    MARIADB_MEMORY_LIMIT
    MILLINER_MEMORY_LIMIT
    MYSQL_PORT
    PHP_MAJOR_VERSION
    PHP_MAX_EXECUTION_TIME
    PHP_MEMORY_LIMIT
    PHP_MINOR_VERSION
    PHP_POST_MAX_SIZE
    PHP_UPLOAD_MAX_FILESIZE
    POSTGRES_PORT
    PROJECT_DRUPAL_DOCKERFILE
    PUBLIC_FILES_TAR_DUMP_PATH
    RECAST_MEMORY_LIMIT
    RESTART_POLICY
    SITE
    SOLR_MEMORY_LIMIT
    SOLR_PORT
    TAG
    TRAEFIK_DASHBOARD_PORT
    TRAEFIK_LOG_LEVEL
    TRAEFIK_MEMORY_LIMIT
    USE_ACME
    USE_SECRETS
    WATCHTOWER_MEMORY_LIMIT
    ```
6. Bring in the changes to your ISLE-DC git repo

```
rm -rf .git
cp -r path/to/isle/dc/.git .
git checkout -B convert-site-template
```

#### 1.b ISLE Site Template

In your dev environment, bring in the new config from ISLE Site Template

```
cd path/to/your/site/template
git checkout -B convert-site-template
git remote add ist https://github.com/islandora-devops/isle-site-template
git fetch ist
if [ -f Makefile ]; then cat Makefile >> custom.Makefile; fi
git checkout ist/main -- conf Makefile sample.env scripts docker-compose.dev.yml
docker compose --profile dev down
make down
```

### 2 - Remove compose profiles (ISLE Site Template only)

If you're converting from ISLE-DC you can skip this step.

You have two options to convert docker-compose. Choose either 2.a or 2.b

#### 2.a - Vanilla overwrite

If you've made no changes to docker-compose.yml since you made your own copy of isle-site-template you can simply overwrite the docker-compose.yml with the latest version 

```
git checkout ist/main -- docker-compose.yml
```

#### 2.b - Attempt to merge

If you've made changes to docker-compose.yml since copying the repo, you can attempt to convert it using `sitectl`. Install sitectl and run the `sitectl isle migrate merge-compose-profiles` command

```
brew tap libops/homebrew https://github.com/libops/homebrew
brew install libops/homebrew/sitectl-isle
sitectl isle migrate merge-compose-profiles -i docker-compose.yml -o docker-compose.yml -f
```

This will overwrite your `docker-compose.yml` with the new profiles-less version.

### 3 - Review

Very carefully review your changes locally

```
git diff
```

Or if you prefer viewing the diff in your git provider

```
git add .
git commit -m "Removing profiles"
git push origin convert-site-template
```

Ensure your services have the same environment variables, secrets, volumes your old services had. 

Familiarize yourself with the new make command helpers

```
make help
```

### Environment Variables

Ensure the new values you see in `sample.env` are set in your `.env`. Namely

```
URI_SCHEME=http
TLS_PROVIDER="self-managed"
ACME_URL=https://acme-v02.api.letsencrypt.org/directory
ACME_EMAIL=postmaster@example.com
```

In the git diff, compare the `environment` sections of your docker-compose.yml files. You may have some variables set in your ISLE-DC docker-compose.yml that will need to be added to the docker-compose.yml in the new Site Template site.

For example, the [ISLE Buildkit Nginx README](https://github.com/Islandora-Devops/isle-buildkit/blob/main/nginx/README.md) lists the available variables for Nginx that you may have changed on your Drupal containers to do things like increase the timeout time for PHP or the max POST size PHP will accept.

And then adjust sample.env as needed.

## Converting the Makefile

Because ISLE-DC and Site Template use the same containers, much of what is in an ISLE-DC Makefile will work within the site template environment. The main differences that need to be addressed are with the way new sites are built, and the name of the containers. 

Commands to build a site, like `make starter` and `make production` are not necessary with the Site Template, so they can be removed from your Makefile.

Many of the other commands specify which containers to run commands in, for example `docker compose exec drupal`.

## Other Customizations

If you have modified your repository, those modifications will need to be evaluated on a case-by-case basis. This includes editing the Dockerfile and environment variables mentioned above, as well as any other customizations you may have made, such as to settings.php, robots.txt, nginx configs, etc.

### 4 - Test

If you want to keep running TLS in your dev environment you can run this make helper to switch to `https`:

```
make traefik-https-mkcert
```

Otherwise, be sure your `DOMAIN` value in `.env` is `islandora.traefik.me` and be aware your dev site is available at http://islandora.traefik.me

Now, bring up your services. In a development environment, after running `make up` your ISLE site should be available at `${URI_SCHEME}://${DOMAIN}` (the actual value will print after running make up depending on your settings).

!!! warning

    If you don't like the new setup, it's not working, or the changes didn't apply cleanly, you can [reach out for help](https://www.islandora.ca/contact-us#comms-channels) (in the #isle slack channel) or simply delete the git branch that was created to revert the changes.

    This will delete any config settings made in previous steps
    ```
    make down
    git checkout main
    git branch -D convert-site-template
    ```

### 5 - Save changes and cleanup

If you like this new setup and all looks well, merge the `convert-site-template` git branch into main

```
git checkout main
git merge convert-site-template
git push origin main
```

### 6 - Deploy to production

If you committed the changes, deploy to production by:

Pull down the changes

```
git pull origin main
```

If you're using letsencrypt run this make helper to switch to `https` in your prod environment:

```
make traefik-https-letsencrypt
```

If you're not using letsencrypt but instead are using your own TLS certs, make sure `.env` has

```
URI_SCHEME=https
TLS_PROVIDER="self-managed"
```

Next, bring down the site, pull in the changes, and bring everything back up

```
make down
docker compose --profile prod down
make up
```

Your site should be online now using the new setup!
