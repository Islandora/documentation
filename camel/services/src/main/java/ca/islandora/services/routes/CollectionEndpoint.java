package ca.islandora.services.routes;

import org.apache.camel.builder.RouteBuilder;

public class CollectionEndpoint extends RouteBuilder {

    @Override
    public void configure() throws Exception {
        rest("/collection/")

        .post("/{uuid}")
            .description("Creates a collection using uuid as parent.")
            .produces("application/json")
            .to("direct:createCollection");

        from("direct:createCollection")
//            .transacted()
            .beanRef("collectionServiceProcessor", "processForDrupalPOST")
            .recipientList(simple("http4:{{drupal.baseurl}}/node/${property.collectionUUID}"))
            .beanRef("collectionServiceProcessor", "processForFedoraPOST")
            .to("fcrepo:{{fcrepo.baseurl}}")
            .beanRef("collectionServiceProcessor", "processForHibernatePOST")
            .to("hibernate:ca.islandora.services.uuid.UUIDMap");
    }
}
