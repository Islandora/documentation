package ca.islandora.sync.routes;

import java.util.Map;

import javax.servlet.ServletException;

import static org.apache.camel.component.http4.HttpMethods.GET;
import static org.apache.camel.component.http4.HttpMethods.POST;
import static org.apache.camel.component.http4.HttpMethods.PUT;

import org.apache.camel.Exchange;
import org.apache.camel.builder.RouteBuilder;
import org.fcrepo.camel.FcrepoHeaders;
import org.fcrepo.camel.JmsHeaders;
import org.fcrepo.camel.RdfNamespaces;

public class DrupalNodeCreate extends RouteBuilder {
    public void configure() throws Exception {

        from("activemq:topic:fedora")
            .routeId("fedoraIn")
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
                    .log("CANNOT DETERMINE HOW TO HANDLE EVENT: ${headers}");
        
        from("direct:handleNodeAddedEvent")
            .routeId("nodeAddedEvent")
            .to("direct:getFedoraRdf")
            .setProperty("rdf", body(Map.class))
            .setProperty("uuid").simple("${property.rdf['http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/uuid']?[0]['@value']}")
            .choice()
                .when().simple("${property.uuid} != null")
                    .to("direct:updateNodeFromFedoraRdf")
                .otherwise()
                    .to("direct:createNodeFromFedoraRdf");
        
        from("direct:handlePropertyAddedEvent")
            .log("PROPERTY ADDED NOT IMPLEMENTED YET");
        
        from("direct:handlePropertyChangedEvent")
            .log("PROPERTY CHANGED NOT IMPLEMENTED YET");
        
        from("direct:handlePropertyRemovedEvent")
            .log("PROPERTY REMOVED NOT IMPLEMENTED YET");
        
        from("direct:handleNodeRemovedEvent")
            .log("NODE REMOVED NOT IMPLEMENTED YET");
        
        from("direct:getFedoraRdf")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, GET)
            .setHeader(FcrepoHeaders.FCREPO_BASE_URL, property(FcrepoHeaders.FCREPO_BASE_URL))
            .setHeader(FcrepoHeaders.FCREPO_IDENTIFIER, property(FcrepoHeaders.FCREPO_IDENTIFIER))
            .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/ld+json"))
            .to("fcrepo:localhost:8080/fcrepo/rest")
            .beanRef("drupalNodeCreateProcessor", "deserializeRdf");
        
        from("direct:updateNodeFromFedoraRdf")
            .to("direct:drupalAuthenticate")
            .log("I MADE IT THROUGH AUTHENTICATION HELL!!!")
            .to("direct:getNode")
            .beanRef("drupalNodeCreateProcessor", "deserializeMap")
            .beanRef("drupalNodeCreateProcessor", "updateNodeFromRdf")
            .beanRef("drupalNodeCreateProcessor", "serializeNode")
            .to("direct:updateNode");
            
        from("direct:createNodeFromFedoraRdf")
            //.beanRef("drupalNodeCreateProcessor", "createNodeFromRdf")
            .log("CREATE NODE FROM FEDORA RDF NOT IMPLEMENTED YET");
                  
        from("direct:getNode")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, GET)
            .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/json"))
            .setHeader("X-CSRF-Token").simple("${property.token}")
            .setHeader("Cookie").simple("${property.cookie}")
            .setBody().simple("${null}")
            .recipientList(simple("http4:{{drupal.baseurl}}/node/$simple{property.uuid}"));
        
        from("direct:updateNode")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, PUT)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
            .setHeader("X-CSRF-Token").simple("${property.token}")
            .setHeader("Cookie").simple("${property.cookie}")
            .setHeader("Ignore-Hooks", constant(true))
            .recipientList(simple("http4:{{drupal.baseurl}}/node/$simple{property.uuid}"));
    }
}
