/**
 * 
 */
package ca.islandora.sync.processors;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.camel.Property;
import org.fcrepo.camel.FcrepoHeaders;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * @author danny
 *
 */
public class DrupalNodeCreateProcessorBean {
    /**
     * Deserializes JSON RDF into Map<String, ?>, ignoring the format=jcr:xml bits.
     * 
     * @param rdfJson
     * @param baseUrl
     * @param path
     * @return
     * @throws JsonParseException
     * @throws JsonMappingException
     * @throws IOException
     */
    public Map<String, Object> deserializeRdf(String rdfJson,
                                              @Property(FcrepoHeaders.FCREPO_BASE_URL) String baseUrl,
                                              @Property(FcrepoHeaders.FCREPO_IDENTIFIER) String path) throws JsonParseException, JsonMappingException, IOException {
        final ObjectMapper objectMapper = new ObjectMapper();
        @SuppressWarnings("unchecked")
        final List<Map<String, Object>> deserialized = objectMapper.readValue(rdfJson, List.class);

        for (Map<String, Object> item : deserialized) {
            String id = (String) item.get("@id");
            if (id.equals(baseUrl + path)) {
                return item;
            }
        }
        
        return null;
    }
    
    /**
     * @param nodeJson
     * @return
     * @throws JsonParseException
     * @throws JsonMappingException
     * @throws IOException
     */
    public Map<String, Object> deserializeNode(String nodeJson) throws JsonParseException, JsonMappingException, IOException {
        final ObjectMapper objectMapper = new ObjectMapper();
        @SuppressWarnings("unchecked")
        final Map<String, Object> decoded = objectMapper.readValue(nodeJson, Map.class);
        return decoded;
    }
    
    /**
     * @param node
     * @return
     * @throws JsonProcessingException
     */
    public String serializeNode(Map<String, Object> node) throws JsonProcessingException {
        final ObjectMapper objectMapper = new ObjectMapper();
        final String encoded = objectMapper.writeValueAsString(node);
        return encoded;
    }
    
    /**
     * @param node
     * @param rdf
     * @return
     */
    public Map<String, ?> updateNodeFromRdf(Map<String, Object> node,
                                            @Property("rdf") Map<String, Object> rdf) {
        // Get the rdf mapping from the node.
        @SuppressWarnings("unchecked")
        final Map<String, Object> rdfMapping = (Map<String, Object>)node.get("rdf_mapping");

        // Get the rdf namespaces and prefixes from the node.
        @SuppressWarnings("unchecked")
        final Map<String, String> namespaces = (Map<String, String>)node.get("rdf_namespaces");

        // Get the language of the node.
        String language = (String) node.get("language");
        
        // Iterate over the mapping to update node.
        for (String key : rdfMapping.keySet()) {
            // Ignore list.
            if ("rdftype".equals(key) ||
                "uuid".equals(key) ||
                "body".equals(key) ||
                "lastActivity".equals(key)) {
                continue;
             
            // Handle field values
            } else {
                // Get the particular rdf mapping, which can have a few different formats.
                @SuppressWarnings("unchecked")
                Map<String, Object> mapping = (Map<String, Object>) rdfMapping.get(key);

                //String type = (String) mapping.get("datatype");

                // Grab the predicates from the mapping
                @SuppressWarnings("unchecked")
                List<String> predicates = (List<String>) mapping.get("predicates");

                // Iterate over predicates, parsing out field/property values for each.
                for (String predicate : predicates) {
                    // Don't forget to escape the prefixes.  The ld+json from Fedora has namespaces
                    // declared in full.
                    predicate = escapePrefix(predicate, namespaces);
                    
                    @SuppressWarnings("unchecked")
                    List<Map<String, String>> rdfEntry = (List<Map<String, String>>) rdf.get(predicate);
                    
                    // Figure out what the heck this is, as it can be lots of things due to bad structure.
                    // If it's a simple entity property (like title), it's just a string.
                    if (node.get(key) instanceof String) {
                        if (rdfEntry.isEmpty()) {
                            node.put(key, "");
                        }
                        else {
                            node.put(key, rdfEntry.get(0).get("@value"));
                        }
                    // A field that hasn't been set yet is an empty array.
                    // A field that has been set is a Map<String, List<Map<String, String>>>
                    // Either way we're just going to generate new data based on the RDF from Fedora
                    // and overwrite it.
                    } else if (node.get(key) instanceof List<?> ||
                               node.get(key) instanceof Map<?, ?>) {
                        // Make a new field and populate it from Fedora's RDF.
                        Map<String, List<Map<String,String>>> field = new HashMap<String, List<Map<String, String>>>();
                        List<Map<String, String>> fieldValues = new ArrayList<Map<String, String>>();
                        
                        
                        for (Map<String, String> entry : rdfEntry) {
                            Map<String, String> newValue = new HashMap<String, String>();
                            
                            // Special case:
                            
                            // Fedora parent uses @id instead of @value
                            if ("field_fedora_has_parent".equals(key)) {
                                newValue.put("value", entry.get("@id"));
                            }
                            // Otherwise you use @value.
                            else {
                                newValue.put("value", entry.get("@value"));
                            }
                            fieldValues.add(newValue);
                        }
                        
                        field.put(language, fieldValues);
                        node.put(key, field);
                    }
                }
                
                // Another special case:
                // Fedora path is the @id entry of the rdf, and is NOT in the rdf_mapping from Fedora.
                Map<String, List<Map<String,String>>> field = new HashMap<String, List<Map<String, String>>>();
                List<Map<String, String>> fieldValues = new ArrayList<Map<String, String>>();
                Map<String, String> newValue = new HashMap<String, String>();
                newValue.put("value", (String) rdf.get("@id"));
                fieldValues.add(newValue);
                field.put(language, fieldValues);
                node.put("field_fedora_path", field);
            }
        }
        return node;
    }
    
    /**
     * Utility function to escape prefixes to return full URIs.  Returns the
     * unescaped predicate if prefix is not found in namespace map.
     * 
     * @param predicate
     * @param namespaces
     * @return
     */
    private String escapePrefix(String predicate, Map<String, String> namespaces) {
        String[] exploded = predicate.split(":");
        String namespace = exploded[0];
        if (!namespaces.containsKey(namespace)) {
            return predicate;
        }
        exploded[0] = namespaces.get(namespace);
        return exploded[0] + exploded[1];
    }
}
