#Architecture

##Overview
Islandora 7.x-2.x is a large and comrpehensive technology stack that encompasses a Fedora 4 repository, a Drupal CMS, and an Apache Camel based middleware layer to integrate the two along with all of the other complementary technologies required for digital asset management and preservation (full text search engine, triplestore, command line tools, etc...).  Here is a (somewhat) exahuastive list of the technologies in an Islandora stack:
- Fedora 4 (deployed in a servlet container such as Tomcat or Jetty)
- Drupal 7 (deployed in an Apache 2 web server), which includes:
    - A relational database such as MySQL or PostgreSQL
    - Several third-party Drupal modules from drupal.org
    - Several Islandora Drupal modules
    - Several javascript viewers for rendering content (pdf.js, video.js, etc...)
- Apache Solr (deployed in either an OSGi or servlet container)
- Blazegraph (or the triple store of your choice, most likely deployed in a servlet container)
- Apache Camel based middleware (all deployed in an OSGi container), which includes:
    - Islandora Sync - Asynchronous and event driven software to generate derivatives and align Drupal fields with Fedora RDF
    - Islandora Services - RESTful web services exposed through the OSGi container that accept Drupal information and manipulate Fedora resources
    - Fcrepo Camel Toolbox - Indexes Fedora content in the triplestore
- Islandora Commands - A command-line PHP script utilized by Services and Sync to provide processing capabilities beyond what can be done in a Blueprint xml file
- Several command line tools for derivative generation:
    - imagemagick - For image manipulation
    - tesseract - For OCR generation
    - ffmpeg - For video processing
    - lame - For audio processing
    - fits - For technical metadata generation
    - and much much more as functionality requrires....

Here’s an image representing the various components of the stack and how they interoperate:

## Scalability
One of the main goals with the 7.x-2.x project is to allow for horizontal scalability, and the architecture of the stack has been designed with this in mind.  No assumptions are made requiring any of the components to be on the same machine, so it can all be split apart onto multiple resources.  Most of the components also provide some sort of horizontal scalability through sharding, replication, or both.  If desired, the following components can be clustered through configuration and their respective installation procdedures:
- Fedora 4
    - Fedora 4 sits on top of Modeshape, which provides both replication and sharding capabilities.  See https://docs.jboss.org/author/display/MODE/Clustering
- Apache Solr
    - Solr indices can be sharded to allow for distributed searching. See https://cwiki.apache.org/confluence/display/solr/Distributed+Search+with+Index+Sharding
- Blazegraph
    - Blazegraph was chosen as the ‘default’ triplestore for Islandora because it is well known to provide both sharding and replication capabilities.  See https://wiki.blazegraph.com/wiki/index.php/HAJournalServer and  https://wiki.blazegraph.com/wiki/index.php/ClusterSetupGuide
- Drupal
    - The Drupal front end can be scaled out by having multiple Apache webservers behind a load balancer
    - Scaling the relational database (while difficult) is possible, though typical vertical scaling is the first approach taken

While not yet implemented, it will be possible to scale the Camel based middleware both the web services and the sync based listeners through the use of Camel’s load balancer design pattern.  See https://camel.apache.org/load-balancer.html
