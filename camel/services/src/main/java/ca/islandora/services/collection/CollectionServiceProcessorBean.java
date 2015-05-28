package ca.islandora.services.collection;

import org.apache.jena.atlas.io.IndentedWriter;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.hp.hpl.jena.graph.NodeFactory;
import com.hp.hpl.jena.graph.Triple;
import com.hp.hpl.jena.sparql.modify.request.QuadAcc;
import com.hp.hpl.jena.sparql.modify.request.QuadDataAcc;
import com.hp.hpl.jena.sparql.modify.request.UpdateDataInsert;
import com.hp.hpl.jena.sparql.modify.request.UpdateDeleteWhere;
import com.hp.hpl.jena.update.UpdateFactory;
import com.hp.hpl.jena.update.UpdateRequest;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Map;

/**
 * Provides processing functions the Collection Service.
 * 
 * @author danny
 */
public class CollectionServiceProcessorBean {
    
    /**
     * Deserializes Drupal node JSON into Map<String, Object>.
     * 
     * @param exchange
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
     * Constructs a SPARQL update query from a Drupal node.
     * 
     * @param exchange
     * @throws UnsupportedEncodingException
     */
    public String nodeToSparqlUpdate(Map<String, Object> node) throws UnsupportedEncodingException {
        // Get the rdf mapping from the node.
        @SuppressWarnings("unchecked")
        final Map<String, Object> rdfMapping = (Map<String, Object>)node.get("rdf_mapping");

        // Get the rdf namespaces and prefixes from the node.
        @SuppressWarnings("unchecked")
        final Map<String, String> namespaces = (Map<String, String>)node.get("rdf_namespaces");

        // Declare the triples to insert and remove for the SPARQL update query.
        final QuadDataAcc triplesToInsert = new QuadDataAcc();
        final QuadAcc triplesToRemove = new QuadAcc();

        // Incrementing counter for triples to remove so variables are unique.
        int counter = 0;

        // Iterate over the mapping to collect triples.
        for (String key : rdfMapping.keySet()) {
            // Handle the rdf types separately.
            if ("rdftype".equals(key)) {
                triplesToRemove.addTriple(new Triple(NodeFactory.createURI(""),
                        NodeFactory.createURI(escapePrefix("rdf:type", namespaces)),
                        NodeFactory.createVariable("o" + Integer.toString(counter++))));
                @SuppressWarnings("unchecked")
                List<String> types = (List<String>) rdfMapping.get(key);
                
                for (String type : types) {
                    triplesToInsert.addTriple(new Triple(NodeFactory.createURI(""),
                            NodeFactory.createURI(escapePrefix("rdf:type", namespaces)),
                            NodeFactory.createURI(escapePrefix(type, namespaces))));
                }
            // Ignore read only properties.
            } else if ("field_fedora_has_parent".equals(key) ||
                       "field_fedora_path".equals(key)) {
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
                    // Don't forget to escape the prefixes.  SPARQL updates in Fedora require namespaces
                    // to be declared in full.
                    predicate = escapePrefix(predicate, namespaces);
                    
                    // Figure out what the heck this is, as it can be lots of things due to bad structure.
                    // If it's a simple entity property (like uuid), it's just a string.
                    if (node.get(key) instanceof String) {
                        String value = (String) node.get(key);
                        triplesToRemove.addTriple(new Triple(NodeFactory.createURI(""),
                                NodeFactory.createURI(predicate),
                                NodeFactory.createVariable("o" + Integer.toString(counter++))));
                        triplesToInsert.addTriple(new Triple(NodeFactory.createURI(""),
                                NodeFactory.createURI(predicate),
                                NodeFactory.createLiteral(value)));
                    // If it's a field, it's got a weird nested structure based on language.
                    } else if (node.get(key) instanceof Map<?, ?>) {
                        String language = (String) node.get("language");
                        @SuppressWarnings("unchecked")
                        Map<String, List<Map<String, String>>> field = (Map<String, List<Map<String, String>>>) node.get(key);
                        List<Map<String, String>> languageInstance = field.get(language);
                        for (Map<String, String> valueMap : languageInstance) {
                            String value = valueMap.get("value");
                            triplesToRemove.addTriple(new Triple(NodeFactory.createURI(""),
                                    NodeFactory.createURI(predicate),
                                    NodeFactory.createVariable("o" + Integer.toString(counter++))));
                            triplesToInsert.addTriple(new Triple(NodeFactory.createURI(""),
                                    NodeFactory.createURI(predicate),
                                    NodeFactory.createLiteral(value)));
                        }
                    }
                }
            }
        }

        final UpdateRequest updateRequest = UpdateFactory.create();

        // Register namespace prefixes.
        for (String key : namespaces.keySet()) {
            updateRequest.setPrefix(key, namespaces.get(key));
        }
        updateRequest.add(new UpdateDeleteWhere(triplesToRemove));
        updateRequest.add(new UpdateDataInsert(triplesToInsert));

        final ByteArrayOutputStream sparqlUpdate = new ByteArrayOutputStream();
        updateRequest.output(new IndentedWriter(sparqlUpdate));
        return sparqlUpdate.toString("UTF-8");
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

