package ca.islandora.camel.indexing.triplestore;

import org.apache.camel.builder.RouteBuilder;

public class TriplestoreIndexer extends RouteBuilder {

    @Override
    public void configure() {
        from("timer:foo?period=5000")
            .log("HERPADERPDERP");
    }
}
