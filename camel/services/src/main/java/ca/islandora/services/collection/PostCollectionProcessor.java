package ca.islandora.services.collection;

import org.apache.camel.Exchange;
import org.apache.camel.Processor;
import org.fcrepo.camel.FcrepoHeaders;

import ca.islandora.services.uuid.UUIDService;


public class PostCollectionProcessor implements Processor {

    private final UUIDService uuidService;

    public PostCollectionProcessor(UUIDService uuidService) {
        this.uuidService = uuidService;
    }

    @Override
    public void process(Exchange exchange) throws Exception {
        final String path = uuidService.getPathForUUID(exchange.getIn().getHeader("uuid", String.class));
        exchange.getIn().removeHeader("uuid");
        if (path != null) {
            exchange.getIn().setHeader(FcrepoHeaders.FCREPO_IDENTIFIER, path); 
        }
    }

}
