package ca.islandora.sync.routes;

import java.util.Map;

import org.apache.camel.LoggingLevel;
import org.apache.camel.builder.RouteBuilder;
import org.fcrepo.camel.FcrepoHeaders;
import org.fcrepo.camel.JmsHeaders;
import org.fcrepo.camel.RdfNamespaces;

import ca.islandora.sync.processors.UUIDExtractor;

public class FedoraEventGateway extends RouteBuilder {
    public void configure() throws Exception {

        from("activemq:topic:fedora")
            .routeId("fedoraEventGateway")
            .setProperty(FcrepoHeaders.FCREPO_BASE_URL, header("org.fcrepo.jms.baseURL"))
            .setProperty(FcrepoHeaders.FCREPO_IDENTIFIER, header("org.fcrepo.jms.identifier"))
            .choice()
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_REMOVED"))
                    .to("direct:handleNodeRemovedEvent")
                .otherwise()
                    .to("direct:handleNodeUpsertEvent");

        from("direct:handleNodeUpsertEvent")
            .routeId("nodeUpsertEvent")
            .to("direct:fedoraGetRdf")
            .setProperty("rdf", body(Map.class))
            .process(new UUIDExtractor())
            .choice()
                .when(property("uuid").isNotNull())
                    .to("direct:drupalUpsertNodeFromRdf")
                .otherwise()
                    .log(LoggingLevel.DEBUG, "SKIPPING EVENT: NO UUID");

        from("direct:handleNodeRemovedEvent")
            .routeId("nodeRemovedEvent")
            .log("NODE REMOVED NOT IMPLEMENTED YET");
    }
}
