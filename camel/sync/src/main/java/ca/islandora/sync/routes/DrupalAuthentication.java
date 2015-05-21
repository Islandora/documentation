/**
 * 
 */
package ca.islandora.sync.routes;

import static org.apache.camel.component.http4.HttpMethods.POST;

import org.apache.camel.Exchange;
import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.component.http4.HttpOperationFailedException;

import predicates.DrupalInvalidCSRFPredicate;

/**
 * @author danny
 *
 */
public class DrupalAuthentication extends RouteBuilder {

    /* (non-Javadoc)
     * @see org.apache.camel.builder.RouteBuilder#configure()
     */
    @Override
    public void configure() throws Exception {
        
        DrupalInvalidCSRFPredicate invalidCSRF = new DrupalInvalidCSRFPredicate();

        from("direct:drupalAuthenticate")
            .doTry()
                .log("ATTEMPTING LOGIN")
                .to("direct:drupalLogin")
            .doCatch(HttpOperationFailedException.class).onWhen(invalidCSRF)
                .log("ATTEMPTING RE-LOGIN DUE TO BAD CSRF")
                .to("direct:drupalLogin")
            .end();
        
        from("direct:drupalReauthenticate")
            .to("direct:drupalGetToken")
            .to("direct:drupalLogout")
            .to("direct:drupalLogin");
        
        from("direct:drupalLogin")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, POST)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
            .setBody().simple("{\"username\": \"{{drupal.username}}\", \"password\" : \"{{drupal.password}}\"}")
            .to("http4:{{drupal.baseurl}}/user/login")
            .beanRef("drupalNodeCreateProcessor", "deserializeMap")
            .setProperty("token").simple("${body['token']}")
            .setProperty("cookie").simple("${body['session_name']}=${body['sessid']}");
                
        from("direct:drupalGetToken")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, POST)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
            .setBody().simple("{\"username\": \"{{drupal.username}}\", \"password\" : \"{{drupal.password}}\"}")
            .to("http4:{{drupal.baseurl}}/user/token")
            .beanRef("drupalNodeCreateProcessor", "deserializeMap")
            .setProperty("token").simple("${body['token']}");
        
        from("direct:drupalLogout")
            .removeHeaders("*")
            .setHeader(Exchange.HTTP_METHOD, POST)
            .setHeader(Exchange.CONTENT_TYPE, constant("application/json"))
            .setHeader("X-CSRF-Token").simple("${property.token}")
            .setBody().simple("${null}")
            //.setBody().simple("{\"username\": \"{{drupal.username}}\", \"password\" : \"{{drupal.password}}\"}")
            .to("http4:{{drupal.baseurl}}/user/logout");
                

    }

}
