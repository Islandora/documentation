package ca.islandora.services.routes.collection;

import org.apache.camel.builder.RouteBuilder;

/**
 * Exposes the Collection service.
 * 
 * @author danny
 */
public class CollectionEndpoint extends RouteBuilder {

    /* (non-Javadoc)
     * @see org.apache.camel.builder.RouteBuilder#configure()
     */
    @Override
    public void configure() throws Exception {
        rest("/collection/")
        .post("/")
            .description("Creates a collection off the fcrepo root")
            .consumes("application/json")
            .produces("application/json")
            .to("direct:createCollection")
        .put("/{uuid}")
            .description("Updates a collection in Fedora using Drupal node data")
            .consumes("application/json")
            .produces("application/json")
            .to("direct:updateCollection");
    }
}
