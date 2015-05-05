package ca.islandora.services.routes;

import org.apache.camel.builder.RouteBuilder;

public class CollectionEndpoint extends RouteBuilder {

    @Override
    public void configure() throws Exception {
        rest("/collection")

        .post("/{uuid}")
            .description("Creates a collection.  Uses optional uuid as parent.")
            .to("direct:createCollection");

        from("direct:createCollection")
            .transacted()
            .beanRef("postCollectionProcessor")
            .to("fcrepo:{{fcrepo.baseurl}}")
            .log("HEADERS: ${headers}")
            .log("BODY: ${body}");
    }
}
