# Islandora Commands

## Introduction
Sometimes you need to do deeper logic in a Camel route than can realistically be done within a Blueprint xml.  Since Apache Camel is an *integration* framework, we can call out to command line PHP instead of having to use Java for the majority of our tasks.  Though Java is still always an option to those who are inclined (and may be required for more advanced use cases), there are many advantages to sticking with PHP.  Not only is it the language we're most comfortable with as a community, but its also well suited for dealing with both ld+json from Fedora and node content from Drupal (much more so than Java).  We also get the full advantages of modern PHP, including object oriented programming and tools like Composer.    

## Basic Usage
In your git project's root, head on over to the `bin` folder of the `camel/commands` directory and execute the `islandora.php` script to see what it has to say.
```bash
vagrant@islandora:~/islandora$ cd camel/commands/bin
vagrant@islandora:~/islandora/camel/commands/bin$ php islandora.php
Islandora Command Tool version 0.0.0-SNAPSHOT

Usage:
  command [options] [arguments]

Options:
  -h, --help            Display this help message
  -q, --quiet           Do not output any message
  -V, --version         Display this application version
      --ansi            Force ANSI output
      --no-ansi         Disable ANSI output
  -n, --no-interaction  Do not ask any interactive question
  -v|vv|vvv, --verbose  Increase the verbosity of messages: 1 for normal output, 2 for more verbose output and 3 for debug

Available commands:
  help                            Displays help for a command
  list                            Lists commands
 collectionService
  collectionService:nodeToSparql  Converts Drupal node JSON for a collection to a SPARQL Update query
 rdf
  rdf:createNode                  Creates a Drupal node from Fedora RDF.
  rdf:extractContentType          Extracts a Drupal content type from Fedora RDF.
  rdf:updateNode                  Updates a Drupal node from Fedora RDF.
```
Wow, that's a lot.  How did we get all that nice output?  It's because we're using the Symfony Console component as a framework for our script.  We get all kinds of stuff for free because of it, including versioning, exception catching, and this helpful message.

As you can see, this tool expects a command to be provided.  Notice that the commands are even namespaced!  Normally, commands would accept arguments and parameters optionally after the command.  But from Camel, message bodies are piped to the tool using STDIN, which the tool will gladly accept.  Let's try running a command that will convert a node represented in JSON from Drupal into a SPARQL update query.  First, we'll save the input to a file so that when we run the command we can just dump its contents into STDIN to emulate how Camel behaves.  Go find a node on your site and get its UUID by looking at its devel tab.  

PUT PICTURE OF DEVEL TAB HERE

You can get its JSON representation by visiting http://yoursite/islandora/node/your_nodes_uuid.  If you're running on the vagrant environment, and have a uuid of 38f7d551-28f7-49ac-9ceb-d6adcc6cc9c0, then the url would look like `localhost:8000/islandora/node/38f7d551-28f7-49ac-9ceb-d6adcc6cc9c0`.

PUT A PICTURE OF THE JSON OUTPUT HERE

Save the response somewhere, like `/tmp/node.json`. Then you can run the command like so:
```bash
vagrant@islandora:~/islandora/camel/commands/bin$ cat /tmp/node.json | php islandora.php collectionService:nodeToSparql
PREFIX islandora: <http://islandora.ca/ontology/v2/>
PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
PREFIX fedora: <http://fedora.info/definitions/v4/repository#>
PREFIX pcdm: <http://pcdm.org/models#>
PREFIX nfo: <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/>
PREFIX nt: <http://www.jcp.org/jcr/nt/1.0>
PREFIX mix: <http://www.jcp.org/jcr/mix/1.0>
PREFIX ldp: <http://www.w3.org/ns/ldp#>
PREFIX dc11: <http://purl.org/dc/elements/1.1/>
PREFIX modsrdf: <http://www.loc.gov/mods/modsrdf/v1#>
PREFIX content: <http://purl.org/rss/1.0/modules/content/>
PREFIX dc: <http://purl.org/dc/terms/>
PREFIX foaf: <http://xmlns.com/foaf/0.1/>
PREFIX og: <http://ogp.me/ns#>
PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>
PREFIX sioc: <http://rdfs.org/sioc/ns#>
PREFIX sioct: <http://rdfs.org/sioc/types#>
PREFIX skos: <http://www.w3.org/2004/02/skos/core#>
PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>

DELETE WHERE {
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> ?o0 .
    <> <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/uuid> ?o1 .
    <> <http://purl.org/dc/terms/title> ?o2 .
    <> <http://purl.org/dc/terms/date> ?o3 .
    <> <http://purl.org/dc/terms/created> ?o4 .
    <> <http://purl.org/dc/terms/modified> ?o5 .
    <> <http://rdfs.org/sioc/ns#has_creator> ?o6 .
    <> <http://xmlns.com/foaf/0.1/name> ?o7 .
};
INSERT DATA {
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/ns/ldp#RDFSource> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/ns/ldp#Container> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.jcp.org/jcr/nt/1.0hierarchyNode> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.jcp.org/jcr/nt/1.0folder> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.jcp.org/jcr/nt/1.0base> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.jcp.org/jcr/mix/1.0referenceable> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://fedora.info/definitions/v4/repository#Resource> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://fedora.info/definitions/v4/repository#Container> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://pcdm.org/models#Collection> .
    <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://islandora.ca/ontology/v2/collection> .
    <> <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/uuid> "38f7d551-28f7-49ac-9ceb-d6adcc6cc9c0" .
    <> <http://purl.org/dc/terms/title> "Test Collection" .
    <> <http://purl.org/dc/terms/date> "1970-01-01T00:33:35+00:00"^^<http://www.w3.org/2001/XMLSchema#dateTime> .
    <> <http://purl.org/dc/terms/created> "1970-01-01T00:33:35+00:00"^^<http://www.w3.org/2001/XMLSchema#dateTime> .
    <> <http://purl.org/dc/terms/modified> "2015-07-16T13:49:27+00:00"^^<http://www.w3.org/2001/XMLSchema#dateTime> .
    <> <http://rdfs.org/sioc/ns#has_creator> 1 .
    <> <http://xmlns.com/foaf/0.1/name> "admin" .
}
```

And you can see that we have our SPARQL output.  Executing commands like this is a great way to test without having to trigger anything via Camel.

If you are within a Camel route in the Blueprint DSL, the message body is automatically provided to the command, and you can utilize the Islandora component to execute one like so:
```xml
<to uri="islandora:namespace:command"/>
```

For example, in order to run the same command we just did from inside Camel, you can simply do this (assuming the message body is the same JSON data from Drupal services):
```xml
<to uri="islandora:collectionService:nodeToSparql"/>
```

See the documentation on the Islandora command for more information about using a command from within Camel.

## Creating a new Command
Let's go through the steps required to make your own Camel command.  As is tradition, we're going to make a simple "Hello World" command.

All that's required for creating a command is to extend IslandoraCommand (or one of its subclasses).  It already handles everything for you, including accepting input from STDIN.  The bare minimum that is required of an IslandoraCommand is to implement two methods: `configure()` and `execute(InputInterface $input, OutputInterface $output)`.  From your project's root, create the `camel/commands/src/Greeter` directory and open up a new file called `HelloGreeter.php`.  Place the following code inside and be sure to save it to the newly created `Greeter` directory.
```php

<?php

/**
 * This file is part of Islandora.
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * PHP Version 5.5.9
 *
 * @category Islandora
 * @package  Islandora\Greeter
 * @author   Your Name <your_email@your_employer.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */

use Islandora\IslandoraCommand;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Says hello!
 *
 * @category Islandora
 * @package  Islandora\Greeter
 * @author   Your Name <your_email@your_employer.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
class HelloGreeter extends IslandoraCommand
{
    /**
     * Sets command configuration.
     *
     * @return null
     */
    protected function configure()
    {
        $this->setName('greeter:hello')
            ->setDescription(
                "Says hello!"
            );
    }

    /**
     * Says hello!
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $this->data;
        $output->writeln("Hello " . trim($name) . "!");
    }
}
```
In the `configure()` method, we set some properties for the command.  A command's setters all return the command itself, so we can chain the calls together.  We set the name of the command, complete with namespace and ':' delimiter.  We also give a description of the command that will show up when listing available commands.

The `execute(InputInterface $input, OutputInterface $output)` method is where we do our actual work.  Since we extended Islandora command, all input from STDIN is provided as a String and can be accessed any time through `$this->data`.

So let's try it out!  Go back to the `camel/commands/bin` directory and get a listing of all the commands to make sure it appears.

```bash
vagrant@islandora:~/islandora/camel/commands/bin$ cd ~/islandora/camel/commands/bin
vagrant@islandora:~/islandora/camel/commands/bin$ php islandora.php
Islandora Command Tool version 0.0.0-SNAPSHOT

Usage:
  command [options] [arguments]

Options:
  -h, --help            Display this help message
  -q, --quiet           Do not output any message
  -V, --version         Display this application version
      --ansi            Force ANSI output
      --no-ansi         Disable ANSI output
  -n, --no-interaction  Do not ask any interactive question
  -v|vv|vvv, --verbose  Increase the verbosity of messages: 1 for normal output, 2 for more verbose output and 3 for debug

Available commands:
  help                            Displays help for a command
  list                            Lists commands
 collectionService
  collectionService:nodeToSparql  Converts Drupal node JSON for a collection to a SPARQL Update query
 greeter
  greeter:hello                   Says hello!
 rdf
  rdf:createNode                  Creates a Drupal node from Fedora RDF.
  rdf:extractContentType          Extracts a Drupal content type from Fedora RDF.
  rdf:updateNode                  Updates a Drupal node from Fedora RDF.
```
And sure enough, it does!  We now have the `greeter:hello` command from the `greeter` namespace!  Let's use it!  As is tradition, we're going to greet the world.
```bash
vagrant@islandora:~/islandora/camel/commands/bin$ echo "World" | php islandora.php greeter:hello
Hello World!
```

## Working with JSON data
Most of the time, we're dealing with JSON output.  Often, we're working with node data from Drupal or ld+json RDF from Fedora 4.  As a convienence, the JsonInputIslandoraCommand can be extended instead of IslandoraCommand.  With a JsonInputIslandoraCommand, `$this->data` is an associative array that is automatically parsed from JSON input through STDIN.  Let's trying writing another Greeter, only this time it will accept JSON input of the form `{"name": "some_name"}`.
```php
<?php

/**
 * This file is part of Islandora.
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * PHP Version 5.5.9
 *
 * @category Islandora
 * @package  Islandora\Greeter
 * @author   Your Name <your_email@your_employer.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */

use Islandora\JsonInputIslandoraCommand;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Says hello!  Accepts JSON data in the form {"name" : "your_name"}.
 *
 * @category Islandora
 * @package  Islandora\Greeter
 * @author   Your Name <your_email@your_employer.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.en.html GPL
 * @link     http://www.islandora.ca
 */
class JsonHelloGreeter extends JsonInputIslandoraCommand
{
    /**
     * Sets command configuration.
     *
     * @return null
     */
    protected function configure()
    {
        $this->setName('greeter:helloUsingJson')
            ->setDescription(
                'Says hello!  Accepts JSON data in the form {"name" : "your_name"}.'
            );
    }

    /**
     * Says hello!  Accepts JSON data in the form {"name" : "your_name"}.
     *
     * @param InputInterface  $input  An InputInterface instance
     * @param OutputInterface $output An OutputInterface instance
     *
     * @return null
     */
    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $this->data['name'];
        $output->writeln("Hello " . trim($name) . "!");
    }
}
```
This command is almost exactly the same as the last, except it extends JsonInputIslandoraCommand and its execute function pulls the name of the person to greet out of `$this->data` using the key 'name'.

Let's make sure the command is available.
```bash
vagrant@islandora:~/islandora/camel/commands/bin$ php islandora.php
Islandora Command Tool version 0.0.0-SNAPSHOT

Usage:
  command [options] [arguments]

Options:
  -h, --help            Display this help message
  -q, --quiet           Do not output any message
  -V, --version         Display this application version
      --ansi            Force ANSI output
      --no-ansi         Disable ANSI output
  -n, --no-interaction  Do not ask any interactive question
  -v|vv|vvv, --verbose  Increase the verbosity of messages: 1 for normal output, 2 for more verbose output and 3 for debug

Available commands:
  help                            Displays help for a command
  list                            Lists commands
 collectionService
  collectionService:nodeToSparql  Converts Drupal node JSON for a collection to a SPARQL Update query
 greeter
  greeter:hello                   Says hello!
  greeter:helloUsingJson          Says hello!  Accepts JSON data in the form {"name" : "your_name"}.
 rdf
  rdf:createNode                  Creates a Drupal node from Fedora RDF.
  rdf:extractContentType          Extracts a Drupal content type from Fedora RDF.
  rdf:updateNode                  Updates a Drupal node from Fedora RDF.
```
Once you've confirmed that `greeter:helloUsingJson` is available, you can run it using:
```bash
vagrant@islandora:~/islandora/camel/commands/bin$ echo '{"name" : "World"}' | php islandora.php greeter:helloUsingJson
Hello World!
```
And there you have it!  That's how you can create your own commands for use in Camel routes, and also how to experiment with them outside of Camel!  Remember, when experimenting with more complicated data, you'll probably want to pipe the contents of a file to the command like we did above.
