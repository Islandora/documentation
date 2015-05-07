package ca.islandora.services.routes;

import org.apache.camel.builder.RouteBuilder;

/**
 * Boilerplate to configure REST service through Camel.
 * 
 * @author danny
 */
public class ServletConfigurator extends RouteBuilder {

    @Override
    public void configure() throws Exception {
        restConfiguration()
            .component("servlet");
    }
}

