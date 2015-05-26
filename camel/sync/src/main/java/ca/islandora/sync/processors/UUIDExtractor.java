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
public class UUIDExtractor implements Processor {

    /* (non-Javadoc)
     * @see org.apache.camel.Processor#process(org.apache.camel.Exchange)
     */
    @Override
    public void process(Exchange exchange) throws Exception {
        @SuppressWarnings("unchecked")
        Map<String, Object> rdf = (Map<String, Object>) exchange.getProperty("rdf");
        
        String uuid = null;
        
        if (rdf.containsKey("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/uuid")) {
            @SuppressWarnings("unchecked")
            List<Map<String, String>> rdfEntry = (List<Map<String, String>>) rdf.get("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.2/uuid");
            
            uuid = rdfEntry.get(0).get("@value");
        }

        exchange.setProperty("uuid", uuid);
    }

}
