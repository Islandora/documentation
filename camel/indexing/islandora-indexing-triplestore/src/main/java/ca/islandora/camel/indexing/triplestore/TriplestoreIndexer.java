package ca.islandora.camel.indexing.triplestore;

import org.apache.camel.LoggingLevel;
import org.apache.camel.builder.RouteBuilder;

public class TriplestoreIndexer extends RouteBuilder {

    @Override
    public void configure() {

        onException(Exception.class)
            .maximumRedeliveries("{{error.maxRedeliveries}}")
            .log(LoggingLevel.ERROR, "Error Indexing in Triplestore: ${routeId}");

        from("{{input.stream}}")
            .log("GOT THIS BODY: ${body}")
            .log("GOT THIS OPERATION: ${headers.operation}");
    }
}
