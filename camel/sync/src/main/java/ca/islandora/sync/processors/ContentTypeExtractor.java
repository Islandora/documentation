/**
 * 
 */
package ca.islandora.sync.processors;

import java.util.List;
import java.util.Map;

import org.apache.camel.Exchange;
import org.apache.camel.Processor;

/**
 * @author danny
 *
 */
public class ContentTypeExtractor implements Processor {

    /* (non-Javadoc)
     * @see org.apache.camel.Processor#process(org.apache.camel.Exchange)
     */
    @Override
    public void process(Exchange exchange) throws Exception {
        @SuppressWarnings("unchecked")
        Map<String, Object> rdf = (Map<String, Object>) exchange.getProperty("rdf");
        
        @SuppressWarnings("unchecked")
        List<String> types = (List<String>) rdf.get("@type");
        
        String contentType = null;
        
        for (String type : types) {
            if (type.contains("http://islandora.ca/ontology/v2/")) {
                String escaped = type.replace("http://islandora.ca/ontology/v2/", "islandora_");
                contentType = escaped;
                break;
            }
        }
        
        exchange.setProperty("contentType", contentType);
    }

}
