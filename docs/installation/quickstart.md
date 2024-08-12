# Quickstart

There are several ways to try Islandora, which are presented here in order of increasing sophistication. By default, they all install the [Islandora Starter Site](https://github.com/Islandora-Devops/islandora-starter-site) which is a starting place for customizing your own site, as well as our suite of external services.

## Online Sandbox

Try Islandora without installing anything at [sandbox.islandora.ca](https://sandbox.islandora.ca/).
[Login credentials](https://github.com/Islandora/documentation/wiki/Sandbox.Islandora.ca-online-credentials) for the sandbox can be found on our Wiki.

Anyone can log in to this sandbox as an administrator and explore the interface! However, this site is refreshed every night so your changes will not be permanent. This site uses the Islandora Starter Site. This sandbox includes some sample content and configuration (such as views and blocks) to increase its usefulness as a sandbox. .

## Docker Portainer Demo

Alternately, with Docker Desktop, you can run a demo locally using Docker's Portainer extension. The command line is not required. See [Docker Portainer Demo](install-a-demo.md) for instructions. Note that this method does not support installing modules or themes that are not included with the demo.

## Sandbox running locally

Alternately, you can use Docker and install the Online Sandbox repository locally, with minimal command-line usage. Note that this method does not support installing modules or themes that are not included with the demo. See the instructions for "Running Locally" in the [Sandbox's README](https://github.com/Islandora-Devops/sandbox).
  
## Ansible Playbook

To provision a local Vagrant or remote Ubuntu virtual machine (without Docker), you can use the [Islandora Ansible Playbook](https://github.com/Islandora-Devops/islandora-playbook). The playbook results in all services installed on a single machine, but can be altered to spread services across various machines. This is a full-fledged VM where you can install modules and themes using Composer. This method requires basic command-line usage and it's advantageous if you are familiar with provisioning software on Ubuntu. This Playbook is suitable for local or production use, though local use (through VirtualBox and Vagrant) is not supported yet by Apple hardware (i.e. M1/M2 machines). See documentation: [Installation - Ansible Playbook](playbook.md) for more details.

## ISLE Site Template 

[ISLE Site Template](https://github.com/Islandora-Devops/isle-site-template) uses Docker and is based off images created by [ISLE Buildkit](https://github.com/Islandora-Devops/isle-buildkit), but is a simpler tool than ISLE-DC. This is a full-fledged Docker installation where you can install modules and themes using Composer, either by executing commands in the container or by using the built-in IDE.  It is suitable for local development or production. See documentation: [Installation - Site Template](docker/site-template/site-template.md) for more details. 

## ISLE-DC

[ISLE-DC](https://github.com/Islandora-Devops/isle-dc) uses Docker and provisions each service in the Islandora stack in a separate container. The containers are also based off of the images in ISLE Buildkit. ISLE-DC uses the [GNU Make](https://www.gnu.org/software/make/) tool to provide several shortcuts to performing common management functions. It is suitable for local development or production.  See documentation: [Installation - Docker ISLE](docker/isle-dc/docker-local.md) for more details.


