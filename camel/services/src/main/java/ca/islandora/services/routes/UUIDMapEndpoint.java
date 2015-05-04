package ca.islandora.services.routes;

import org.apache.camel.builder.RouteBuilder;

import ca.islandora.services.uuidmap.UUIDMap;

public class UUIDMapEndpoint extends RouteBuilder {

	@Override
	public void configure() throws Exception {
		rest("/uuidmap/uuid")
			.consumes("application/json").produces("application/json")
			
			.get("/{path}").description("Gets a mapping by path").outType(UUIDMap.class)
				.to("direct:getMappingForPath");
		
		from("direct:getMappingForPath")
			.transacted()
			.to("bean:mapBean?method=getMappingForPath(${headers.path})");
	
		/*
		
			.post()
				.to("direct:createUUIDForPath")
			.put("/{path}")
				.to("direct:updateUUIDForPath")
			.delete("/{path}")
				.to("direct:deleteMappingForPath");

		rest("/uuidmap/path")
			.get("/{uuid}")
				.to("direct:getPathForUUID")
			.post()
				.to("direct:createPathForUUID")
			.put("/{uuid}")
				.to("direct:updatePathForUUID")
			.delete("/{uuid}")
				.to("direct:deleteMappingForUUID");
		
		from("direct:getUUIDForPath")
			.log("FINALLY")
			.transform().simple("DERPADOODOO");
		
		from("direct:createUUIDForPath")
			.log("direct:createUUIDForPath");
		
		from("direct:updateUUIDForPath")
			.log("direct:updateUUIDForPath");
		
		from("direct:deleteMappingForPath")
			.log("direct:deleteMappingForPath");
		
		from("direct:getPathForUUID")
			.log("direct:getPathForUUID");
		
		from("direct:createPathForUUID")
			.log("direct:createPathForUUID");
		
		from("direct:updatePathForUUID")
			.log("direct:updatePathForUUID");
		
		from("direct:deleteMappingForUUID")
			.log("direct:deleteMappingForUUID");
			*/
	}
	

}
