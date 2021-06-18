# Installing a Demo Server

Using ISLE, you can spin up a repository that is exactly like future.islandora.ca, including the sample content. If you want to kick the tires and see what Islandora can do with the minimal amount of setup, this is for you.

!!! Warning "Demonstration Purposes Only!"
    Please be advised, the environment you are about to create is meant to be temporary. The drupal codebase is baked into a container and is ephemeral.  If you [install new modules](../docker-maintain-drupal/), they will be gone if your drupal container goes down for any reason.

## Getting Started

To get started with a **demo** environment, run the following command from your `isle-dc` directory:

```bash
make demo
```

This will pull down images from Dockerhub and generate

| File                 | Purpose                                                                                                                                                                                                                                                                     |
| :------------------- | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `.env`               | A configuration file that is yours to customize. This file controls how the docker-compose.yml file gets generated to meet your use case.</br>It also allows you to set variables that make their way into the final `docker-compose.yml` file, such as your site's domain. |
| `docker-compose.yml` | A ready to run `docker-compose.yml` file based on your `.env` file.  This file is considered disposable. When you change your `.env` file, you will generate a new one.                                                                                                     |

## Kicking the Tires

Your new Islandora instance will be available at [https://islandora.traefik.me](https://islandora.traefik.me). Don't let the
funny url fool you, it's a dummy domain that resolves to `127.0.0.1`.

You can log into Drupal as `admin` using the default password, `password`. 

Enjoy your Islandora instance!  Check out the [Islandora documentation](https://islandora.github.io/documentation) to see all
the things you can do.  If you want to poke around, here's all the services that are available to visit:

| Service     | Url                                                                                            |
| :---------- | :--------------------------------------------------------------------------------------------- |
| Drupal      | [https://islandora.traefik.me](https://islandora.traefik.me)                                   |
| Traefik     | [https://islandora.traefik.me:8080](https://islandora.traefik.me:8080)                         |
| Fedora      | [https://islandora.traefik.me:8081/fcrepo/rest](https://islandora.traefik.me:8081/fcrepo/rest) |
| Blazegraph  | [https://islandora.traefik.me:8082/bigdata](https://islandora.traefik.me:8082/bigdata)         |
| Activemq    | [https://islandora.traefik.me:8161](https://islandora.traefik.me:8161)                         |
| Solr        | [https://islandora.traefik.me:8983](https://islandora.traefik.me:8983)                         |
| Cantaloupe  | [https://islandora.traefik.me/cantaloupe](https://islandora.traefik.me/cantaloupe)             |
| Matomo      | [https://islandora.traefik.me/matomo/](https://islandora.traefik.me/matomo/)                   |
| Code Server | [https://islandora.traefik.me:8443/](https://islandora.traefik.me:8443/)                       |

## Shutting down

When you're done with your demo environment, shut it down by running

```bash
docker-compose down
```

This will keep your ingested data around until the next time you start your instance.  If you want to completely destroy the repository and 
all of its content, use

```
docker-compose down -v
```

