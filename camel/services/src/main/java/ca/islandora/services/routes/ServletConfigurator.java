package ca.islandora.services.routes;

import org.apache.camel.builder.RouteBuilder;

public class ServletConfigurator extends RouteBuilder {

    @Override
    public void configure() throws Exception {
        restConfiguration()
            .component("servlet");
    }
}

