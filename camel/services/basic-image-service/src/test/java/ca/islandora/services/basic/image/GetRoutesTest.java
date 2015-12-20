package ca.islandora.services.basic.image;

import static java.net.URLEncoder.encode;
import static java.util.UUID.randomUUID;
import static org.apache.camel.Exchange.HTTP_METHOD;
import static org.apache.camel.Exchange.HTTP_QUERY;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import javax.activation.DataHandler;

import org.apache.camel.EndpointInject;
import org.apache.camel.Exchange;
import org.apache.camel.Message;
import org.apache.camel.Produce;
import org.apache.camel.ProducerTemplate;
import org.apache.camel.builder.AdviceWithRouteBuilder;
import org.apache.camel.component.mock.MockEndpoint;
import org.apache.camel.impl.DefaultExchange;
import org.apache.camel.impl.DefaultMessage;
import org.apache.camel.test.blueprint.CamelBlueprintTestSupport;
import org.apache.camel.util.ObjectHelper;

import org.junit.Test;

/**
 * Test the route workflow.
 *
 * @author Aaron Coburn
 * @since 2015-10-07
 */
public class GetRoutesTest extends CamelBlueprintTestSupport {

    @EndpointInject(uri = "mock:result")
    protected MockEndpoint resultEndpoint;

    @Produce(uri = "direct:start")
    protected ProducerTemplate template;

    @Override
    public boolean isUseAdviceWith() {
        return true;
    }

    @Override
    public boolean isUseRouteBuilder() {
        return false;
    }

    @Override
    protected String getBlueprintDescriptor() {
        return "/OSGI-INF/blueprint/*.xml";
    }

    @Test
    public void getObjectUri() throws Exception {
        context.getRouteDefinition("getObjectUri").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpointsAndSkip("direct:*");
                mockEndpointsAndSkip("http:*");
                weaveAddLast().to("mock:result");
            }
        });

        context.start();

        final String uuid = randomUUID().toString();

        resultEndpoint.expectedMessageCount(1);
        resultEndpoint.expectedPropertyReceived("uuid", uuid);
        resultEndpoint.message(0).header(HTTP_METHOD).equals("POST");
        resultEndpoint.message(0).header(HTTP_QUERY).equals(
                "format=xml&query=" + encode(
                "PREFIX nfo: <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/>\n" +
                "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n" +
                "PREFIX pcdm: <http://pcdm.org/models#>\n" +
                "SELECT ?s WHERE {\n" +
                "  ?s nfo:uuid \"" + uuid + "\"^^<http://www.w3.org/2001/XMLSchema#string> .\n" +
                "  ?s rdf:type pcdm:Object .\n}\n", "UTF-8"));

        final Exchange exchange = new DefaultExchange(context);
        exchange.setProperty("uuid", uuid);

        template.send(exchange);

        assertMockEndpointsSatisfied();
    }

    @Test
    public void getCollectionUri() throws Exception {
        context.getRouteDefinition("getCollectionUri").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpointsAndSkip("direct:*");
                mockEndpointsAndSkip("http:*");
                weaveAddLast().to("mock:result");
            }
        });

        context.start();

        final String uuid = randomUUID().toString();

        resultEndpoint.expectedMessageCount(1);
        resultEndpoint.expectedPropertyReceived("uuid", uuid);
        resultEndpoint.message(0).header(HTTP_METHOD).equals("POST");
        resultEndpoint.message(0).header(HTTP_QUERY).equals(
                "format=xml&query=" + encode(
                "PREFIX nfo: <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/>\n" +
                "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n" +
                "PREFIX pcdm: <http://pcdm.org/models#>\n" +
                "SELECT ?s WHERE {\n" +
                "  ?s nfo:uuid \"" + uuid + "\"^^<http://www.w3.org/2001/XMLSchema#string> .\n" +
                "  ?s rdf:type pcdm:Collection .\n}\n", "UTF-8"));

        final Exchange exchange = new DefaultExchange(context);
        exchange.setProperty("uuid", uuid);

        template.send(exchange);

        assertMockEndpointsSatisfied();
    }
}
