package ca.islandora.sync.routes;

import java.util.Map;

import static org.apache.camel.component.http4.HttpMethods.GET;
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
            .filter(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_ADDED"))
                .log("INCOMING HEADERS: ${headers}")
                .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/ld+json"))
                .to("fcrepo:localhost:8080/fcrepo/rest")
                .setProperty(FcrepoHeaders.FCREPO_BASE_URL, header("org.fcrepo.jms.baseURL"))
                .setProperty(FcrepoHeaders.FCREPO_IDENTIFIER, header("org.fcrepo.jms.identifier"))
                .beanRef("drupalNodeCreateProcessor", "deserializeRdf")
                .setProperty("rdf", body(Map.class))
                .setProperty("uuid").simple("${property.rdf['http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/uuid']?[0]['@value']}")
                .choice()
                    .when().simple("${property.uuid} != null")
                        .to("direct:updateNodeFromFedora")
                    .otherwise()
                        .to("direct:createNodeFromFedora");
        
        from("direct:updateNodeFromFedora")
            .to("direct:getNode")
            .beanRef("drupalNodeCreateProcessor", "deserializeNode")
            .beanRef("drupalNodeCreateProcessor", "updateNodeFromRdf")
            .beanRef("drupalNodeCreateProcessor", "serializeNode")
            .to("direct:updateNode");
            
        from("direct:createNodeFromFedora")
            .log("${body}");
        
        from("direct:getNode")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, GET)
            .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/json"))
            .setBody().simple("${null}")
            .recipientList(simple("http4:{{drupal.baseurl}}/node/$simple{property.uuid}"));
        
        from("direct:updateNode")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, PUT)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
            .setHeader("Ignore-Hooks", constant(true))
            .recipientList(simple("http4:{{drupal.baseurl}}/node/$simple{property.uuid}"));
    }
}
