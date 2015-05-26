/**
 * 
 */
package ca.islandora.sync.processors;

import java.io.IOException;
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
public class JsonProcessorBean {
    /**
     * Deserializes JSON RDF into Map<String, Object>, ignoring the format=jcr:xml bits.
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
     * @param mapJson
     * @return
     * @throws JsonParseException
     * @throws JsonMappingException
     * @throws IOException
     */
    public Map<String, Object> deserializeMap(String mapJson) throws JsonParseException, JsonMappingException, IOException {
        final ObjectMapper objectMapper = new ObjectMapper();
        @SuppressWarnings("unchecked")
        final Map<String, Object> decoded = objectMapper.readValue(mapJson, Map.class);
        return decoded;
    }
    
    public Map<String, Object> deserializeNode(String nodeJson) throws JsonParseException, JsonMappingException, IOException {
        if ("[false]".equals(nodeJson)) {
            return null;
        }
        return deserializeMap(nodeJson);
    }
    
    /**
     * @param node
     * @return
     * @throws JsonProcessingException
     */
    public String serializeMap(Map<String, Object> node) throws JsonProcessingException {
        final ObjectMapper objectMapper = new ObjectMapper();
        final String encoded = objectMapper.writeValueAsString(node);
        return encoded;
    }
    
}
