# Converting ISLE-DC to Site Template

ISLE-DC is being deprecated in favour of using the Isle Site Template for Docker installs of Islandora. The following instructions explain how to convert your ISLE-DC site into the Site Template style of docker-compose.yml.

## Development Instructions

ISLE-DC and the Site Template both use [isle-builkit](https://github.com/Islandora-Devops/isle-buildkit) images to spin up an Islandora site. Since the images are the same, you can transfer an ISLE-DC site to a Site Template setup by doing the following:

1. Follow the instructions in the [Site Template README](https://github.com/Islandora-Devops/isle-site-template) to create a new site from the Islandora Starter Site

    !!! note
        Use the same TAG in your Site Template .env that you used in ISLE-DC, so you have the same set of containers.

2. Replace the Starter Site's composer.json and composer.lock files with yours, and install the correct Drupal modules using Composer

    !!! note
        You may need to make some modifications to your composer.json file. Specifically, the `scripts` and `drupal-scaffold` sections may have changed.
    
        You can look at the [most recent starter site composer.json](https://github.com/Islandora-Devops/islandora-starter-site/blob/main/composer.json) for comparison.

        If you have custom modules and/or themes that are not installed via Composer you will need to install those as well.

3. Compare your old Dockerfile with [the one in the Site Template repository](https://github.com/Islandora-Devops/isle-site-template/blob/main/drupal/Dockerfile). If you have made customizations, you may want to replicate them in the new Dockerfile.

4. Backup your ISLE-DC site's Fedora, Drupal database, and public/private files.

5. Import your backups to the new Site Template site.

6. Delete your Solr core, and regenerate new configs.

    !!! note
        Site Template uses a different core name than ISLE-DC did

7. Commit and push your git repository so it is ready for production.


## Production Instructions

Once you have converted the development instance of your site, moving it to production requires the following:

1. Clone the git repository that you set up in the development instructions above

2. Prepare your images/containers as described in the Site Template README

    1. Run the generate secrets script included with the site template, or copy your secrets from your existing site if you want them to be the same
    2. Add custom modules or themes, if you have some that are not included in your composer.json
    3. Build your Drupal container with `docker compose --profile prod build`
    4. Pull in the remaining containers with `docker compose --profile prod pull --ignore-buildable --ignore-pull-failures`
    5. Start the containers with `docker compose --profile prod up -d`

3. Import the backups from your ISLE-DC site that you made in the development instructions above

4. Delete your Solr core, and regenerate new configs.

## Converting Your docker-compose.yml

The main difference between ISLE-DC and the Site Template is the way the docker-compose.yml file is generated. In ISLE-DC we generate it based on your .env variables and a make command, but in Site Template it is ready to go out of the box. 

This means that in ISLE-DC you had separate docker-compose.yml files for development and production, but in site template there is one file for both, which contains instructions for a “dev” and “prod” profile. The end result is the same set of containers, but instead of running `docker compose up` you would run `docker compose --profile dev up`

### Environment Variables

You should compare the `environment` sections of your docker-compose.yml files. You may have some variables set in your ISLE-DC docker-compose.yml that will need to be added to the docker-compose.yml in the new Site Template site.

For example, the [ISLE Buildkit Nginx README](https://github.com/Islandora-Devops/isle-buildkit/blob/main/nginx/README.md) lists the available variables for Nginx that you may have changed on your Drupal containers to do things like increase the timeout time for PHP or the max POST size PHP will accept.

### URLs

The Site Template also handles the URLs for microservices differently. You will need to follow the instructions in the Site Template README to allow access to your microservices as a subdomain. For example, `solr.mydomain.com`. 

!!! Note "Subdomains"
    If you are already using a subdomain and can’t use URLs like `solr.mysubdomain.mydomain.com`, you can change this in the Traefik section of the docker-compose.yml file, by changing the aliases from `solr.{DOMAIN}` to something else, like `solr-{DOMAIN}`. You will also need to change this in a few other spots in your docker-compose.yml. You can find them by searching for `.{DOMAIN}`

## Converting the Makefile

Because ISLE-DC and Site Template use the same containers, much of what is in an ISLE-DC Makefile will work within the site template environment. The main differences that need to be addressed are with the way new sites are built, and the name of the containers. 

Commands to build a site, like `make starter` and `make production` are not necessary with the Site Template, so they can be removed from your Makefile.

Many of the other commands specify which containers to run commands in, for example `docker compose exec drupal`, but in the Site Template environment you need to specify -dev or -prod, so that would be changed to `docker compose exec drupal-dev` or `docker compose exec drupal-prod`.

## Other Customizations

If you have modified your ISLE-DC repository, those modifications will need to be evaluated on a case-by-case basis. This includes editing the Dockerfile and environment variables mentioned above, as well as any other customizations you may have made, such as to settings.php, robots.txt, nginx configs, etc.



