# TransactionService

This a Islandora PHP Microservice to create/extend/commit or rollback Fedora 4 transactions

## Requirements

* [Chullo](https://github.com/Islandora-CLAW/chullo)
* [ResourceService](../ResourceService)
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

The TransactionService provides the following endpoints for HTTP requests. 

**Note**: The transaction ID (or txID) is of the form `tx:83e34464-144e-43d9-af13-b3464a1fb9b5`

1. POST to `/transaction`

    for creating a new transaction. It returns the transaction ID in the Location: header. It can be retrieved by passing the Response to the `getId()` function.
    
2. POST to `/transaction/{txID}/extend`

    for extending a transaction. Normally a transaction will expire once it has sat for approximately 3 minutes without any interactions. This allows you to extend the transaction without performing any other interaction.
    
3. POST to `/transaction/{txID}/commit`

    to commit the transaction.
    
4. POST to `/transaction/{txID}/rollback`

    to rollback a transaction

## License

[MIT](https://opensource.org/licenses/MIT)
