package ca.islandora.services.routes;

import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.model.dataformat.JsonLibrary;


public class CollectionEndpoint extends RouteBuilder {

    @Override
    public void configure() throws Exception {
        rest("/collection/")

        .post("/")
            .description("Creates a collection off the fcrepo root.")
            .consumes("text/turtle")
            .produces("application/json")
            .to("direct:createCollection")
            
        .post("/{uuid}")
            .description("Creates a collection using uuid as parent.")
            .consumes("text/turtle")
            .produces("application/json")
            .to("direct:createCollection");

        from("direct:createCollection")
            .transacted()
            .beanRef("collectionServiceProcessor", "processForDrupalPOST")
            .recipientList(simple("http4:{{drupal.baseurl}}/node/${property.collectionUUID}"))
            .beanRef("collectionServiceProcessor", "processForFedoraPOST")
            .to("seda:toFedora")
            .beanRef("collectionServiceProcessor", "processForHibernatePOST")
            .to("hibernate:ca.islandora.services.uuid.UUIDMap")
            .marshal().json(JsonLibrary.Jackson);
        
        // Duck out of the transacted thread for Fedora call so TransactionManagers don't collide.
        from("seda:toFedora")
            .to("fcrepo:{{fcrepo.baseurl}}");
    }
}
