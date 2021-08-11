## Islandora 8 Migration Overview

This [video](https://www.youtube.com/watch?v=95Bnix-z1zY) (Aug 10, 2020) provides an overview of the various options available to migrate data into an Islandora 8 installation.

[![Islandora Online: Islandora Migration Tools](https://img.youtube.com/vi/95Bnix-z1zY/0.jpg)](https://www.youtube.com/watch?v=95Bnix-z1zY)

The three main migration options are:

- [REST API](migration-rest-api.md)
- [Migrate API](migration-migrate-api.md)
    - migrate_islandora_csv
    - migrate_7x_claw
- [Islandora Workbench](migration-islandora-workbench.md)


### REST API

Why use the rest API?

- **Works anywhere**: You don’t have to work on the Drupal server. Migrate from your laptop!
- **No PHP required**: Use any language that can make an http request. Even cURL will do just fine.
- **JSON**: Why use XML if you don’t have to?
- Relies on Drupal’s own REST API

Visit the [REST API](migration-rest-api.md) migration documentation section for more details.


### Migrate API

Why use the Migrate API?

- You can (potentially) do everything with configs!
- Leverage contrib module plugins.
- Making plugins for more complex sources and processes is (relatively) simple.
- Updating metadata is as simple as:
`drush mim node --update`

Two tools that use the Migrate API are [migrate_islandora_csv](migrate-csv.md) and [migrate_7x_claw](migrate-7x).

Visit the [Migrate API](migration-migrate-api.md) migration documentation section for more details.


### Islandora Workbench

Why use the Migrate API?

- More tailored for end users with less technical knowledge or limited server access.
- Uses Islandora’s REST API
- Runs on your computer
- “CSVs and a pile of scans”
- Cross Platform - Python

Visit the [Islandora Workbench](migration-islandora-workbench.md) migration documentation section for more details.
