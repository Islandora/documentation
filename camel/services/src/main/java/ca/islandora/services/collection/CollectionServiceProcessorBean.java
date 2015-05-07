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


public class CollectionServiceProcessorBean {

    private final UUIDService uuidService;

    public CollectionServiceProcessorBean(UUIDService uuidService) {
        this.uuidService = uuidService;
    }

    public void processForDrupalPOST(Exchange exchange) {
        final String parentUUID = exchange.getIn().getHeader("uuid", String.class);
        exchange.setProperty("parentUUID", parentUUID);
        exchange.getIn().removeHeader("uuid");
        
        final String collectionUUID = UUID.randomUUID().toString();
        exchange.setProperty("collectionUUID", collectionUUID);
        
        /*
         * {
         *      "type": "collection",
         *      "uuid": "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
         * }
         */
        JSONObject body = new JSONObject();
        body.put("type", "collection");
        body.put("uuid",  collectionUUID);
        
        exchange.getIn().setBody(body.toJSONString(), String.class);
        exchange.getIn().removeHeaders("*");
        exchange.getIn().setHeader(Exchange.HTTP_METHOD, PUT);
        exchange.getIn().setHeader(Exchange.CONTENT_TYPE, "application/json");
    }
    
    public void processForFedoraPOST(Exchange exchange) throws ServletException {
        exchange.getIn().removeHeaders("*");
        exchange.getIn().setHeader(Exchange.HTTP_METHOD, POST);
        exchange.getIn().setBody(null);
        
        final String uuid = exchange.getProperty("parentUUID", String.class);
        if ("root".equals(uuid)) {
            return;
        }
        
        final String path = uuidService.getPathForUUID(exchange.getIn().getHeader("uuid", String.class));
        if (path == null) {
            throw new ServletException("There is no resource associated with UUID " + uuid);
        }
        exchange.getIn().setHeader(FcrepoHeaders.FCREPO_IDENTIFIER, path);
    }
    
    public void processForHibernatePOST(Exchange exchange) throws ServletException {
        final String body = exchange.getIn().getBody(String.class);
        //final String regex = "/tx:.{36}";
        final String regex = "/rest";
        
        final String[] parts = body.split(regex);
        
        if (parts.length != 2) {
            throw new ServletException("Malformed path returned from Fedora: " + body + ".  Cannot match on regex " + regex + " in order to get Fedora path.");
        }
        
        final UUIDMap map = new UUIDMap();
        final String path = parts[1];
        final String uuid = exchange.getProperty("collectionUUID", String.class);
        map.setPath(path);
        map.setPathHash(DigestUtils.md5Hex(path));
        map.setUuid(uuid);
        
        exchange.getIn().removeHeaders("*");
        exchange.getIn().setBody(map);
    }
}
