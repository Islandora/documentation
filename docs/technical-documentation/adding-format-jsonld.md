Drupal requires the use of a `_format` query parameter to get alternate representations of a node/media.

By default, Islandora deploys with the [jsonld](https://github.com/Islandora/jsonld) module and the [Milliner](https://github.com/Islandora/Crayfish/tree/main/Milliner) microservice. These two components are configured to strip this `_format` query parameter off of the end of URIs.

This means that when your content is indexed in Fedora, the triplestore, etc... it's URI will
be something like `http://islandora.traefik.me/node/1` and not `http://islandora.traefik.me/node/1?_format=jsonld`.


## Adding ?_format=jsonld to your URIs

To turn the `?_format` parameter back on:

- Go to `admin/config/search/jsonld` and confirm the *"Remove jsonld parameter from @ids"* checkbox is **unchecked**.
- Add `strip_format_jsonld: false` to your Milliner config. If you deployed using the default Islandora-playbook this file would be located at `/var/www/html/Crayfish/Milliner/cfg/config.yaml`.

If you are using [Islandora-playbook](https://github.com/Islandora-Devops/Islandora-playbook) and are provisioning new environments for your older Islandora, you'll want to lock down the variables in your inventory that control this config.

- `crayfish_milliner_strip_format_jsonld: true`
- `webserver_app_jsonld_remove_format: 1`
