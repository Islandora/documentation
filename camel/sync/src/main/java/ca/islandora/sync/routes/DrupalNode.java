/**
 * 
 */
package ca.islandora.sync.routes;

import static org.apache.camel.component.http4.HttpMethods.GET;
import static org.apache.camel.component.http4.HttpMethods.PUT;

import org.apache.camel.Exchange;
import org.apache.camel.builder.RouteBuilder;

/**
 * @author danny
 *
 */
public class DrupalNode extends RouteBuilder {

    /* (non-Javadoc)
     * @see org.apache.camel.builder.RouteBuilder#configure()
     */
    @Override
    public void configure() throws Exception {
        // TODO Auto-generated method stub
        from("direct:drupalGetNode")
            .routeId("drupalGetNode")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, GET)
            .setHeader(Exchange.ACCEPT_CONTENT_TYPE, constant("application/json"))
            .setHeader("X-CSRF-Token").simple("${property.token}")
            .setHeader("Cookie").simple("${property.cookie}")
            .setBody().simple("${null}")
            .recipientList(simple("http4:{{drupal.baseurl}}/node/$simple{property.uuid}"));
    
        from("direct:drupalUpdateNode")
            .routeId("drupalUpdateNode")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, PUT)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
            .setHeader("X-CSRF-Token").simple("${property.token}")
            .setHeader("Cookie").simple("${property.cookie}")
            .setHeader("Ignore-Hooks", constant(true))
            .recipientList(simple("http4:{{drupal.baseurl}}/node/$simple{property.uuid}"));
        
    }

}
