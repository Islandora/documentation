# ResourceService

This an Islandora PHP Microservice to perform some middleware functions such as

1. UUID -> Fedora4 path translation
2. UUID validation
3. Host header normalization

and pass the request to Chullo.

## Requirements

* [Chullo](https://github.com/Islandora-CLAW/chullo)
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

The ResourceService provides the following endpoints for HTTP requests. 

**Note**: The UUID is of the form `18c67794-366c-a6d9-af13-b3464a1fb9b5`

1. GET from `/resource/{uuid}/{child}`

    for getting the Fedora Resource from either {uuid} (if {child} is left off), or {child} if both are provided.
    

1. POST to `/resource`

    for creating a new Resource at the root level

2. POST to `/resource/{uuid}`

    for creating a new Resource as a child of resource {uuid}

3. PUT to `/resource/{uuid}/{child}`

    for creating a new Resource with a predefined name {child} under the parent {uuid}, to PUT at root leave the {uuid} blank (ie. //).

1. PATCH to `/resource/{uuid}/{child}`

    for patching a resource at either {uuid} (if {child} is left off), or {child} if both are provided.
    
2. DELETE to `/resource/{uuid}/{child}`

    for deleting a resource at either {uuid} (if {child} is left off), or {child} if both are provided.
    

## License

[MIT](https://opensource.org/licenses/MIT)
