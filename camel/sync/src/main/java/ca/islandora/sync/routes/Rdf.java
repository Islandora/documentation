/**
 * 
 */
package ca.islandora.sync.routes;

import static org.apache.camel.component.http4.HttpMethods.GET;

import org.apache.camel.Exchange;
import org.apache.camel.builder.RouteBuilder;
import org.fcrepo.camel.FcrepoHeaders;

/**
 * @author danny
 *
 */
public class Rdf extends RouteBuilder {

    /* (non-Javadoc)
     * @see org.apache.camel.builder.RouteBuilder#configure()
     */
    @Override
    public void configure() throws Exception {
        from("direct:fedoraGetRdf")
            .routeId("fedoraGetRdf")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, GET)
            .setHeader(FcrepoHeaders.FCREPO_BASE_URL, property(FcrepoHeaders.FCREPO_BASE_URL))
            .setHeader(FcrepoHeaders.FCREPO_IDENTIFIER, property(FcrepoHeaders.FCREPO_IDENTIFIER))
            .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/ld+json"))
            .to("fcrepo:localhost:8080/fcrepo/rest")
            .beanRef("jsonProcessor", "deserializeRdf");
        
        from("direct:drupalGetRdfMapping")
            .routeId("drupalGetRdfMapping")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, GET)
            .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/json"))
            .setHeader("X-CSRF-Token").simple("${property.token}")
            .setHeader("Cookie").simple("${property.cookie}")
            .setBody().simple("${null}")
            .recipientList(simple("http4:{{drupal.baseurl}}/rdf_mapping/node/$simple{property.contentType}"));
    }

}
