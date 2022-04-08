## Islandora Workbench


### Repository

<https://github.com/mjordan/islandora_workbench>

### Overview

More tailored for end users with less technical knowledge or limited server access.

- Uses Islandora’s REST API
- Runs on your computer
- “CSVs and a pile of scans”
- Cross Platform - Python

### Islandora Workbench highlights

- Opinionated
    - MUCH less configuration. Decisions made for you.
- No Processing
    - CSV has to be in the right format
- Write Operations
    - Create, Update, and Delete content
- Bumpers On
    - Configuration and CSV are validated

### Islandora Workbench basics

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


### Videos


#### Summer of Islandora Workbench: Introduction to Islandora Workbench

This [video](https://www.youtube.com/watch?v=bHMPsbYG-4c) (July 29, 2021) is an introduction of Islandora Workbench.

[![Summer of Islandora Workbench: Introduction to Islandora Workbench](https://img.youtube.com/vi/bHMPsbYG-4c/0.jpg)](https://www.youtube.com/watch?v=bHMPsbYG-4c)


#### Islandora Workbench Demo

This [video](https://www.youtube.com/watch?v=hNS5ouqdcfk) (Dec 17, 2020) is a demo of Islandora Workbench.

[![Islandora Workbench Demo](https://img.youtube.com/vi/hNS5ouqdcfk/0.jpg)](https://www.youtube.com/watch?v=hNS5ouqdcfk)


#### Islandora Online: Islandora Migration Tools

This [video](https://www.youtube.com/watch?v=95Bnix-z1zY) (Aug 10, 2020) provides an overview of the **Islandora Workbench** and the two other options available to migrate data into an Islandora installation.

[![Islandora Online: Islandora Migration Tools](https://img.youtube.com/vi/95Bnix-z1zY/0.jpg)](https://www.youtube.com/watch?v=95Bnix-z1zY)
