# Islandora CLAW

## Summary

<a href="https://islandora.ca">Islandora</a> is an open-source repository framework, that combines a Drupal front-end
with a Fedora repository. It is a solution for institutions who want an extremely flexible and configurable preservation repository without creating a lot of custom code.

Islandora CLAW integrates [Drupal](https://www.drupal.org/) (currently, Drupal 8) and Duraspace's [Fedora Repository](https://wiki.duraspace.org/display/FF/Fedora+Repository+Home) project (currently, Fedora 5). Islandora CLAW is currently in beta. (todo: release section?)

Islandora CLAW allows you to create nodes, media, files, and taxonomy terms in Drupal, which are converted to RDF
and pushed into a Fedora repository. It allows you to configure derivatives, automatic processes that transform files to other types for display or preservation, or extract additional metadata.

## Installation

Islandora CLAW is installed through an Ansible Playbook called [claw-playbook](https://github.com/Islandora-Devops/claw-playbook).
With Git, Vagrant, and Ansible installed, you can spin up a local development environment with
```bash
git clone https://github.com/Islandora-Devops/claw-playbook
cd claw-playbook
vagrant up
```
See the Installation section for more information.
