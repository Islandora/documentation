## Islandora 8 Migration Overview

This video provides an overview of the various options available to migrate data into an Islandora 8 installation.

[![Islandora Online: Islandora Migration Tools](https://img.youtube.com/vi/95Bnix-z1zY/0.jpg)](https://www.youtube.com/watch?v=95Bnix-z1zY)

The three options are:
- REST API
- Migrate API
- Islandora Workbench

### REST API

Why use the rest API?
- Works anywhere
  - You don’t have to work on the Drupal server. Migrate from your laptop!
- No PHP required
  - Use any language that can make an http request. Even cURL will do just fine.
- JSON
  - Why use XML if you don’t have to?

BONUS: It’s just Drupal’s REST API

#### Islandora only provides two additional API endpoints

- /media/{mid}/source
  - PUT a file to this endpoint to create/update a Media’s file
- /node/{nid}/media/{media_type}/{taxonomy_term}
  - PUT a file to this endpoint to create/update a Media for a Node

Just be aware, you are writing everything yourself! (In other words you are making all of the migration decisions yourself.)

### Migrate API

Uses the Drupal 8 [Migrate API](https://www.drupal.org/docs/8/api/migrate-api/migrate-api-overview), which "provides services for migrating data from a source system to Drupal 8.

The "source system" can be almost anything:
- an Islandora 7 system
- a group of scanned images and their metadata inside a CSV file
- a web API

Why use the Migrate API?

- You can (potentially) do everything with configs!
- Leverage contrib module plugins.
- Making plugins for more complex sources and processes is (relatively) simple.
- Updating metadata is as simple as:
`drush mim node --update`

#### A Migration Configuration defines an Extract, Transform, Load (ETL) process

- Source plugins extract data from a source
- Process plugins transform the data
- Destination plugins load the data (create new entities)

#### We’ve built two tools for you using the Migrate API

- [migrate_islandora_csv](https://github.com/Islandora/migrate_islandora_csv)
  - Tutorial with a sample migration using some files and a CSV
- [migrate_7x_claw](https://github.com/Islandora-Devops/migrate_7x_claw)
  - A tool to get all your Islandora 7 content migrated over

##### Recap of migrate_islandora_csv
- CSVs
  - Everyone understands and knows how to work with CSVs
- Documented
  - It’s a step-by-step walkthrough
- Process Metadata
  - Clean up / transform the metadata using processors
- Build Relationships
  - Migrations can reference other migrated content or generate new content on the fly

##### Recap of migrate_7x_claw

- Designed to migrate Islandora 7 data to Islandora 8.
- DATASTREAMS
  - All of your datasteams, including the audit trail, are migrated
- METADATA
  - Migrate metadata from Solr or any XML datastream
- CUSTOMIZABLE
  - Migrate_7x_claw is a starting point, meant to be tailored to your metadata

###### To make migrate_7x_claw work you need

- Access
  - You need credentials to both your Islandora 7 and 8 installs.
- Migrate API Knowledge
  - The tutorial for migrate_islandora_csv
Is still relevant
- Config Sync
  - You need to understand Drupal config synchronization.  Features knowledge helps too.
- Command Line Skills
  - This is best done with shell access and drush

#### Migrate API demo video

Check out this video that demonstrates the Drupal Migrate API migration process: [Islandora Webinar: Migrating from Islandora 7 to Islandora 8](migrate-7x.md)

### Islandora Workbench

https://github.com/mjordan/islandora_workbench

More tailored for end users with less technical knowledge or limited server access.

- Uses Islandora’s REST API
- Runs on your computer
- “CSVs and a pile of scans”
- Cross Platform - Python

#### Islandora Workbench highlights

- Opinionated
  - MUCH less configuration. Decisions made for you.
- No Processing
  - CSV has to be in the right format
- Write Operations
  - Create, Update, and Delete content
- Bumpers On
  - Configuration and CSV are validated

#### Islandora Workbench basics

- Column names are field names
- If your value contains a comma, wrap it in double quotes
- Multiple values are pipe delimited
- Entity references are done via numeric id (nid, mid, tid)

Islandora Workbench - Taxonomy Terms:

- Can use term id, term name, or both
  - 26
  - Cats
  - 26|Cats
- If using multiple vocabularies, prefix with vocabulary id:
  - cats:Calico|dogs:Dachshund
- Terms that don’t exist can be created

Islandora Workbench - More Field Types:

- Typed Relations - Prefix term ids with namespace:rel:
  - relators:pht:30
  - Relators:pht:30|relators:pub:45
-  Geolocation fields - “Lat,Long”
  - "49.16667,-123.93333"

Paged Content - Two Ways:

- Metadata on Parent
  - Simple directory structure and filename convention
- Page Level Metadata
  - Parent and page metadata in same CSV
