By default, Islandora deploys with the `jsonld` module and `Milliner` microservice set to strip Drupal's Symfony-style
`_format` query parameter.  This means that when your content is indexed in Fedora, the triplestore, etc... it's URI will
be something like `http://localhost:8000/node/1` and not `http://localhost:8000/node/1?_format=jsonld`.

If you are using a __very__ early version of Islandora 8 (pre-release), then you may have uris with `_format=jsonld` at the
end of them.  If you update to newer code, you will need to ensure that your site is configured to add `?_format=jsonld`
back to the urls if you want to maintain consistency.

- Go to `admin/config/search/jsonld` and confirm the 'Remove jsonld parameter from @ids' checkbox is unchecked.
- Add `strip_format_jsonld: false` to `/var/www/html/Crayfish/Milliner/cfg/config.yaml`

If you are using claw-playbook and are provisioning new environments for your older Islandora 8, you'll want to lock down the
variables in your inventory that control this config.

- `crayfish_milliner_strip_format_jsonld: true`
- `webserver_app_jsonld_remove_format: 1`
