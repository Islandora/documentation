package ca.islandora.camel.services.collection;

import java.io.InputStream;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.DELETE;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.PathParam;

@Path("/")
public interface CollectionController {

    @GET
    @Path("{uuid}")
    @Produces({"application/ld+json",
            "application/n-triples",
            "application/rdf+xml",
            "application/x-turtle",
            "application/xhtml+xml",
            "application/xml",
            "text/html",
            "text/n3",
            "text/plain",
            "text/rdf+n3",
            "text/turtle"})
    public InputStream getCollection(@PathParam("uuid") String uuid);

    @POST
    @Consumes("application/json")
    @Produces("text/plain")
    public InputStream createCollection(InputStream rdf);

    @POST
    @Path("{uuid}")
    @Consumes("application/json")
    @Produces("text/plain")
    public InputStream createSubCollection(InputStream rdf, @PathParam("uuid") String uuid);

    @PUT
    @Path("{uuid}")
    @Consumes("application/json")
    @Produces("text/plain")
    public InputStream updateCollection(InputStream rdf, @PathParam("uuid") String uuid);

    @DELETE
    @Path("{uuid}")
    public InputStream deleteCollection(@PathParam("uuid") String uuid);

}