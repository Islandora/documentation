/**
 * 
 */
package ca.islandora.sync.processors;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.camel.Property;

/**
 * @author danny
 *
 */
public class DrupalUpsertProcessorBean {
    
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
                    
                    // Carry on if there is no value for the predicate in Fedora.
                    if (rdfEntry == null) {
                        continue;
                    }
                    
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
    
    public Map<String, Object> createNodeFromRdf(Map<String, Object> mappings,
                                                 @Property("rdf") Map<String, Object> rdf,
                                                 @Property("contentType") String contentType,
                                                 @Property("uuid") String uuid) {
        @SuppressWarnings("unchecked")
        Map<String, Map<String, String>> fileMapping = (Map<String, Map<String, String>>) mappings.get("pcdm_file_mapping");
        
        @SuppressWarnings("unchecked")
        Map<String, String> namespaces = (Map<String, String>) mappings.get("rdf_namespaces");
        
        @SuppressWarnings("unchecked")
        Map<String, Object> rdfMapping = (Map<String, Object>) mappings.get("rdf_mapping");
        
        Map<String, Object> node = new HashMap<String, Object>();
        
        String language = "und";
        
        node.put("type", contentType);
        node.put("language", language);
        node.put("status", "1");
        node.put("promote", "1");
        node.put("uuid", uuid);
        
        // Iterate over the mapping to update node.
        for (String key : rdfMapping.keySet()) {
            // Ignore list.
            if ("rdftype".equals(key) ||
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
                    
                    // Carry on if there is no value for the predicate in Fedora.
                    if (rdfEntry == null) {
                        continue;
                    }
                    
                    // Special case:
                    // Title is an entity property, not a field
                    if (isEntityProperty(key)) {
                        for (Map<String, String> entry : rdfEntry) {
                            node.put(key, entry.get("@value"));
                        }
                    // Everything else is a field
                    } else {
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
    
    /**
     * Utility function to determine if rdf maps to an entity property.
     * @param possibleProperty
     * @return true if entity property, false if field.
     */
    private boolean isEntityProperty(String possibleProperty) {
        List<String> properties = Arrays.asList("vid",
                                                "uid",
                                                "title",
                                                "log",
                                                "status",
                                                "comment",
                                                "promote",
                                                "sticky",
                                                "vuuid",
                                                "nid",
                                                "type",
                                                "language",
                                                "created",
                                                "changed",
                                                "tnid",
                                                "translate",
                                                "uuid",
                                                "revision_timestamp",
                                                "revision_uid",
                                                "cid",
                                                "last_comment_timestamp",
                                                "last_comment_name",
                                                "last_commment_uid",
                                                "comment_count",
                                                "name",
                                                "picture",
                                                "data");
        
        return properties.contains(possibleProperty);
    }
}
