# Islandora Architecture Diagram

## Site Serving Path

When a client visits an Islandora website, the request flow looks like a typical Drupal request.
The request is received by an nginx web server, which forwards the request to `php-fpm` process that points to a Drupal codebase.
Drupal bootstraps and queries the backend database (Islandora ships with `mariadb`).
If the request was for a search page, Drupal make also query `solr` to include search results in the HTML response.
```mermaid
flowchart TD
    user([Client / Browser])
    user e1@==>|HTTP request| nginx

    subgraph webserver[Nginx Web Server]
        nginx[Nginx] e2@==>|forward request| drupal[Drupal]
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

For some items in Islandora, the HTML response may include a [IIIF Manifest]. This typically happens when showing items that have image media. For those requests, Islandora's IIIF module may also query the [Cantaloupe] IIIF server for metadata needed to generate a IIIF manifest. The HTML response will then reference the location of the images in the IIIF server to allow a IIIF viewer to render the image in the browser. Practically what this means is in addition to the typical drupal request flow, Drupal may also query the IIIF server for basic image metadata (e.g. width/height) which are needed in the IIIF manifest response. The client web browser will then read that manifest using Javascript and the IIIF viewer will `GET` the images referenced in the IIIF manifest directly from the IIIF server.

```mermaid
flowchart TD
    user([Client / Browser])
    user e1@==>|HTTP request| nginx

    subgraph webserver[Nginx Web Server]
        nginx[Nginx] e2@==>|forward request| drupal[Drupal]
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

As an example, when an Islandora Repository manager uploades an image to Islandora, an event is emitted to generate a thumbnail for that image. That event is put on the event queue, alpaca reads the message from the queue, and forwards the event to the configured service. In the case of a thumbnail, houdini handles generating the thumbnail for the uploaded image 

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

* [FITS](https://github.com/roblib/CrayFits) - A Symfony 4 Microservice to generate FITS data and persist it as a Drupal media node. Works with [Islandora FITS](https://github.com/roblib/islandora_fits)
* [Homarus](https://github.com/Islandora/Crayfish/tree/dev/Homarus) - Provides [FFmpeg](https://www.ffmpeg.org/) as a microservice for generating video and audio derivatives.
* [Houdini](https://github.com/Islandora/Crayfish/tree/dev/Houdini) - [ImageMagick](https://www.imagemagick.org/script/index.php) as a microservice for generating image-based derivatives, including thumbnails.
* [Hypercube](https://github.com/Islandora/Crayfish/tree/dev/Hypercube) - [Tesseract](https://github.com/tesseract-ocr) as a microservice for optical character recognition (OCR).
* [Milliner](https://github.com/Islandora/Crayfish/tree/dev/Milliner) - A microservice that converts Drupal entities into Fedora resources.
* [Recast](https://github.com/Islandora/Crayfish/tree/dev/Recast) - A microservice that remaps Drupal URIs to add Fedora-to-Fedora links based on associated Drupal URIs in RDF.


### Other Open Source

The following components are deployed with Islandora, but are developed and maintained by other open source projects:


* [Apache](https://www.apache.org/) - The Apache HTTP Server, colloquially called Apache, is a free and open-source cross-platform web server software. Provides the environment in which Islandora and its components run.
   * [ActiveMQ](https://activemq.apache.org/) - Apache ActiveMQ is an open source message broker written in Java together with a full Java Message Service client.
   * [Karaf](https://karaf.apache.org/) - A modular open source OSGi runtime environment.
   * [Tomcat](http://tomcat.apache.org/) - an open-source implementation of the Java Servlet, JavaServer Pages, Java Expression Language and WebSocket technologies. Tomcat provides a "pure Java" HTTP web server environment in which Java code can run.
   * [Solr](https://lucene.apache.org/solr/) - An open-source enterprise-search platform. Solr is the default search and discover layer of Islandora, and a key component in some methods for [migration to Islandora from Islandora Legacy](https://github.com/Islandora-devops/migrate_7x_claw)
* [Blazegraph](https://blazegraph.com/) - Blazegraph is a triplestore and graph database.
* [Cantaloupe](https://cantaloupe-project.github.io/) - an open-source dynamic image server for on-demand generation of derivatives of high-resolution source images. Used in Islandora to support [IIIF](https://iiif.io/)
* [Drupal](https://www.drupal.org/) - Drupal is an open source content management system, and the heart of Islandora. All user and site-building aspects of Islandora are experienced through Drupal as a graphical user interface.
* [Fedora](https://wiki.lyrasis.org/display/FF/Fedora+Repository+Home) - A robust, modular, open source repository system for the management and dissemination of digital content. The default smart storage for Islandora.
* [Matomo](https://matomo.org/) - Matomo, formerly Piwik, is a free and open source web analytics application. It provides usage statistics and a rich dashboard for Islandora.
* [MySQL](https://www.mysql.com/) - MySQL is an open-source relational database management system. Used as a Drupal database in Islandora, it can be easily replaced with other database management systems such as [PostgreSQL](https://www.postgresql.org/)
* Triplestore - See Blazegraph.
