Drupal requires the use of a `_format` query parameter to get alternate representations of a node/media.

By default, Islandora deploys with the [jsonld](https://github.com/Islandora/jsonld) module and the [Milliner](https://github.com/Islandora/Crayfish/tree/master/Milliner) microservice. These two components are configured to strip this `_format` query parameter off of the end of URIs.

This means that when your content is indexed in Fedora, the triplestore, etc... it's URI will
be something like `http://localhost:8000/node/1` and not `http://localhost:8000/node/1?_format=jsonld`.

## Pre-1.0 installations.

If you are using a __very__ early version of Islandora 8 (pre-release), then you may have URIs with `_format=jsonld` at the end of them.

If you update to newer code, you will need to ensure that your site is configured to add `?_format=jsonld`
back to the URLs if you want to maintain consistency.

If you **don't** do this, you can end up with two copies of your objects in your Fedora repository (one with and one without `?_format=jsonld`). You will also have two sets of triples in your triplestore.

## Adding ?_format=jsonld to your URIs

To turn the `?_format` parameter back on:

- Go to `admin/config/search/jsonld` and confirm the *"Remove jsonld parameter from @ids"* checkbox is **unchecked**.
- Add `strip_format_jsonld: false` to your Milliner config. If you deployed using the default Islandora-playbook this file would be located at `/var/www/html/Crayfish/Milliner/cfg/config.yaml`.

If you are using [Islandora-playbook](https://github.com/Islandora-Devops/Islandora-playbook) and are provisioning new environments for your older Islandora 8, you'll want to lock down the variables in your inventory that control this config.

- `crayfish_milliner_strip_format_jsonld: true`
- `webserver_app_jsonld_remove_format: 1`
