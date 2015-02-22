package ca.islandora.sync.routes;

import org.apache.camel.builder.RouteBuilder;
import org.fcrepo.camel.JmsHeaders;
import org.fcrepo.camel.RdfNamespaces;
import ca.islandora.sync.processors.DrupalNodeCreateJsonTransform;

public class DrupalNodeCreate extends RouteBuilder {
    public void configure() throws Exception {

        from("activemq:topic:fedora")
            .routeId("fedoraIn")
            .filter(header(JmsHeaders.EVENT_TYPE).contains(RdfNamespaces.REPOSITORY + "NODE_ADDED"))
                .to("fcrepo:localhost:8080/fcrepo/rest")
                .process(new DrupalNodeCreateJsonTransform())
                .to("http4:localhost/drupal7/rest/node")
                .log("RESPONSE: ${headers} / ${body}")
                .to("mock:result");
    }
}
