# Docker Compose (ISLE)

The ISLE Islandora Enterprise 8 Prototype ([Islandora-Devops/isle-dc](https://github.com/Islandora-Devops/isle-dc)) is a Docker project to manage Islandora 8 infrastructure under Docker using Docker Compose. These instructions cover the basics of using Docker locally to create a development Islandora sandbox. More detailed configuration options are described in the project's [README](https://github.com/Islandora-Devops/isle-dc).

## Why use Docker?

Docker is a way to separate out the "state" of your site (i.e. all the content, files, and configurations that you've entered) from the underlying software that runs it (e.g. webserver, database engine, etc). This allows for easier upgrades, faster development, and more flexible deployment. The ISLE project, run by the Islandora Collaboration Group (ICG) was the driving force behind dockerizing Islandora.

## Requirements

* Docker (version 19.x+)

To see if you have Docker installed, type `docker --version` in a shell.

## Installing Docker

If you are installing Docker, we recommend using the application [Docker Desktop](https://www.docker.com/products/docker-desktop). It provides a GUI for managing Docker container in Windows and MacOS, along with the Docker engine and suite of command-line tools. Linux users can download the Engine and command-line tools from that same link.

There is also a legacy project called [Docker Toolbox](https://docs.docker.com/toolbox/overview/) which may be of interest if your machine cannot run Docker Desktop, or if you already have it installed.


[Download Docker](https://www.docker.com/products/docker-desktop)


## Launching Islandora with Docker

In a shell, clone the isle-dc project. In that directory, enter the command `make` (or `make dev` - see note) to build the docker infrastructure. Then use the command `docker-compose up -d` to start the containers.

!!! Note
    The `make` command alone will spin up a sandbox-like version of Isle on the front end, but the code files will be inaccessible. The `make dev` command will also copy the active code files locally in a way that they are live to the Isle site. This method takes longer, but is required if you will be testing pull requests or writing code.

```bash
clone https://github.com/islandora-devops/isle-dc
cd isle-dc
make
```


Results:
```
isle-dc$ make
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  3562  100  3562    0     0  12779      0 --:--:-- --:--:-- --:--:-- 12812
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  1708  100  1708    0     0   6809      0 --:--:-- --:--:-- --:--:--  6832
docker-compose pull
Pulling activemq   ... done
Pulling alpaca     ... done
Pulling blazegraph ... done
Pulling cantaloupe ... done
Pulling fcrepo     ... done
Pulling fits       ... done
Pulling crayfits   ... done
Pulling gemini     ... done
Pulling homarus    ... done
Pulling houdini    ... done
Pulling hypercube  ... done
Pulling mariadb    ... done
Pulling matomo     ... done
Pulling milliner   ... done
Pulling recast     ... done
Pulling solr       ... done
Pulling drupal     ... done
Pulling traefik    ... done
Pulling watchtower ... done
```

!!! Troubleshooting
    If you get an error such as: `ERROR: Version in "./docker-compose.activemq.yml" is unsupported.`, then you need to upgrade Docker, and then enter the command `make clean`. If you forget to `make clean`, then the next time `make` runs you may see an error such as: `ERROR: Top level object in './docker-compose.yml' needs to be an object not '<class 'NoneType'>'.`

!!! Troubleshooting
    If you get an error such as: `make[1]: *** [create-codebase-from-demo] Error 143 make: *** [dev] Error 21` ... documentation to come.


Once `make` has successfully completed, launch the Isle containers using `docker-compose up`. The `-d` flag allows you to return to using the command line. Without it, your shell will be stuck in the `docker-compose` process as long as the containers are running.


```bash
docker-compose up -d
```

## Visiting your Islandora site

Direct a browser to [https://islandora-isle-dc.traefik.me/](https://islandora-isle-dc.traefik.me/) (yes, it's awkward and we hope to change it to something easier to remember). Right after spinning up the Docker containers, you will probably see a white screen with the words "Bad Gateway". This will last until all the containers are successfully up and ready to go. This often takes 2-5 minutes. In theory, it should be longer the first time when creating the database etc; and subsequent times (as long as you don't destroy your state), it should be faster. When all containers are ready, you should see a basic Drupal login screen.

!["Bad Gateway" white screen while still loading](../assets/docker_bad_gateway_still_loading.png)

![Drupal login screen](../assets/docker_drupal_login_screen.png)

To log in:

* username: **admin**
* password: **password**

!!! Note Docker logs and Docker Compose logs
    `docker logs -tf isle-dc_drupal_1` shows Docker logs for the "drupal" container, which will continue ot have new log entries as long as the drupal box is still starting up. When it gets to `confd using 'env' backend`, you're done. `docker-compose logs` is like a firehose, showing the log messages from all containers.

## Spinning down your Islandora site

To shut down the containers without destroying your site, use `docker-compose down`. To also destroy your "state" (i.e. your content, your database, your files), use `docker-compose down -v`.

## Editing Code

Documentation to come.

## Testing a Pull Request

Documentation to come, but you have to have done `make dev`. If you just did `make`, you will need to destroy your state before starting a new one with `make dev`.

## Updating Isle

[Roughly, according to Danny]. If you're not doing "anything special" and want new stuff, type `docekr-compose pull`, docker-compose up -d` you don't even have to take things down. (Does this apply regardless of whether you did `make` or `make dev`? 

If you have your own stuff, do that but also 'make a new drupal container' - that's what isle-dc will turn into... when you have a site and you export "the tree" i.e. all the files starting where your composer file is, e.g.  /var/www/html/drupal - put that in a folder in your codebase, then when you type `make` it uses that exported drupal site as your container. There are some subtleties if you have to change crayfish or alpaca.

To add a new module, go into your codebase folder, use composer to require the module, then "make a new container out of all of this". (This requires composer installed on your host, right? is there a specific version we need?)


