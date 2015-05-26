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
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_ADDED"))
                    .to("direct:handleNodeAddedEvent")
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "PROPERTY_ADDED"))
                    .to("direct:handlePropertyAddedEvent")
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "PROPERTY_CHANGED"))
                    .to("direct:handlePropertyChangedEvent")
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "PROPERTY_REMOVED"))
                    .to("direct:handlePropertyRemovedEvent")
                .when(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_REMOVED"))
                    .to("direct:handleNodeRemovedEvent")
                .otherwise()
                    .log(LoggingLevel.DEBUG, "SKIPPING EVENT: UNDETERMINED EVENT TYPE")
                    .log(LoggingLevel.DEBUG, "HEADERS OF SKPPIED EVENT: ${headers}");
        
        from("direct:handleNodeAddedEvent")
            .routeId("nodeAddedEvent")
            .to("direct:fedoraGetRdf")
            .setProperty("rdf", body(Map.class))
            .process(new UUIDExtractor())
            .choice()
                .when(property("uuid").isNotNull())
                    .to("direct:drupalUpsertNodeFromRdf")
                .otherwise()
                    .log(LoggingLevel.DEBUG, "SKIPPING EVENT: NO UUID");
        
        from("direct:handlePropertyAddedEvent")
            .routeId("propertyAddedEvent")
            .log("PROPERTY ADDED NOT IMPLEMENTED YET");
        
        from("direct:handlePropertyChangedEvent")
            .routeId("propertyChangedEvent")
            .log("PROPERTY CHANGED NOT IMPLEMENTED YET");
        
        from("direct:handlePropertyRemovedEvent")
            .routeId("propertyRemovedEvent")
            .log("PROPERTY REMOVED NOT IMPLEMENTED YET");
        
        from("direct:handleNodeRemovedEvent")
            .routeId("nodeRemovedEvent")
            .log("NODE REMOVED NOT IMPLEMENTED YET");
        

    }
}
