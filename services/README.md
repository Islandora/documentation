# Services

This is a top level container for the various microservices. It allows you to mount the various endpoints at one port on one machine and makes a development vagrant/docker configuration easier to produce.

## Requirements

* [Chullo](https://github.com/Islandora-CLAW/chullo)
* [ResourceService](./ResourceService/)
* [TransactionService](./TransactionService/)
* [CollectionService](./CollectionService/)
* [Fedora 4](https://github.com/fcrepo4/fcrepo4)
* A triplestore (i.e. [BlazeGraph](https://www.blazegraph.com/download/), [Fuseki](https://jena.apache.org/documentation/fuseki2/), etc)

## Installation

You will need to copy the configuration file _example.settings.yml_ to either **settings.yml** or **settings.dev.yml** (if $app['debug'] = TRUE) and change any required settings.

You can run just this service using PHP by executing 

```
php -S localhost:<some port> -t src/ src/index.php
```
from this directory to start it running.

## Services

This mounts all the various individual microservices under the `/islandora` URL, so you currently have access to 

* ResourceService at `/islandora/resource`
* CollectionService at `/islandora/collection`
* TransactionService at `/islandora/transaction`

See the individual services for more information on their endpoints.

## License

[MIT](https://opensource.org/licenses/MIT)
