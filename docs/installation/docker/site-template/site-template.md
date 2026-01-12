# ISLE Site Template

## What is the ISLE Site Template?

The [ISLE Site Template][ISLE Site Template] is a system for installing
Islandora on Docker. As with ISLE-DC, it uses [Docker Compose][Docker Compose]
to orchestrate the installation of all the different services (Docker
containers) that make up Islandora. Unlike ISLE-DC, in ISLE Site Template you
use Docker Compose commands directly, helping you to get familiar with the
kinds of commands that will be a key part of running and maintaining Islandora. 

## Usage

1. **Use the GitHub template feature**
    * Click the green "Use this template" button on the [ISLE Site Template][ISLE Site Template]
      repository to create your own copy. This creates a new repository in your GitHub account
      with the same directory structure and files, including a pre-installed copy of the
      [Islandora Starter Site][Islandora Starter Site].

2. **Clone your new repository**
    * After creating your repository from the template, clone it to your local machine or server.

3. **Quick start with make commands**
    * The template now uses a `Makefile` to simplify setup. Just run `make up` to initialize
      and start your Islandora site. This handles generating secrets, certificates,
      and bringing up all services.

4. **Default configuration**
    * By default, the environment runs over HTTP at `islandora.traefik.me` (which resolves to
      127.0.0.1) for easier local development. You can switch to HTTPS using make commands
      for development (`make traefik-https-mkcert`) or production (`make traefik-https-letsencrypt`).

5. **Customizing your site**
    * All customizations you make can be committed to your repository, including Docker
      configuration, service settings, Drupal modules, themes, and site configuration. Your
      custom Drupal codebase is part of the repository and gets built into a custom Docker
      image. 

[ISLE Site Template]: https://github.com/Islandora-Devops/isle-site-template
[Docker Compose]: https://docs.docker.com/compose/ 
[Islandora Starter Site]: https://github.com/Islandora-Devops/islandora-starter-site
