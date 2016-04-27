# CollectionService

This an Islandora PHP Microservice to create PCDM:Collections and add/remove PCDM:Objects to a PCDM:Collection.

## Requirements

* [Chullo](https://github.com/Islandora-CLAW/chullo)
* [ResourceService](../ResourceService)
* [TransactionService](../TransactionService)
* [Fedora 4](https://github.com/fcrepo4/fcrepo4)
* A triplestore (i.e. [BlazeGraph](https://www.blazegraph.com/download/), [Fuseki](https://jena.apache.org/documentation/fuseki2/), etc)

## Installation

You will need to copy the configuration file [_example.settings.yml_](config/example.settings.yml) to either **settings.yml** or **settings.dev.yml** (if $app['debug'] = TRUE) and change any required settings.

You can run just this service using PHP by executing 

```
php -S localhost:<some port> -t src/ src/index.php
```
from this directory to start it running.

## Services

The CollectionService provides the following endpoints for HTTP requests. 

**Note**: The UUID is of the form `18c67794-366c-a6d9-af13-b3464a1fb9b5`

1. POST to `/collection`

    for creating a new PCDM:Collection at the root level

2. POST to `/collection/{uuid}`

    for creating a new PCDM:Collection as a child of resource {uuid}
    
2. POST to `/collection/{uuid}/member/{member}`

    for adding the resource identifier by the UUID {member} to the collection identified by the UUID {uuid}
    
2. DELETE to `/collection/{uuid}/member/{member}`

    for removing the resource identifier by the UUID {member} from the collection identified by the UUID {uuid}

## License

[MIT](https://opensource.org/licenses/MIT)
