package ca.islandora.services.routes.collection;

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
            .log("BODY: ${body}");
//            .beanRef("collectionServiceProcessor", "deserializeNode")
//            .beanRef("collectionServiceProcessor", "constructSparql")
//            .log("SPARQL: ${body}")
//            .to("fcrepo:{{fcrepo.baseurl}}")
//            .log("RESULTS: ${body}");
    }

}
