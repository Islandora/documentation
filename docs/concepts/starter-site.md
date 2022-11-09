# Islandora Starter Site

The [Islandora Starter Site](https://github.com/Islandora/islandora-starter-site/) is an out-of-the-box deployment of Islandora. It is a complete exported Drupal site, that makes use of the Islandora modules and
configures them in a way that is illustrative and useful.

- For evaluators, it is intended to show off the features and capabilities of Islandora.
- For interest groups, it is intended to be a place to develop solutions to shared problems.
- For site builders, it is intended to be a starting point for configuring a site.

The Islandora Starter Site contains no code, only references to other modules and lots of Drupal configuration. A very motivated person could re-create the Starter Site just by installing and configuring modules. This means there's nothing tying you to using the Starter Site. There's also nothing tying you to doing things in a particular way. Also, it means that you won't be getting any "updates" - there's no code to update.

To experience the full Islandora Starter Site, it requires access to external services such as Solr,
Fedora, Alpaca, and Matomo. It is therefore suggested to deploy the Starter Site using one of our
two deployment platforms: [ISLE-DC](../../installation/docker-local) (using the `make starter` or `make starter_dev` commands), or
the [Islandora Playbook](../../installation/playbook) (using the `starter` (default) or `starter_dev` option in the Vagrantfile).

