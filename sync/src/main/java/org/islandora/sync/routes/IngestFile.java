package org.islandora.sync.routes;

import org.apache.camel.Exchange;
import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.component.http4.HttpMethods;

public class IngestFile extends RouteBuilder {
    public void configure() throws Exception {

        from("file:///mnt/ingest?delay=1000")
                .choice()
                    .when(simple("${file:ext} == 'jpg'"))
                        .removeHeaders("*")
                        .setHeader(Exchange.HTTP_METHOD, constant(HttpMethods.POST))
                        .to("fcrepo:localhost:8080/fcrepo/rest?contentType=image/jpeg")
                        .log("HEADERS: ${headers}")
                        .log("BODY: ${body}");
    }
}
