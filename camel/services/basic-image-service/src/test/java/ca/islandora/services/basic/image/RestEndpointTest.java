package ca.islandora.services.basic.image;

import static org.junit.Assert.assertEquals;

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
public class RestEndpointTest extends CamelBlueprintTestSupport {

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
    public void getImage() throws Exception {
        context.getRouteDefinition("basicImageCxfrs").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpointsAndSkip("*");
            }
        });
        context.start();

        final String uuid = "foo";

        getMockEndpoint("mock:direct:getCollectionUri").expectedMessageCount(0);
        getMockEndpoint("mock:direct:getBasicImage").expectedMessageCount(1);
        getMockEndpoint("mock:direct:getBasicImage").expectedHeaderReceived("uuid", uuid);
        getMockEndpoint("mock:direct:createBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:updateBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:deleteBasicImage").expectedMessageCount(0);

        final Map<String, Object> headers = new HashMap<>();
        headers.put(Exchange.HTTP_METHOD, "GET");
        headers.put("uuid", uuid);
        headers.put("operationName", "getBasicImage");

        template.sendBodyAndHeaders(null, headers);

        assertMockEndpointsSatisfied();
    }

    @Test
    public void createImage() throws Exception {
        context.getRouteDefinition("basicImageCxfrs").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpointsAndSkip("*");
            }
        });
        context.start();

        final String uuid = "foo";
        final String node = "bar";
        final String mimetype = "text/plain";
        final DataHandler attachment = new DataHandler("some text", mimetype);

        getMockEndpoint("mock:direct:getBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:getCollectionUri").expectedMessageCount(0);
        getMockEndpoint("mock:direct:createBasicImage").expectedMessageCount(1);
        getMockEndpoint("mock:direct:createBasicImage").expectedHeaderReceived("uuid", uuid);
        getMockEndpoint("mock:direct:createBasicImage").expectedHeaderReceived("node", node);
        getMockEndpoint("mock:direct:getCollectionUri").expectedHeaderReceived("attachment", attachment);
        getMockEndpoint("mock:direct:getCollectionUri").expectedHeaderReceived("mimetype", mimetype);
        getMockEndpoint("mock:direct:updateBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:deleteBasicImage").expectedMessageCount(0);

        final Map<String, Object> headers = new HashMap<>();
        headers.put(Exchange.HTTP_METHOD, "POST");
        headers.put("uuid", uuid);
        headers.put("node", node);
        headers.put("mimetype", mimetype);
        headers.put("operationName", "createBasicImage");

        final Exchange exchange = new DefaultExchange(context);
        final Message message = new DefaultMessage();
        message.addAttachment("key", attachment);
        message.setHeaders(headers);
        exchange.setIn(message);

        template.send(exchange);

        assertMockEndpointsSatisfied();
    }

    @Test
    public void updateImage() throws Exception {
        context.getRouteDefinition("basicImageCxfrs").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpointsAndSkip("*");
            }
        });
        context.start();

        final String uuid = "foo";
        final String node = "bar";
        final String mimetype = "text/plain";
        final DataHandler attachment = new DataHandler("some text", mimetype);

        getMockEndpoint("mock:direct:getBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:getCollectionUri").expectedMessageCount(0);
        getMockEndpoint("mock:direct:createBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:updateBasicImage").expectedMessageCount(1);
        getMockEndpoint("mock:direct:updateBasicImage").expectedHeaderReceived("uuid", uuid);
        getMockEndpoint("mock:direct:updateBasicImage").expectedHeaderReceived("node", node);
        getMockEndpoint("mock:direct:updateBasicImage").expectedHeaderReceived("mimetype", mimetype);
        getMockEndpoint("mock:direct:deleteBasicImage").expectedMessageCount(0);

        final Map<String, Object> headers = new HashMap<>();
        headers.put(Exchange.HTTP_METHOD, "PUT");
        headers.put("uuid", uuid);
        headers.put("node", node);
        headers.put("mimetype", mimetype);
        headers.put("operationName", "updateBasicImage");

        final Exchange exchange = new DefaultExchange(context);
        final Message message = new DefaultMessage();
        message.addAttachment("key", attachment);
        message.setHeaders(headers);
        exchange.setIn(message);

        template.send(exchange);

        assertEquals(getMockEndpoint("mock:direct:updateBasicImage").getExchanges().get(0).getIn().getAttachment("key"), attachment);
        assertMockEndpointsSatisfied();
    }

    @Test
    public void deleteImage() throws Exception {
        context.getRouteDefinition("basicImageCxfrs").adviceWith(context, new AdviceWithRouteBuilder() {
            @Override
            public void configure() throws Exception {
                replaceFromWith("direct:start");
                mockEndpointsAndSkip("*");
            }
        });
        context.start();

        final String uuid = "foo";

        getMockEndpoint("mock:direct:getCollectionUri").expectedMessageCount(0);
        getMockEndpoint("mock:direct:getBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:createBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:updateBasicImage").expectedMessageCount(0);
        getMockEndpoint("mock:direct:deleteBasicImage").expectedMessageCount(1);
        getMockEndpoint("mock:direct:deleteBasicImage").expectedHeaderReceived("uuid", uuid);


        final Map<String, Object> headers = new HashMap<>();
        headers.put(Exchange.HTTP_METHOD, "DELETE");
        headers.put("uuid", uuid);
        headers.put("operationName", "deleteBasicImage");

        template.sendBodyAndHeaders(null, headers);

        assertMockEndpointsSatisfied();
    }

}
