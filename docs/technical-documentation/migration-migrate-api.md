## Migrate API

Uses the Drupal [Migrate API](https://www.drupal.org/docs/drupal-apis/migrate-api/migrate-api-overview), which "provides services for migrating data from a source system" to Drupal 8, 9, or 10.

The "source system" can be almost anything:

- an Islandora Legacy system
- a group of scanned images and their metadata inside a CSV file
- a web API

Why use the Migrate API?

- You can (potentially) do everything with configs!
- Leverage contrib module plugins.
- Making plugins for more complex sources and processes is (relatively) simple.
- Updating metadata is as simple as:
`drush mim node --update`

### A Migration Configuration defines an Extract, Transform, Load (ETL) process

- Source plugins extract data from a source
- Process plugins transform the data
- Destination plugins load the data (create new entities)

### We’ve built two tools for you using the Migrate API

- **migrate_islandora_csv**
    - <https://github.com/Islandora/migrate_islandora_csv>
        - Tutorial with a sample migration using some files and a CSV
    - Documentation section on [migrate_islandora_csv](migrate-csv.md)
- **migrate_7x_claw**
    - <https://github.com/Islandora-Devops/migrate_7x_claw>
      - A tool to get all your Islandora Legacy content migrated over
    - Documentation section on [migrate_7x_claw](migrate-7x)

#### Recap of migrate_islandora_csv

- CSVs
    - Everyone understands and knows how to work with CSVs
- Documented
    - It’s a step-by-step walkthrough
- Process Metadata
    - Clean up / transform the metadata using processors
- Build Relationships
    - Migrations can reference other migrated content or generate new content on the fly

#### Recap of migrate_7x_claw

- Designed to migrate Islandora Legacy data to Islandora.
- DATASTREAMS
    - All of your datasteams, including the audit trail, are migrated
- METADATA
    - Migrate metadata from Solr or any XML datastream
- CUSTOMIZABLE
    - Migrate_7x_claw is a starting point, meant to be tailored to your metadata

##### To make migrate_7x_claw work you need

- Access
    - You need credentials to both your Islandora Legacy and Islandora installs.
- Migrate API Knowledge
    - The tutorial for migrate_islandora_csv
Is still relevant
- Config Sync
    - You need to understand Drupal config synchronization.  Features knowledge helps too.
- Command Line Skills
    - This is best done with shell access and drush

### Migrate API demo video

Check out this video that demonstrates the Drupal Migrate API migration process: [Islandora Webinar: Migrating from Islandora Legacy to Islandora](migrate-7x.md) (Nov 21, 2019)
