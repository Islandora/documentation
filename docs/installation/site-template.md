# ISLE Site Template

## What is the ISLE Site Template?

The [ISLE Site Template][ISLE Site Template] is a system for installing
Islandora on Docker. As with ISLE-DC, it uses [Docker Compose][Docker Compose]
to orchestrate the installation of all the different services (Docker
containers) that make up Islandora. Unlike ISLE-DC, in ISLE Site Template you
use Docker Compose commands directly, helping you to get familiar with the
kinds of commands that will be a key part of running and maintaining Islandora. 

## Usage

1. **Do not clone the Isle Site Template!**
    * Unlike most other repositories we provide, the Isle Site Template is not
      meant to be cloned or forked. Rather, it can be downloaded using `curl`
and installed either manually or automatically.

2. Instead, follow the instructions in the ISLE Site Template's `README.md` and
`README.template.md` files.

    * Instructions are provided both for `dev` and `prod` environments, with
      different services available on each.

3. During installation, you will install a copy of the [Islandora Starter
Site][Islandora Starter Site].
    * Though, if you select the manual installation option, you can change that
      out for a different base composer project. This will form the basis of
your Drupal site. If you don't have a custom version, we recommend using the
Islandora Starter Site (and it's installed automatically during the automatic
install).

4. Customizing your site can be persisted to your own repo.
    * In the process of setting you the ISLE Site Template, you are encouraged
      to create a custom Git repository for this project. When you do, you can
save your changes to several components of your own site, for example the site
name in Docker, which services you have running, and all changes made to your
entire Drupal site configuration. 

[ISLE Site Template]: https://github.com/Islandora-Devops/isle-site-template
[Docker Compose]: https://docs.docker.com/compose/ 
[Islandora Starter Site]: https://github.com/Islandora-Devops/islandora-starter-site
