package ca.islandora.services.routes.collection;

import static org.apache.camel.component.http4.HttpMethods.POST;

import org.apache.camel.Exchange;
import org.apache.camel.builder.RouteBuilder;

/**
 * Routes exposed by the CollectionEndpoint.
 * 
 * @author danny
 */
public class CollectionService extends RouteBuilder {

    /* (non-Javadoc)
     * @see org.apache.camel.builder.RouteBuilder#configure()
     */
    @Override
    public void configure() throws Exception {
        from("direct:createCollection")
            .description("Creates a Collection node in Fedora from a Drupal node.")
            .beanRef("collectionServiceProcessor", "deserializeNode")
            .beanRef("collectionServiceProcessor", "nodeToSparqlUpdate")
            .to("direct:createFedoraResource");
        
        from("direct:createFedoraResource")
            .description("Creates a resource in Fedora using a SPARQL update.")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, POST)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/sparql-update"))
            .to("fcrepo:{{fcrepo.baseurl}}");    
    }
}