package ca.islandora.services.collection;

import org.apache.camel.Exchange;
import static org.apache.camel.component.http4.HttpMethods.POST;
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
     * Deserializes Drupal node JSON into Map<?, ?>.
     * 
     * @param exchange
     * @throws JsonParseException
     * @throws JsonMappingException
     * @throws IOException
     */
    public void deserializeNode(Exchange exchange) throws JsonParseException, JsonMappingException, IOException {
        final ObjectMapper objectMapper = new ObjectMapper();
        final Map<?, ?> decoded = objectMapper.readValue(exchange.getIn().getBody(String.class), Map.class);
        exchange.getIn().setBody(decoded, Map.class);
    }
    
    /**
     * Constructs a SPARQL update query from a Drupal node.
     * 
     * @param exchange
     * @throws UnsupportedEncodingException
     */
    public void constructSparql(Exchange exchange) throws UnsupportedEncodingException {
        @SuppressWarnings("unchecked")
        final Map<String, ?> node = exchange.getIn().getBody(Map.class);

        @SuppressWarnings("unchecked")
        final Map<String, ?> rdfMapping = (Map<String, ?>)node.get("rdf_mapping");

        @SuppressWarnings("unchecked")
        final Map<String, String> namespaces = (Map<String, String>)node.get("rdf_namespaces");

        final QuadDataAcc triplesToInsert = new QuadDataAcc();
        final QuadAcc triplesToRemove = new QuadAcc();

        int counter = 0;

        for (String key : rdfMapping.keySet()) {
            if ("rdftype".equals(key)) {
                triplesToRemove.addTriple(new Triple(NodeFactory.createURI(""),
                        NodeFactory.createURI(escapePrefix("rdf:type", namespaces)),
                        NodeFactory.createVariable("o" + Integer.toString(counter))));
                @SuppressWarnings("unchecked")
                List<String> types = (List<String>) rdfMapping.get(key);
                
                for (String type : types) {
                    triplesToInsert.addTriple(new Triple(NodeFactory.createURI(""),
                            NodeFactory.createURI(escapePrefix("rdf:type", namespaces)),
                            NodeFactory.createURI(escapePrefix(type, namespaces))));
                }
            } else if ("field_fedora_has_parent".equals(key) ||
                       "field_fedora_path".equals(key)) {
                // Ignore read only properties.
                continue;
            } else {
                @SuppressWarnings("unchecked")
                Map<String, ?> mapping = (Map<String, ?>) rdfMapping.get(key);

                String type = (String) mapping.get("datatype");

                @SuppressWarnings("unchecked")
                List<String> predicates = (List<String>) mapping.get("predicates");

                for (String predicate : predicates) {
                    predicate = escapePrefix(predicate, namespaces);
                    triplesToRemove.addTriple(new Triple(NodeFactory.createURI(""),
                            NodeFactory.createURI(predicate),
                            NodeFactory.createVariable("o" + Integer.toString(counter))));

                    // Figure out what the heck this is, as it can be lots of things due to bad structure.
                    if (node.get(key) instanceof String) {
                        String value = (String) node.get(key);
                        triplesToInsert.addTriple(new Triple(NodeFactory.createURI(""),
                                NodeFactory.createURI(predicate),
                                NodeFactory.createLiteral(value)));
                    } else if (node.get(key) instanceof Map<?, ?>) {
                        String language = (String) node.get("language");
                        @SuppressWarnings("unchecked")
                        Map<String, List<Map<String, String>>> field = (Map<String, List<Map<String, String>>>) node.get(key);
                        List<Map<String, String>> languageInstance = field.get(language);
                        for (Map<String, String> valueMap : languageInstance) {
                            String value = valueMap.get("value");
                            triplesToInsert.addTriple(new Triple(NodeFactory.createURI(""),
                                    NodeFactory.createURI(predicate),
                                    NodeFactory.createLiteral(value)));
                        }
                    }
                }
            }
            counter++;
        }

        final UpdateRequest updateRequest = UpdateFactory.create();

        // Register namespace prefixes.
        for (String key : namespaces.keySet()) {
            updateRequest.setPrefix(key, namespaces.get(key));
        }

        updateRequest.add(new UpdateDeleteWhere(triplesToRemove));
        updateRequest.add(new UpdateDataInsert(triplesToInsert));

        final ByteArrayOutputStream sparqlUpdate = new ByteArrayOutputStream();
        updateRequest.output(new IndentedWriter(sparqlUpdate));;
        final String query = sparqlUpdate.toString("UTF-8");

        exchange.getIn().removeHeaders("*");
        exchange.getIn().setHeader(Exchange.HTTP_METHOD, POST);
        exchange.getIn().setHeader(Exchange.CONTENT_TYPE, "application/sparql-update");
        exchange.getIn().setBody(query, String.class);
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

