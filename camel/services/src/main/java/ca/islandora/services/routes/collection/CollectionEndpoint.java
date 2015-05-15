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

        .get()
            .to("direct:derp")

        .post("/")
            .description("Creates a collection off the fcrepo root")
            .consumes("application/json")
            .produces("application/json")
            .to("direct:createCollection")
            
        .post("/{uuid}")
            .description("Creates a collection using uuid as parent.")
            .consumes("text/turtle")
            .produces("application/json")
            .to("direct:createCollection");


    }
}
