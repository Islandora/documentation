## Islandora Workbench

Islandora Workbench provides a command-line solution to create, update, and delete Islandora content from CSV data. The Islandora Workbench repository can be found on [Github](https://github.com/mjordan/islandora_workbench). Full details and documentation for Islandora Workbench are maintained separately ([Islandora Workbench Documentation](https://mjordan.github.io/islandora_workbench_docs/)).


### Overview

Islandora Workbench is tailored towards end users with less technical knowledge or limited server access. 

Workbench provides an alternative to Drupal's Migrate framework, as it does not need to be run on the Drupal server. Islandora Workbench uses Islandora's REST API and offers cross-platform support (Windows, Mac, Linux) to run on your computer, using your provided CSVs and files.

### Islandora Workbench highlights

Islandora Workbench:

- **Enables you to perform write operations:** Create, update, and delete content.
- **Is opinionated:** Workbench provides MUCH less configuration than Drupal's Migrate framework. Decisions are made for you.
- **Does not provide pre-processing:** Your CSVs have to be in the right format. More information on preparing your data can be found in [the Islandora Workbench documentation](https://mjordan.github.io/islandora_workbench_docs/preparing_data/).
- **Provides data validation:**: The YAML configuration file and CSVs you provide are validated with the [--check option](https://mjordan.github.io/islandora_workbench_docs/check/). 

### Islandora Workbench Basics

- Column names are field names.
- If a cell value contains a comma, make sure the value is wrapped in double quotes. Spreadsheet applications will do this for you
- Multiple values are pipe delimited.
- Entity references are done via numeric id (nid, mid, tid).

#### [Taxonomy Terms](https://mjordan.github.io/islandora_workbench_docs/fields/#taxonomy-reference-fields)

Using Islandora Workbench, **you can assign both existing and new taxonomy terms to nodes.** Within the CSVs you provide, the values of the taxonomy field/columns can:

- Use term IDs (integers), term names, or both. For example:
    - 26
    - Cats
    - 26|Cats
-  Use multiple vocabularies, by prefixing the value with the vocabulary id:
    - cats:Calico|dogs:Dachshund
- Create new terms that don't exist yet in your taxonomy.

If you need to create terms with multiple fields (such as an External URI) or which use term hierarchy, you can [create terms](https://mjordan.github.io/islandora_workbench_docs/creating_taxonomy_terms/) in a separate task.

#### Other Field Types

- Typed Relations - Prefix term ids with namespace:rel:. More available on typed relation fields [here](https://mjordan.github.io/islandora_workbench_docs/fields/#typed-relation-fields). For example:
    - relators:pht:30
    - Relators:pht:30|relators:pub:45
-  Geolocation fields - Workbench allows geocoordinates to be provides in “Lat,Long” format. For example:
    - "49.16667,-123.93333"

#### [Paged Content](https://mjordan.github.io/islandora_workbench_docs/paged_and_compound/)

There are multiple ways to create paged content with Islandora Workbench. More information on each option is available [here](https://mjordan.github.io/islandora_workbench_docs/paged_and_compound/). You may:

- Use [a specific subdirectory structure](https://mjordan.github.io/islandora_workbench_docs/paged_and_compound/#using-subdirectories) to define the relationship between the parent item and its children.
- Use [page-level metadata in the CSV](https://mjordan.github.io/islandora_workbench_docs/paged_and_compound/#with-pagechild-level-metadata) to create the relationship.
- Create [a secondary task](https://mjordan.github.io/islandora_workbench_docs/paged_and_compound/#using-a-secondary-task) in Workbench.

### Workbench Instructional Videos

Click the video previews shown below to open the corresponding video in Youtube.

### IslandoraCon 2022: Migration Strategies

This [video](https://www.youtube.com/watch?v=FzISzvc9xbE) (August 2022) is an overview of how to harvest data from an existing Islandora 7 site using Workbench, how to sanitize and prep that data with custom Python tools, and how to pull that data into a new Islandora 2 site.

[![IslandoraCon 2022: Migration Strategies](https://img.youtube.com/vi/FzISzvc9xbE/0.jpg)](https://www.youtube.com/watch?v=FzISzvc9xbE)


#### Summer of Islandora Workbench: Introduction to Islandora Workbench

This [video](https://www.youtube.com/watch?v=bHMPsbYG-4c) (July 29, 2021) is an introduction of Islandora Workbench.

[![Summer of Islandora Workbench: Introduction to Islandora Workbench](https://img.youtube.com/vi/bHMPsbYG-4c/0.jpg)](https://www.youtube.com/watch?v=bHMPsbYG-4c)


#### Islandora Workbench Demo

This [video](https://www.youtube.com/watch?v=hNS5ouqdcfk) (Dec 17, 2020) is a demo of Islandora Workbench.

[![Islandora Workbench Demo](https://img.youtube.com/vi/hNS5ouqdcfk/0.jpg)](https://www.youtube.com/watch?v=hNS5ouqdcfk)


#### Islandora Online: Islandora Migration Tools

This [video](https://www.youtube.com/watch?v=95Bnix-z1zY) (Aug 10, 2020) provides an overview of the **Islandora Workbench** and the two other options available to migrate data into an Islandora installation.

[![Islandora Online: Islandora Migration Tools](https://img.youtube.com/vi/95Bnix-z1zY/0.jpg)](https://www.youtube.com/watch?v=95Bnix-z1zY)
