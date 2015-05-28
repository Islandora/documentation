/**
 * 
 */
package ca.islandora.services.collection;

import java.util.List;
import java.util.Map;

import org.apache.camel.Exchange;
import org.apache.camel.Processor;

/**
 * @author danny
 *
 */
public class FedoraPathExtractor implements Processor {

    /* (non-Javadoc)
     * @see org.apache.camel.Processor#process(org.apache.camel.Exchange)
     */
    @Override
    public void process(Exchange exchange) throws Exception {
        @SuppressWarnings("unchecked")
        Map<String, Object> node = (Map<String, Object>) exchange.getProperty("node");
        
        if (!node.containsKey("field_fedora_path")) {
            exchange.setProperty("fedoraPath", null);
            return;
        }
        
        @SuppressWarnings("unchecked")
        Map<String, List<Map<String, Object>>> field = (Map<String, List<Map<String, Object>>>) node.get("field_fedora_path");
        
        if (!node.containsKey("language")) {
            exchange.setProperty("fedoraPath", null);
            return;
        }
        
        String language = (String) node.get("language");
        
        if (!field.containsKey(language)) {
            exchange.setProperty("fedoraPath", null);
            return;
        }
        
        List<Map<String, Object>> fieldValues = field.get(language);
        
        if (fieldValues.isEmpty()) {
            exchange.setProperty("fedoraPath", null);
            return;
        }
        
        Map<String, Object> fieldValue = fieldValues.get(0);
        
        if (!fieldValue.containsKey("value")) {
            exchange.setProperty("fedoraPath", null);
            return;
        }
        
        exchange.setProperty("fedoraPath", fieldValue.get("value"));
    }
}
