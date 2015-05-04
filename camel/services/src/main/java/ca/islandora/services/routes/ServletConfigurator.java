package ca.islandora.services.routes;

import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.model.rest.RestBindingMode;

public class ServletConfigurator extends RouteBuilder {

	@Override
	public void configure() throws Exception {
		restConfiguration().component("servlet")
			.bindingMode(RestBindingMode.json);
	}

}
