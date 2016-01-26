package ca.islandora.camel.indexing.triplestore;

import org.apache.camel.LoggingLevel;
import org.apache.camel.Predicate;
import org.apache.camel.builder.PredicateBuilder;
import org.apache.camel.builder.RouteBuilder;
import org.fcrepo.camel.FcrepoHeaders;
import org.fcrepo.camel.processor.SparqlUpdateProcessor;
import org.fcrepo.camel.processor.SparqlDeleteProcessor;

public class TriplestoreIndexer extends RouteBuilder {

    @Override
    public void configure() {

        Predicate isTriples = header("Content-Type").isEqualTo("application/n-triples");
        Predicate hasBaseUrl = header(FcrepoHeaders.FCREPO_BASE_URL).isNotNull();
        Predicate hasIdentifier = header(FcrepoHeaders.FCREPO_IDENTIFIER).isNotNull();
        Predicate hasFcrepoCamelHeaders = PredicateBuilder.and(hasBaseUrl, hasIdentifier);
        Predicate hasAction = PredicateBuilder.or(header("action").isEqualTo("delete"), header("action").isEqualTo("upsert"));
        Predicate isValid = PredicateBuilder.and(isTriples, hasFcrepoCamelHeaders, hasAction);

        onException(Exception.class)
            .maximumRedeliveries("{{error.maxRedeliveries}}")
            .log(LoggingLevel.ERROR, "Error Indexing in Triplestore: ${routeId}");

        from("{{input.stream}}")
            .routeId("IslandoraTriplestoreIndexerRouter")
            .filter(isValid)
                .choice()
                    .when(header("action").isEqualTo("delete"))
                        .to("direct:triplestoreDelete")
                    .otherwise()
                        .to("direct:triplestoreUpsert");

        from("direct:triplestoreUpsert")
            .routeId("islandoraTripelstoreIndexerUpsert")
            .process(new SparqlUpdateProcessor())
            .to("http4://{{triplestore.baseUrl}}");

        from("direct:triplestoreDelete")
            .routeId("islandoraTripelstoreIndexerDelete")
            .process(new SparqlDeleteProcessor())
            .to("http4://{{triplestore.baseUrl}}");

    }
}
