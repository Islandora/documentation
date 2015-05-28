/**
 * 
 */
package ca.islandora.sync.routes;

import java.util.Map;

import org.apache.camel.builder.RouteBuilder;

import ca.islandora.sync.processors.ContentTypeExtractor;

/**
 * @author danny
 *
 */
public class DrupalUpsert extends RouteBuilder {

    /* (non-Javadoc)
     * @see org.apache.camel.builder.RouteBuilder#configure()
     */
    @Override
    public void configure() throws Exception {
        from("direct:drupalUpsertNodeFromRdf")
            .routeId("drupalUpsertNodeFromRdf")
            .to("direct:drupalAuthenticate")
            .to("direct:drupalGetNode")
            .beanRef("jsonProcessor", "deserializeNode")
            .choice()
                .when(body(Map.class).isNull())
                    .to("direct:drupalInsertNodeFromRdf")
                .otherwise()
                    .to("direct:drupalUpdateNodeFromRdf")
            .endChoice()
            .to("direct:drupalLogout");
        
        from("direct:drupalUpdateNodeFromRdf")
            .routeId("drupalUpdateNodeFromRdf")
            .beanRef("drupalUpsertProcessor", "updateNodeFromRdf")
            .beanRef("jsonProcessor", "serializeMap")
            .to("direct:drupalUpdateNode");
            
        from("direct:drupalInsertNodeFromRdf")
            .routeId("drupalInsertNodeFromRdf")
            .process(new ContentTypeExtractor())
            .log("${property.contentType}")
            .choice()
                .when(property("contentType").isNotNull())
                    .to("direct:drupalGetRdfMapping")
                    .beanRef("jsonProcessor", "deserializeMap")
                    .beanRef("drupalUpsertProcessor", "createNodeFromRdf")
                    .setProperty("uuid").simple("${body['uuid']}")
                    .beanRef("jsonProcessor", "serializeMap")
                    .log("${body}")
                    .to("direct:drupalUpdateNode")
                .otherwise()
                    .log("COULD NOT PARSE CORRECT CONTENT TYPE FROM RDF");
                  
    }

}
