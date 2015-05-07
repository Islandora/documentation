package ca.islandora.services.collection;

import static org.apache.camel.component.http4.HttpMethods.PUT;
import static org.apache.camel.component.http4.HttpMethods.POST;

import org.apache.camel.Exchange;
import org.apache.commons.codec.digest.DigestUtils;
import org.fcrepo.camel.FcrepoHeaders;
import org.json.simple.JSONObject;

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
}
