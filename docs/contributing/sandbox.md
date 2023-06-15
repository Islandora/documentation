# Updating the Sandbox

!!! Note 
    This page is about the online sandbox, https://sandbox.islandora.ca. If you are looking to set up your own site, see the Installation section.

The sandbox at https://sandbox.islandora.ca is built, configured, and deployed, by the [https://github.com/Islandora-Devops/sandbox](https://github.com/Islandora-Devops/sandbox) repository. This build process happens nightly from the Sandbox's latest release.

## Maintaining the front-end site

The Sandbox makes use of the following components, which are version-locked in the `drupal/Dockerfile` file:

* Islandora Workbench
* Islandora Demo Objects
* Islandora Starter Site

In order to update them, you need to update the desired `XXX_COMMIT` hash; compute and add a sha-256 checksum; and then cut a release of the Sandbox repository. It will be deployed to the production site overnight. See full instructions at the [Sandbox README](https://github.com/Islandora-Devops/sandbox).

Note that some of these components will need to be updated together, for example a new column in the Demo Objects may require a new field in the Starter Site. 

## Maintaining the back-end containers

The containers used in the Sandbox are specified by the `ISLANDORA_TAG` value in the `.env` file. 
