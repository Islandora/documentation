package ca.islandora.services.collection;

import static org.apache.camel.component.http4.HttpMethods.PUT;
import static org.apache.camel.component.http4.HttpMethods.POST;

import org.apache.camel.Exchange;
import org.apache.commons.codec.digest.DigestUtils;
import org.apache.jena.atlas.io.IndentedWriter;
import org.fcrepo.camel.FcrepoHeaders;
import org.json.simple.JSONObject;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.hp.hpl.jena.graph.NodeFactory;
import com.hp.hpl.jena.graph.Triple;
import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.rdf.model.Resource;
import com.hp.hpl.jena.rdf.model.ResourceFactory;
import com.hp.hpl.jena.sparql.modify.request.QuadAcc;
import com.hp.hpl.jena.sparql.modify.request.QuadDataAcc;
import com.hp.hpl.jena.sparql.modify.request.UpdateDataInsert;
import com.hp.hpl.jena.sparql.modify.request.UpdateDeleteWhere;
import com.hp.hpl.jena.update.UpdateFactory;
import com.hp.hpl.jena.update.UpdateRequest;
import com.hp.hpl.jena.vocabulary.DC;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.servlet.ServletException;

import ca.islandora.services.uuid.UUIDMap;
import ca.islandora.services.uuid.UUIDService;

/**
 * Provides processing functions the Collection Service.
 * 
 * @author danny
 */
public class CollectionServiceProcessorBean {

    /**
     * For working with UUID / Path mappings.
     */
    private final UUIDService uuidService;

    /**
     * Ctor.  Gets the UUID service injected via Spring.
     * 
     * @param uuidService
     */
    public CollectionServiceProcessorBean(UUIDService uuidService) {
        this.uuidService = uuidService;
    }

    /**
     * Takes incoming message and processes it so it can be sent along to Drupal.
     * 
     * @param exchange
     */
    public void processForDrupalPOST(Exchange exchange) {
        // Hack out parent uuid (which is optional, so result may be null)
        final String parentUUID = exchange.getIn().getHeader("uuid", String.class);
        exchange.setProperty("parentUUID", parentUUID);
        exchange.getIn().removeHeader("uuid");
        
        // Hack out the RDF properties to apply
        final String rdf = exchange.getIn().getBody(String.class);
        exchange.setProperty("rdf", rdf);
        
        // Hack out the Content-Type header
        final String contentType = exchange.getIn().getHeader(Exchange.CONTENT_TYPE, String.class);
        exchange.setProperty("rdfContentType", contentType);
        
        // Create a new UUID to PUT to Drupal
        final String collectionUUID = UUID.randomUUID().toString();
        exchange.setProperty("collectionUUID", collectionUUID);
        
        /* Create JSON message to send to Drupal services to create Node.
         * Looks like:
         * {
         *      "type": "collection",
         *      "uuid": "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
         * }
         */
        JSONObject body = new JSONObject();
        body.put("type", "collection");
        body.put("uuid",  collectionUUID);
        
        // Format message in preperation to be sent to Drupal.
        exchange.getIn().setBody(body.toJSONString(), String.class);
        exchange.getIn().removeHeaders("*");
        exchange.getIn().setHeader(Exchange.HTTP_METHOD, PUT);
        exchange.getIn().setHeader(Exchange.CONTENT_TYPE, "application/json");
    }
    
    /**
     * Takes Drupal PUT results and processes it so it can be sent along to Fedora.
     * @param exchange
     * @throws ServletException
     */
    public void processForFedoraPOST(Exchange exchange) throws ServletException {
        // Format message in preperation to be sent to Fedora
        exchange.getIn().removeHeaders("*");
        exchange.getIn().setHeader(Exchange.HTTP_METHOD, POST);
        exchange.getIn().setHeader(Exchange.CONTENT_TYPE, exchange.getProperty("rdfContentType", String.class));
        exchange.getIn().setBody(exchange.getProperty("rdf", String.class));
        
        // Exit now if parent uuid was not supplied.
        final String uuid = exchange.getProperty("parentUUID", String.class);
        if (uuid == null) {
            return;
        }
        
        // Resolve parent path using supplied uuid and add it to the message header.
        final String path = uuidService.getPathForUUID(uuid);
        if (path == null) {
            throw new ServletException("There is no resource associated with UUID " + uuid);
        }
        exchange.getIn().setHeader(FcrepoHeaders.FCREPO_IDENTIFIER, path);
    }
    
    /**
     * Takes Fedora POST results and processes it so it can be sent along to Hibernate.
     * 
     * @param exchange
     * @throws ServletException
     */
    public void processForHibernatePOST(Exchange exchange) throws ServletException {
        // Extract the path from the Fedora results.
        // TODO: Use the Fedora url from configuration instead of this hard coded hack.
        final String body = exchange.getIn().getBody(String.class);
        final String regex = "/rest";
        
        final String[] parts = body.split(regex);
        
        if (parts.length != 2) {
            throw new ServletException("Malformed path returned from Fedora: " + body + ".  Cannot match on regex " + regex + " in order to get Fedora path.");
        }
        
        // Make the mapping POJO.
        final UUIDMap map = new UUIDMap();
        final String path = parts[1];
        final String uuid = exchange.getProperty("collectionUUID", String.class);
        map.setPath(path);
        map.setPathHash(DigestUtils.md5Hex(path));
        map.setUuid(uuid);
        
        // Format message in preparation to be sent to Hibernate.
        exchange.getIn().removeHeaders("*");
        exchange.getIn().setBody(map);
    }
    
    public void deserializeNode(Exchange exchange) throws JsonParseException, JsonMappingException, IOException {
        final ObjectMapper objectMapper = new ObjectMapper();
        final Map<?, ?> decoded = objectMapper.readValue(exchange.getIn().getBody(String.class), Map.class);
        exchange.getIn().setBody(decoded, Map.class);
    }
    
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

