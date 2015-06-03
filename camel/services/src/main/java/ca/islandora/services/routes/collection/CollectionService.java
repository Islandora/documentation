package ca.islandora.services.routes.collection;

import static org.apache.camel.component.http4.HttpMethods.GET;
import static org.apache.camel.component.http4.HttpMethods.POST;
import static org.apache.camel.component.http4.HttpMethods.PATCH;

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
        
        from("direct:getCollection")
            .description("Retrieves ld+json RDF metadata for the collection identified by the provided UUID")
            .beanRef("collectionServiceProcessor", "getCollectionSparqlQuery")
            .to("http4:{{triplestore.baseurl")
            .beanRef("collectionServiceProcessor", "deserializeMap")
            .beanRef("collectionServiceProcessor", "extractFedoraPathFromSparqlResults")
            .choice()
                .when(property("fedoraPath").isNotNull())
                    .to("direct:getFedoraResource")
                .otherwise()
                    .removeHeaders("*")
                    .setBody().simple("[\"Could not retrieve path to resource in Fedora for ${headers.uuid}.\"]")
                    .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
                    .setHeader(Exchange.HTTP_RESPONSE_CODE, constant(500));
        
        from("direct:getFedoraResource")
            .description("Gets ld+json metadata for a resource in fedora")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, GET)
            .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/ld+json"))
            .recipientList(simple("fcrepo:$simple{property.fedoraPath}"));
        
        from("direct:createCollection")
            .description("Creates a Collection node in Fedora from a Drupal node.")
            .beanRef("collectionServiceProcessor", "deserializeMap")
            .beanRef("collectionServiceProcessor", "nodeToSparqlUpdate")
            .to("direct:createFedoraResource");
        
        from("direct:createFedoraResource")
            .description("Creates a resource in Fedora using a SPARQL update.")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, POST)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/sparql-update"))
            .to("fcrepo:{{fcrepo.baseurl}}");
        
        from("direct:updateCollection")
            .description("Updates a Collection node in Fedora from a Drupal node.")
            .beanRef("collectionServiceProcessor", "deserializeMap")
            .setProperty("node").simple("${body}")
            .beanRef("collectionServiceProcessor", "extractFedoraPathFromNode")
            .choice()
                .when(property("fedoraPath").isNotNull())
                    .beanRef("collectionServiceProcessor", "nodeToSparqlUpdate")
                    .to("direct:updateFedoraResource")
                .otherwise()
                    .removeHeaders("*")
                    .setBody().simple("[\"Could not extract Fedora path from Drupal node JSON.\"]")
                    .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
                    .setHeader(Exchange.HTTP_RESPONSE_CODE, constant(500));
        
        from("direct:updateFedoraResource")
            .description("Updates a resource in Fedora using a SPARQL udpate")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, PATCH)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/sparql-update"))
            .recipientList(simple("fcrepo:${property.fedoraPath}"));
    }
}