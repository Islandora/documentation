package ca.islandora.sync.processors;

import static org.apache.camel.component.http4.HttpMethods.POST;

import org.apache.camel.Exchange;
import org.apache.camel.Message;
import org.apache.camel.Processor;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class DrupalNodeCreateJsonTransform implements Processor {

    @SuppressWarnings("unchecked")
    @Override
    public void process(Exchange exchange) throws Exception {
        /*
         * Make some json that looks like this:
         * {
         *   "title":"From Fedora!!!",
         *   "body":{
         *     "und":[
         *       {
         *          "value": "RDF FROM FEDORA",
         *          "format":"plain_text"
         *       }
         *     ]
         *    },
         *    "type":"article"
         * }
         */
        JSONObject outBody = new JSONObject();
        outBody.put("title", "From Fedora!!!");
        outBody.put("type", "article");

        JSONObject bodyField = new JSONObject();
        JSONObject bodyValue = new JSONObject();
        JSONArray arr = new JSONArray();
        bodyValue.put("value", exchange.getIn().getBody(String.class));
        bodyValue.put("format", "plain_text");
        arr.add(bodyValue);
        bodyField.put("und", arr);
        outBody.put("body", bodyField);

        /*
         * Set up the out message to be a POST for the
         * subsequent call to Drupal's REST service.
         */
        Message outMessage = exchange.getOut();
        outMessage.setHeader(Exchange.HTTP_METHOD, POST);
        outMessage.setHeader(Exchange.CONTENT_TYPE, "application/json");
        outMessage.setBody(outBody.toJSONString());
    }
}
