#Islandora 7.x 2.x Technical Design Doc

Islandora version 7.x-2.x is middleware built using Apache Camel to orchestrate distributed data processing and to provide web services required by institutions who would like to use Drupal as a frontend to a Fedora 4 JCR repository.  This goal presents a unique set of challenges, as Drupal is much, much more than a simple display layer.  It is a full blown content management system designed to be built on top of a traditional relational database such as MySQL or Postgres, not a JCR repository.  Additionally, there is a large amount of data processing and manipulation that must be performed for presentation and discovery.  This means that there is much more software that must be integrated than just Drupal and Fedora (Tesseract, ImageMagick, ffmpeg, just to name a few).  To make matters worse, doing all of this processing on the servers containing either Fedora or Drupal is detrimental to the performance of the overall system, resulting in a unusable site during periods of content migration or manipulation.  Plus, as most of us have already found out, systems such as these are incredibly difficult to install, configure, and maintain.

To mitigate these issues, the overall design goals of the 7.x-2.x version of Islandora are:
  - A properly modularized installation procedure so that Islandora can be consistently installed and configured in distributed environments.  As a result of this, a consistent development environment can also be made available to contributors.
  - Asynchronous communication between Fedora and Drupal, so that neither waits on the other nor any of the various processing components of the stack.  This will be achieved through the use of persistant queues, which will also allow the stack to be easily distribruted across multiple computers.
  - Fedora is treated as the source of the truly important data, only containing preservation masters and descriptive metadata.  Metadata can exist either in Fedora's native RDF attached to the resource itself, or as standardized formats such as MODS, MADS, PBCORE, etc... that exist as resources in their own right.  
  - Data from Fedora is transformed and indexed into the other major system of the stack, most notably Drupal and Solr.  This includes lower quality access copies of perservation masters such as thumbnails or streaming video, which will be stored in Drupal as managed files.
  - Drupal content is represented as Nodes and Fields, allowing the content management system to utilize the relational database it is expecting instead of shimming in a completely different type of datastore.  This will open up the entire Drupal module ecosystem to Islandora.  As an added benefit, viewers (OpenSeaDragon, IA Book Viewer, Video.js, etc...) can be written as custom Field renderers, finally giving site builders the ability to control the display of content.  
  - Drupal's Services module will be used to expose RESTful services to middleware layer so that it can sanely perform CRUD operations on Nodes without having to delve into Drupal's internals.

### The Importance of Using an Integration Framework
Let's not mince words.  *Islandora is middleware, warts and all.*  The word 'middleware' has plenty of connotations and baggage, but it really is what we're doing.  We have a huge stack with a lot of moving parts, and we have to glue them all together.  So it only makes sense to adopt a integration framework to help us pull this off.  

The framework that has been chosen for the project is [Apache Camel].  It seeks to provide implementations of the fundamental design patterns codified by Gregor Hohpe and Bobby Woolf in their book [Enterprise Integration Patterns].  The best way to describe these patterns is that they're standardized, re-usable templates for processing messages that have to flow through multiple pieces of software before arriving at their final destination.  So Camel will help us do things like route messages from Fedora's queue/topic to the appropriate handling function for an operation on a particular content type, allowing it to be processed along the way by derivative generation tools on the commandline.  In addition to this, Camel provides code for interacting with software through basically any protocol you can think of, so we don't have to waste our time writing code for common situations like posting to http endpoints, reading data from files, polling queues, etc...  It also has fantastic support for try/catch exception handling across distributed systems and transactional functionality.  

But perhaps the greatest advantage of using an integration framework is that lets us focus solely on the application logic that's important to Islandora.  *There is no need for us to engineer any generic systems to get our job done.*  We can identify the operations that need to happen for every supported content type, carve out a space to do the work, and get to it.  It's not particuarly sexy, but the work we have to do is difficult and we've already got enough on our plate!  

### Using Camel
Camel, which at first glance appears as terrifying as Java IOC frameworks (more on those later), is actually incredibly straightforward.  A Camel application is known as a [Camel Context], which is really just a collection of messaging [Route]s.  These routes are defined in [Route Builder] classes.  Each route has a starting point (the from() method), from which an initial [Message] is consumed.  The Message is placed in an [Exchange], which contains two messages: one incoming, and another outgoing.  As the Exchange is passed through each step of the route, the outgoing message from one step becomes the incoming message of the next.  Data that must persist between multiple steps in the route can be cached in the Exchange as properties.

That's pretty much it.  Seriously.

Camel provides most of the functionality we need to work with these routes out of the box, and has built-ins for message routing, filtering, data extraction with XPath, transformations with Xslt's, and much more.  If you need something beyond what is offered by default, you can make your own custom [Processor]s, which have unfettered access to the Exchange for whatever custom logic you desire.  Heck, you can even define Processors as anonymous subclasses on the fly within the RouteBuilder.  It looks almost like Javascript!

But perhaps the best part of using Camel is that [Aaron Coburn] has already created [fcrepo-camel].  This Camel component makes working with Fedora's REST API and JMS Messages incredibly easy.  Everyone involved in the project now officially owes Aaron a beer :)

But enough talk.  Let's look at an example.  Here's a Camel route that consumes a NODE_ADDED message from Fedora and responds by creating a Node in Drupal!

```Java
import org.apache.camel.builder.RouteBuilder;
import org.fcrepo.camel.JmsHeaders;
import org.fcrepo.camel.RdfNamespaces;
import org.islandora.sync.processors.DrupalNodeCreateJsonTransform;

public class DrupalNodeCreate extends RouteBuilder {
    public void configure() throws Exception {

        from("activemq:topic:fedora")
            .routeId("fedoraIn")
            .filter(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_ADDED"))
                .to("fcrepo:localhost:8080/fcrepo/rest")
                .process(new DrupalNodeCreateJsonTransform())
                .to("http4:localhost/drupal7/rest/node");
    }
}
```

One of the first things you'll notice when looking at Camel routes is that they almost look as if they're written in English.  This makes them incredibly easy to follow.  Without knowing anything about Camel, you should be able to tell that this route is named "fedoraIn", reads a message from Fedora's Activemq topic, filters out messages that aren't NODE_ADDED, fetches data from Fedora, transforms it into JSON, and sends it off to Drupal.  Phew!  

So let's take a look a that custom transform and see what's going on.
```Java
import static org.apache.camel.component.http4.HttpMethods.POST;

import org.apache.camel.Exchange;
import org.apache.camel.Message;
import org.apache.camel.Processor;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class DrupalNodeCreateJsonTransform implements Processor {

    @SuppressWarnings("unchecked")
    @Override
    public void process(Exchange exchange) throws Exception {
        /*
         * Make some json that looks like this:
         * {
         *   "title":"From Fedora!!!",
         *   "body":{
         *     "und":[
         *       {
         *          "value": "RDF FROM FEDORA",
         *          "format":"plain_text"
         *       }
         *     ]
         *    },
         *    "type":"article"
         * }
         */
        JSONObject outBody = new JSONObject();
        outBody.put("title", "From Fedora!!!");
        outBody.put("type", "article");

        JSONObject bodyField = new JSONObject();
        JSONObject bodyValue = new JSONObject();
        JSONArray arr = new JSONArray();
        bodyValue.put("value", exchange.getIn().getBody(String.class));
        bodyValue.put("format", "plain_text");
        arr.add(bodyValue);
        bodyField.put("und", arr);
        outBody.put("body", bodyField);

        /*
         * Set up the out message to be a POST for the
         * subsequent call to Drupal's REST service.
         */
        Message outMessage = exchange.getOut();
        outMessage.setHeader(Exchange.HTTP_METHOD, POST);
        outMessage.setHeader(Exchange.CONTENT_TYPE, "application/json");
        outMessage.setBody(outBody.toJSONString());
    }
}
```

The comments in this file give it all away, but let's walk through it anyway.  The whole purpose of this file is to create a JSON message in the format that Drupal is expecting for the REST call to create a node.  So we construct a JSONObject and stuff RDF from Fedora (the in message's body) into it.  Then we construct an outgoing message, set the headers appropriately for the http call out to Drupal, and set the body to be the JSON we just made.

To keep things simple for the purposes of this example, we're leaving out some pretty important details, most notably authentication.  But as you can see, there's not much to it.  In fact, the most difficult part of assembling this example was setting up the Drupal Services module and figuring out what the format Drupal was expecting for the JSON message.

For more on Camel, check out the [Camel API] documentation and the community documentation on the [Camel][Apache Camel] website.  If you're looking for something more than just community and api docs, check out [Camel in Action].  It's well worth the money.

### Inversion of Control and Camel
Camel works very well with both the [Spring] and [Blueprint] Inversion of Control (e.g. Dependency Injection) frameworks.  The routes can even be defined directly in the application context xml's for either.  These sorts of frameworks, while both powerful and valuable, are often a stumbling point for developers who have never been exposed to them.  We will be using an inversion of control framework to bootstrap the application, but routes will be defined in the Java DSL.  It is also advisable to stick to the Camel API, extending custom Processors when extra functionality is needed.  Bean injection and delegation should only happen when interfaces and single inheritance cannot be utilized for code re-use.  We are attempting to keep the application context's xml as simple as possible.  Plus, let's be honest, non-programmers and managers aren't going to be manipulating the xml and redeploying if we expose bean references in this manner.  We're programmers, let's do as much in code as possible.

Aside from configuration and activemq setup, hopefully the application context can stay as simple as this for as long as possible:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<blueprint xmlns="http://www.osgi.org/xmlns/blueprint/v1.0.0"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:camel="http://camel.apache.org/schema/blueprint"
       xsi:schemaLocation="
       http://www.osgi.org/xmlns/blueprint/v1.0.0 http://www.osgi.org/xmlns/blueprint/v1.0.0/blueprint.xsd
       http://camel.apache.org/schema/blueprint http://camel.apache.org/schema/blueprint/camel-blueprint.xsd">

  <camelContext id="islandora-sync" xmlns="http://camel.apache.org/schema/blueprint">
    <package>org.islandora.sync.routes</package>
  </camelContext>

</blueprint>
```

### Camel and Scripting Languages
Camel provides extra functionality for scripting language integration, but it is advised to stick to using Camel's provided Simple language for basic filters, expressions, and predicates.  Anything more complicated than the Simple language can handle should be done in Java for compile-time safety and debugging purposes.  The other languages offered by Camel that look attractive (javascript, ruby, python) aren't *The Real McKoy*.  They are JVM implementations of said languages (Rhino, JRuby, Jython), and often have unexpected idiosyncrasies because they're being boiled down to JVM bytecode behind the scenes.  Being explicit with Java is less of a hassle than it may seem, particularly given the fact that you can attach a remote debugger in your IDE and troubleshoot so easily.

# *THE PLAN*
In order to avoid over-genericized and over-engineered code, we are simply going to map out space for each operation that must be performed on each type of resource based on message type and content model.  As work progresses and similarities present themselves, we will aggressively refactor in order to maintain code re-use.  But as experience has proven, attempting to make a single system that handles all use cases will only lead to deterioration over time as the assumptions of the generic system are violated with each new data type/format and use case.  We have to give each concept its own room in the code base so that things which at first appear similar can vary independently over the course of development.

### The Gateway
When messages first come in from Fedora through Activemq, there will be a sorting layer that will process each message so that it eventually winds up in the appropriate place.  The things we will have to sort on are:
- Resource type
  - Container
  - Binary
  - NonRdfSourceDescription
- Content Model (or content model of parent container)
  - Image
  - Pdf
  - Video
  - Audio
  - Newspaper
  - Book
  - etc...
- Metadata standard (for NonRdfSourceDescriptions)
  - MODS
  - MADS
  - PBCORE
  - etc...
- Operation
  - NODE_ADDED
  - PROPERTY_CHANGED
  - NODE_REMOVED
  - etc...

To achieve this, we'll have a handful routes that will behave essentially as switchyards using the [Message Router] pattern.  This can be implemented in Camel like so:
```Java
RouteBuilder builder = new RouteBuilder() {
    public void configure() {
        from("activemq:topic:fedora")
            .choice()
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_ADDED"))
                    .to("direct:create")
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_REMOVED"))
                    .to("direct:delete")
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "PROPERTY_CHANGED"))
                    .to("direct:update");
    }
};
```

### Derivative Creation
In order to interact with the various commandline programs utilized to create derivatives, we will take advantage of Camel's exec component, which passes the message body into the program that is executed through STDIN.  Here' a trivial example using the wordcount function in linux 'wc', demonstrating how to handle the results:
```Java
from("direct:exec")
.to("exec:wc?args=--words /usr/share/dict/words")
.process(new Processor() {
     public void process(Exchange exchange) throws Exception {
       // By default, the body is ExecResult instance
       assertIsInstanceOf(ExecResult.class, exchange.getIn().getBody());
       // Use the Camel Exec String type converter to convert the ExecResult to String
       // In this case, the stdout is considered as output
       String wordCountOutput = exchange.getIn().getBody(String.class);
       // do something with the word count
     }
});
```

### Drupal Interaction
Camel will interact with service exposed through Drupal with the use of the http4 component.  Common operations such as authentication, user management, and CRUD operations on Nodes can all be safely performed by the middleware layer without touching a drop of Drupal code.

See the example in the 'Using Camel' section for a look at how we create nodes remotely through the REST interface Drupal provides.

### The Drupal Modules
With so much of the core functionality being moved out of the Drupal layer, we'll see the Drupal modules we've grown accustomed to shrink in size.  The core purpose of the Drupal modules will be to:
- Provide custom Islandora content types (One-to-one with content models)
- Provide custom Islandora views
- Provide custom renderers for the access copy derivatives

It should be noted that although there will still exist a module for each content model, they will not be in separate git repos.  There is a difference between modularity of code and modulatity of revision control.  Managing some thirty odd git repos is a maintenance nightmare, and so you will see all code move into a single repository.  This will help eliminate commit mis-matches between modules, and will synchronize changes with the middlware layer as well.

[Apache Camel]:http://camel.apache.org/
[Enterprise Integration Patterns]:http://www.enterpriseintegrationpatterns.com/
[Camel Context]:http://camel.apache.org/camelcontext.html
[Route Builder]:http://camel.apache.org/routebuilder.html
[Route]:http://camel.apache.org/routes.html
[Message]:http://camel.apache.org/message.html
[Exchange]:http://camel.apache.org/exchange.html
[Camel API]:http://camel.apache.org/maven/current/camel-core/apidocs/index.html
[Camel in Action]:http://www.manning.com/ibsen/
[Processor]:http://camel.apache.org/processor.html
[Aaron Coburn]:https://github.com/acoburn
[fcrepo-camel]:https://github.com/fcrepo4/fcrepo-camel
[Mustache]:https://mustache.github.io/
[Spring]:https://spring.io/
[Blueprint]:http://aries.apache.org/modules/blueprint.html
[Message Router]:http://camel.apache.org/message-router.html

