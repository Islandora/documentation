# Islandora Architecture Diagram

## Site Serving Path

### Page Request

```mermaid
flowchart TD
    user([Client / Browser])
    user e1@==>|HTTP request| nginx

    subgraph webserver[Nginx Web Server]
        nginx[Nginx] e2@==>|forward request| drupal[Drupal]
    end

    drupal e3@-->|query| mariadb[(MariaDB)]
    drupal e4@-->|query| solr[(Solr)]
    drupal e5@-->|image request| cantaloupe[Cantaloupe\nIIIF Image Server]

    mariadb e6@-.->|data| drupal
    solr e7@-.->|results| drupal
    cantaloupe e8@-.->|image| drupal
    drupal e9@-.->|HTML response| nginx
    nginx e10@-.->|HTML response| user

    classDef flow0 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dashFlash 8s 0s linear infinite;
    classDef flow1 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dashFlash 8s 1s linear infinite;
    classDef flow2 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dashFlash 8s 2s linear infinite;
    classDef flow3 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dashFlash 8s 3s linear infinite;
    classDef flow4 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dashFlash 8s 4s linear infinite;
    classDef flow5 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dashFlash 8s 5s linear infinite;

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

### IIIF Image Request

```mermaid
flowchart TD
    user([Client / Browser])
    user --> cantaloupe[Cantaloupe\nIIIF Image Server]
    cantaloupe -.->|fetch source file| fedora[(Fedora\nFile Storage)]
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

    subgraph microservices[scyllaridae microservices]
        houdini[Houdini]
    end

    houdini e4@-.->|derivative stream| alpaca
    alpaca e5@-.->|saves derivative & responds ok| drupal

    classDef flow1 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dash 3s linear infinite;
    classDef flow2 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dash 3s 1s linear infinite;
    classDef flow3 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dash 3s 2s linear infinite;
    classDef flow4 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dash 3s 3s linear infinite;
    classDef flow5 stroke-dasharray: 9,5,stroke-dashoffset: 900,animation: dash 3s 4s linear infinite;

    class e1 flow1;
    class e2 flow2;
    class e3 flow3;
    class e4 flow4;
    class e5 flow5;
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
