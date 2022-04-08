## REST API

Why use the REST API?

- **Works anywhere**: You don’t have to work on the Drupal server. Migrate from your laptop!
- **No PHP required**: Use any language that can make an HTTP request. Even cURL will do just fine.
- **JSON**: Why use XML if you don’t have to?

BONUS: It’s just Drupal’s REST API

### Islandora only provides two additional API endpoints

- /media/{mid}/source
    - PUT a file to this endpoint to create/update a Media’s file
- /node/{nid}/media/{media_type}/{taxonomy_term}
    - PUT a file to this endpoint to create/update a Media for a Node

Just be aware, you are writing everything yourself! (In other words you are making all of the migration decisions yourself.)

### Videos

This [video](https://www.youtube.com/watch?v=95Bnix-z1zY) (Aug 10, 2020) provides an overview of the **REST API** and the two other options available to migrate data into an Islandora installation.

[![Islandora Online: Islandora Migration Tools](https://img.youtube.com/vi/95Bnix-z1zY/0.jpg)](https://www.youtube.com/watch?v=95Bnix-z1zY)
