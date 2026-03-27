# Islandora Architecture

## Site Serving Path

When a client visits an Islandora website, the request flow looks like a typical [Drupal] request.
The request is received by an nginx web server, which forwards the request to `php-fpm` process that points to a Drupal codebase.
Drupal bootstraps and queries the backend database (Islandora ships with `mariadb`).
If the request was for a search page, Drupal make also query `solr` to include search results in the HTML response.
```mermaid
flowchart TD
    user([Client / Browser])
    user e1@-->|HTTP request| nginx

    subgraph webserver[Nginx Web Server]
        nginx[Nginx] e2@-->|forward request| drupal[Drupal]
    end

    drupal e3@-->|query| mariadb[(MariaDB)]
    drupal e4@-->|query| solr[(Solr)]

    mariadb e6@-.->|data| drupal
    solr e7@-.->|results| drupal
    drupal e9@-.->|HTML response| nginx
    nginx e10@-.->|HTML response| user

    class e1 flow0;
    class e2 flow1;
    class e3 flow2;
    class e4 flow2;
    class e5 flow2;
    class e6 flow3;
    class e7 flow3;
    class e8 flow3;
    class e9 flow4;
    class e10 flow5;

```


### IIIF manifests

For some [Resource Node]s in Islandora, the HTML response may include a [IIIF Manifest]. This typically happens when showing items that have image media. For those requests, Islandora's IIIF module may also query the [Cantaloupe] IIIF server for metadata needed to generate a [IIIF Manifest]. The HTML response will then download the images from Cantaloupe to allow a IIIF viewer (e.g. [OpenSeadragon] or [Mirador]) to render the image in the browser.

In addition to [the typical drupal request flow](##site-serving-path), Drupal may also query Cantaloupe for basic image metadata (e.g. width/height) which are needed to complete a valid [IIIF Manifest] response. The client web browser will then read that IIIF Manifest using Javascript and the IIIF viewer will `GET` the images referenced in the IIIF Manifest directly from the IIIF server.

```mermaid
flowchart TD
    user([Client / Browser])
    user e1@-->|HTTP request| nginx

    subgraph webserver[Nginx Web Server]
        nginx[Nginx] e2@-->|forward request| drupal[Drupal]
    end

    drupal e3@-->|GET /info.json| cantaloupe[Cantaloupe IIIF Image Server]

    cantaloupe e4@-->|info.json| drupal
    drupal e5@-.->|HTML response| nginx
    nginx e6@-.->|HTML response| user
    user e7@-->|GET /image.png| cantaloupe
    cantaloupe e8@-.->|image.png| user

    class e1 flow0;
    class e2 flow1;
    class e3 flow2;
    class e4 flow3;
    class e5 flow4;
    class e6 flow5;
    class e7 flow6;
    class e8 flow7;
    class e9 flow8;

```

## Event Driven System

When you create, update, or delete Drupal [entities], Drupal emits an event message which is emitted to [ActiveMQ] and put on ActiveMQ's queue.


```mermaid
flowchart TD
    drupal([Islandora Drupal Website])

    drupal ==>|publishes drupal entity event| activemq

    subgraph broker[Message Broker]
        activemq[ActiveMQ]
        alpaca[Alpaca]
        activemq ==> alpaca
        note[\"Alpaca forwards the message to the\nappropriate service based on its config"/]
    end


    subgraph microservices[scyllaridae microservices]
        fits[FITS]
        homarus[Homarus]
        houdini[Houdini]
        hypercube[Hypercube]

        s[\"scyllaridae generates a derivative and saves the message to the\nappropriate service based on its config"/]

    end

    alpaca ==> fits
    alpaca ==> homarus
    alpaca ==> houdini
    alpaca ==> hypercube
    alpaca ==> milliner

    fits -.->|derivative saved to| alpaca
    homarus -.->|derivative saved to| alpaca
    houdini -.->|derivative saved to| alpaca
    hypercube -.->|derivative saved to| alpaca
alpaca -.->|ok| drupal
    fedora[(Fedora)]
    milliner -.->|syncs resource to| fedora
    blazegraph[(blazegraph)]
    alpaca -.->|sends RDF| blazegraph
```

## Event Example

As an example, when an Islandora Repository manager uploades an image to their Islandora repository, an event is emitted to generate a thumbnail for that image. That event is put on the event queue, alpaca reads the message from the queue, and forwards the event to the configured service. In the case of a thumbnail, [houdini] handles generating the thumbnail for the uploaded image 

```mermaid
flowchart TD
    drupal([Islandora Drupal Website])

    drupal e1@==>|publishes entity event| activemq

    subgraph broker[Message Broker]
        activemq[ActiveMQ]
        alpaca[Alpaca]
        activemq e2@==> alpaca
    end

    alpaca e3@==> houdini

    subgraph microservices[scyllaridae microservice]
        houdini[Houdini]
    end

    houdini e4@-.->|derivative stream| alpaca
    alpaca e5@-.->|saves derivative & responds ok| drupal

    class e1 flow0;
    class e2 flow1;
    class e3 flow2;
    class e4 flow3;
    class e5 flow4;
```

## Components

### Islandora

The following components are microservices developed and maintained by the Islandora community. They are bundled under [Islandora Crayfish](https://github.com/Islandora/Crayfish):

* [FITS]
* [Homarus]
* [Houdini]
* [Hypercube]
* [Milliner]

### Other Open Source

The following components are deployed with Islandora, but are developed and maintained by other open source projects:

* [Apache]
    * [ActiveMQ]
    * [Tomcat]
    * [Solr]
* [Blazegraph]
* [Cantaloupe]
* [Drupal]
* [Fedora (Repository Software)]
* [MariaDB]
* Triplestore - See [Blazegraph].
