#Islandora 7.x-2.x Technical Design Doc

Islandora version 7.x-2.x is middleware built using Apache Camel to orchestrate distributed data processing and to provide web services required by institutions who would like to use Drupal as a front-end to a Fedora 4 JCR repository.  This goal presents a unique set of challenges, as Drupal is much, much more than a simple display layer.  It is a full blown content management system designed to be built on top of a traditional relational database such as MySQL or Postgres, not a JCR repository.  Additionally, there is a large amount of data processing and manipulation that must be performed for presentation and discovery.  This means that there is much more software that must be integrated than just Drupal and Fedora (Tesseract, ImageMagick, FFmpeg, just to name a few).  To make matters worse, doing all of this processing on the servers containing either Fedora or Drupal is detrimental to the performance of the overall system, resulting in a unusable site during periods of content migration or manipulation.  Plus, as most of us have already found out, systems such as these are incredibly difficult to install, configure, and maintain.

To mitigate these issues, the overall design goals of the 7.x-2.x version of Islandora are:
  - A properly modularized installation procedure so that Islandora can be consistently installed and configured in distributed environments.  As a result of this, a consistent development environment can also be made available to contributors.
  - Asynchronous communication between Fedora and Drupal, so that neither waits on the other nor any of the various processing components of the stack.  This will be achieved through the use of persistent queues, which will also allow the stack to be easily distributed across multiple computers.
  - Fedora is treated as the source of the truly important data, only containing preservation objects and descriptive metadata.  Metadata can exist either in Fedora's native RDF attached to the resource itself, or as standardized formats such as MODS, MADS, PBCore, etc., that exist as resources in their own right.  
  - Data from Fedora is transformed and indexed into the other major system of the stack, most notably Drupal and Solr.  This includes lower quality access copies of preservation masters such as thumbnails or streaming video, which will be stored as managed files for Drupal.
  - Drupal content is represented as Nodes and Fields, allowing the content management system to utilize the relational database it is expecting instead of shimming in a completely different type of datastore.  This will open up the entire Drupal module ecosystem to Islandora.  As an added benefit, viewers (OpenSeadragon, IA Book Viewer, Video.js, etc...) can be written as custom Field renderers, finally giving site builders the ability to control the display of content.  
  - Drupal's Services module will be used to expose RESTful services to middleware layer so that it can sanely perform CRUD operations on Nodes without having to delve into Drupal's internals.

### The Importance of Using an Integration Framework
Let's not mince words.  *Islandora is middleware, warts and all.*  The word 'middleware' has plenty of connotations and baggage, but it really is what we're doing.  We have a huge stack with a lot of moving parts, and we have to glue them all together.  So it only makes sense to adopt a integration framework to help us pull this off.  

The framework that has been chosen for the project is [Apache Camel].  It seeks to provide implementations of the fundamental design patterns codified by Gregor Hohpe and Bobby Woolf in their book [Enterprise Integration Patterns].  The best way to describe these patterns is that they're standardized, re-usable templates for processing messages that have to flow through multiple pieces of software before arriving at their final destination.  So Camel will help us do things like route messages from Fedora's queue/topic to the appropriate handling function for an operation on a particular content type, allowing it to be processed along the way by derivative generation tools on the command line.  In addition to this, Camel provides code for interacting with software through basically any protocol you can think of, so we don't have to waste our time writing code for common situations like posting to http endpoints, reading data from files, polling queues, etc...  It also has fantastic support for try/catch exception handling across distributed systems and transactional functionality.  

But perhaps the greatest advantage of using an integration framework is that lets us focus solely on the application logic that's important to Islandora.  *There is no need for us to engineer any generic systems to get our job done, because we already have one!*  We can identify the operations that need to happen for every supported content type, carve out a space to do the work, and get to it.  It's not particularly sexy, but the work we have to do is difficult and we've already got enough on our plate!  If there's a need to provide pluggable solutions for other Fedora users (Hydra, custom Fedora solutions), we can refactor and properly use bean injection to do so.  Using an integration framework gives us all the power we need, and then some.

### Using Camel
Camel, which at first glance appears as terrifying as Java IOC frameworks (more on those later), is actually incredibly straightforward.  A Camel application is known as a [Camel Context], which is really just a collection of messaging [Route]s.  These routes are defined in [Route Builder] classes.  Each route has a starting point (the from() method), from which an initial [Message] is consumed.  The Message is placed in an [Exchange], which contains two messages: one incoming, and another outgoing.  As the Exchange is passed through each step of the route, the outgoing message from one step becomes the incoming message of the next.  Data that must persist between multiple steps in the route can be cached in the Exchange as properties.

That's pretty much it.  Seriously.

Camel provides most of the functionality we need to work with these routes out of the box, and has built-ins for message routing, filtering, data extraction with XPath, transformations with XSLTs, and much more.  If you need something beyond what is offered by default, you can make your own custom [Processor]s, which have unfettered access to the Exchange for whatever custom logic you desire.  Heck, you can even define Processors as anonymous subclasses on the fly within the RouteBuilder.  It looks almost like Javascript!

But perhaps the best part of using Camel is that [Aaron Coburn] has already created [fcrepo-camel].  This Camel component makes working with Fedora's REST API and JMS Messages incredibly easy.  In fact, it is essentially a replacement for Tuque using Java, allowing us to efficiently work with Fedora within a Camel Context.  Everyone involved in the project now officially owes Aaron a beer, since we no longer have to maintain software for one of our most basic needs :)

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

One of the first things you'll notice when looking at Camel routes is that they almost look as if they're written in English.  This makes them incredibly easy to follow.  Without knowing anything about Camel, you should be able to tell that this route is named "fedoraIn", reads a message from Fedora's ActiveMQ topic, filters out messages that aren't NODE_ADDED, fetches data from Fedora, transforms it into JSON, and sends it off to Drupal.  Phew!  

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

For more on Camel, check out the [Camel API] documentation and the community documentation on the [Camel][Apache Camel] website.  If you're looking for something more than just community and API docs, check out [Camel in Action].  It's well worth the money.

### Inversion of Control and Camel
Camel works very well with both the [Spring] and [Blueprint] Inversion of Control (e.g. Dependency Injection) frameworks.  The routes can even be defined directly in the application context XML's for either.  These sorts of frameworks, while both powerful and valuable, are often a stumbling point for developers who have never been exposed to them.  We will be using the Spring inversion of control framework to bootstrap the application, but routes will be defined in the Java DSL.  It is also advisable to stick to the Camel API, extending custom Processors when extra functionality is needed.  Bean injection and delegation should only happen when interfaces and single inheritance cannot be utilized for code re-use, or to provide a pluggable system (but *only if it's truly necessary*).  We are attempting to keep the application context's XML as simple as possible, and in order to do so, the exposure of beans must be minimal.  Plus, let's be honest, non-programmers and managers aren't going to be manipulating the XML and redeploying if we expose every possible bean reference.  We're programmers, let's do as much in code as possible for all the benefits it provides (code completion, compile time safety, debugging, etc...).

### Camel and Scripting Languages
Camel provides extra functionality for scripting language integration, but it is advised to stick to using Camel's provided Simple language for basic filters, expressions, and predicates.  Anything more complicated than the Simple language can handle should be done in Java for compile-time safety and debugging purposes.  The other languages offered by Camel that look attractive (Javascript, ruby, python) aren't *The Real McKoy*.  They are JVM implementations of said languages (Rhino, JRuby, Jython), and often have unexpected idiosyncrasies because they're being boiled down to JVM bytecode behind the scenes.  Being explicit with Java is less of a hassle than it may seem, particularly given the fact that you can attach a remote debugger in your IDE and troubleshoot so easily.

# *THE PLAN*
The integration of all the various subsystems will be achieved through asynchronous middleware.  This means that Drupal will never explicitly call out to Fedora, and Fedora will never explicitly call out to Drupal.  The middleware will consume messages from Fedora's event queue and utilize Drupal services for information flow from Fedora to Drupal.  For information flow from Drupal to Fedora, Drupal will use services provided by the Camel middeware layer that encapsulate complicated data manipulation and Fedora logic within a single transaction.  This will all be done using Drupal's hook system (which is an event system in a sense), allowing a developer to manipulate node content as if they were working on a normal Drupal site, with operations on Fedora content getting seamlessly deferred to the middleware.  In order to maintain asynchronous behavior, Camel services will accept requests from Drupal, but immediately publish request content as messages onto a persistent queue so that work can be performed when and where it is appropriate.  For those who like diagrams, the following image represents this flow of information between the main three layers of the stack.

![Layer Interaction](https://raw.githubusercontent.com/wiki/Islandora-Labs/islandora/images/layer-interaction.gif)

Setting up the software in this manner effectively decouples Fedora and Drupal from each other, both in terms of synchronicity and application logic.  The benefits to this are deep, resulting in:
- A system that can scale out horizontally because of the use of persistent queues, adapting to heavier load.
- A system that offers a better user experience due to decreased wait times for page loads.  Only interacting with Fedora when create, update, and deletion operations will prevent us from becoming disk bound due to a high volume of unnecessary read requests to Fedora.
- The entire Drupal module ecosystem is opened up for Islandora developers, themers, and site builders.  Now when someone says, "But there's a Drupal module for that, right?", we no longer have to sheepishly respond with "Well... that's not how it really works..."
- Tuque as an object relational mapper for Fedora is no longer required.  Drupal's Node system, which is an ORM that can be configured through a UI, in combination with the fcrepo-camel component can replace this functionality in its entirety.
- The ability to utilize Drupal for and its relational database for application logic, avoiding the usual hacks such as temporary RELS-EXT statements and an over-reliance on Solr.
- A generic viewer framework!  Our usual js viewers for objects can be rewritten as custom Drupal field renderers, allowing site builders to finally have the control over page display they've always wanted.
- XML Forms is no longer required.  Between [xpath_field](https://github.com/alxp/xpath_field) and something like [jquery.xmleditor](https://github.com/UNC-Libraries/jquery.xmleditor) or [doctored.js](http://holloway.co.nz/doctored/), xml metadata can be edited and extracted in a manner which is configurable and ties directly into Drupal's display system.
- GSearch is no longer required.  The fcrepo-camel component can replace this functionality with a few lines of code.  Though it is unlikely that xslts will be eliminated from the stack altogether, at least the massive xslt layer in the current stack can be broken apart in a reasonable manner.  Also, the programmer will have access points to provide custom logic before and after processing.  This makes complicated indexing scenarios like those that require information from multiple objects accessible to a wider audience.

### Islandora Sync
When designing systems, it's tempting to be drawn towards what appear to be similarities between requirements at first.  This often leads to a design built around code re-use that seems obvious in the early stages of development.  Unfortunately, as functional requirements shift and use cases are added, more and more of the initial assumptions will be violated.  Over time, this design will start to sag, and its "generic" systems will become more of a hindrance than a benefit.  With this in mind for the middleware, we are simply going to map out space for each operation that must be performed on each type of resource based on message type, content model, and MIME type.  As work progresses and similarities present themselves, we will aggressively refactor in order to maintain code re-use.  But as experience has proven, attempting to make a large systems that handle all use cases will only lead to deterioration, as one by one the initial assumptions will fall by the way side.  We have to give each concept its own room in the code base so that things which at first appear similar can vary independently over the course of development.  This will also allow for eventual bean exposure so that the system can be made pluggable for users with custom use cases.  Ironically, it is by being explicit that we will achieve a more robust and customizable middleware.  

When messages first come in from Fedora through ActiveMQ, there will be a sorting layer that will process each message so that it eventually winds up in the appropriate place.  The things we will have to sort on are:
- Resource type
  - Container
  - NonRdfSourceDescription
- Content Model (or content model of parent container)
  - Image
  - PDF
  - Video
  - Audio
  - Newspaper
  - Book
  - Page
  - Compound
  - Web ARChive
  - Disk image
- Metadata standard (for NonRdfSourceDescriptions)
  - MODS
  - MADS
  - PBCore
  - TECHMD
  - etc.
- Operation
  - NODE_ADDED
  - PROPERTY_CHANGED
  - NODE_REMOVED

To achieve this, we'll have a handful routes that will behave essentially as switchyards using the [Message Router] pattern.  For example, routing based on Fedora event type can be implemented in Camel like so:
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
Of course, this is a simplification for illustrative purposes.  In reality, there will be several of these wired together based on all the criteria presented above in order to route a message to its proper handler.

### Derivative Creation
In order to interact with the various command line programs utilized to create derivatives, we will take advantage of Camel's exec component, which passes the message body into the program that is executed through STDIN.  Here' a trivial example using the word count function in Linux `wc`, demonstrating how to handle the results:
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
Camel will interact with service exposed through Drupal with the use of the http4 component.  Common operations such as authentication, user management, and CRUD operations on Nodes can all be safely performed by the middleware layer without touching a drop of Drupal code (forgive the terrible pun).

In fact, the main role of the Islandora Drupal module is to provide the REST endpoint and define any common services that will be used independent of content type.  The service module acts much like Drupal Views, allowing the endpoint to be customized in through a UI, and code to be exported and inserted into the Drupal module to provide permanency beyond that of the database.

See the example in the 'Using Camel' section for a look at how we create nodes remotely through the REST interface Drupal provides.

### The Drupal Modules
With so much of the core functionality being moved out of the Drupal layer, we'll see the Drupal modules we've grown accustomed to shrink in size.  The core purpose of the Drupal modules will be to:
- Provide custom Islandora content types (One-to-one with content models)
- Provide custom Islandora views
- Provide custom renderers for the access copy derivatives
- Expose custom services for the middleware layer

It should be noted that although there will still exist a module for each content model, they will not be in separate git repos.  There is a difference between modularity of code and modularity of revision control.  Managing some thirty odd git repos is a maintenance nightmare, and so you will see all code move into a single repository.  This will help eliminate commit mis-matches between modules, and will synchronize changes with the middleware layer as well.  Over time, as the code base grows, we can consider moving out larger pieces for specific reasons (getting the Islandora modules on drupal.org, for instance), and using git submodules to replace them in the single canonical Islandora Github repository.

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
