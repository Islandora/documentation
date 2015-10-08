package ca.islandora.services.basic.image;

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
import org.apache.commons.io.IOUtils;

import org.junit.Test;

/**
 * Test the route workflow.
 *
 * @author Aaron Coburn
 * @since 2015-10-07
 */
public class SparqlRouteTest extends CamelBlueprintTestSupport {

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
    public void getObjectExtractUri() throws Exception {
        context.getRouteDefinition("getObjectExtractUri").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpoints("*");
                weaveAddLast().to("mock:result");
            }
        });
        context.start();

        resultEndpoint.expectedMessageCount(1);
        resultEndpoint.expectedBodiesReceived("example.org/book/book5");

        template.sendBody(IOUtils.toString(ObjectHelper.loadResourceAsStream("sparql.xml"), "UTF-8"));

        assertMockEndpointsSatisfied();

    }

    @Test
    public void getObjectExtractUriError() throws Exception {
        context.getRouteDefinition("getObjectExtractUri").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpoints("*");
            }
        });

        context.getRouteDefinition("getObjectExtractUriError").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                weaveAddLast().to("mock:result");
            }
        });
        context.start();

        getMockEndpoint("mock:direct:getObjectExtractUriError").expectedMessageCount(1);
        resultEndpoint.expectedMessageCount(1);
        resultEndpoint.expectedHeaderReceived("Content-Type", "text/plain");
        resultEndpoint.expectedHeaderReceived(Exchange.HTTP_RESPONSE_CODE, 500);
        resultEndpoint.message(0).body().startsWith("Failure extracting URI for object");

        template.sendBody("this ain't xml");

        assertMockEndpointsSatisfied();

    }

    @Test
    public void getCollectionExtractUri() throws Exception {
        context.getRouteDefinition("getCollectionExtractUri").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpoints("*");
                weaveAddLast().to("mock:result");
            }
        });
        context.start();

        resultEndpoint.expectedMessageCount(1);
        resultEndpoint.expectedBodiesReceived("example.org/book/book5");

        template.sendBody(IOUtils.toString(ObjectHelper.loadResourceAsStream("sparql.xml"), "UTF-8"));

        assertMockEndpointsSatisfied();

    }

    @Test
    public void getCollectionExtractUriError() throws Exception {
        context.getRouteDefinition("getCollectionExtractUri").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpoints("*");
            }
        });

        context.getRouteDefinition("getCollectionExtractUriError").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                weaveAddLast().to("mock:result");
            }
        });
        context.start();

        getMockEndpoint("mock:direct:getCollectionExtractUriError").expectedMessageCount(1);
        resultEndpoint.expectedMessageCount(1);
        resultEndpoint.expectedHeaderReceived("Content-Type", "text/plain");
        resultEndpoint.expectedHeaderReceived(Exchange.HTTP_RESPONSE_CODE, 500);
        resultEndpoint.message(0).body().startsWith("Failure extracting URI for collection");

        template.sendBody("this ain't xml");

        assertMockEndpointsSatisfied();
    }
}
