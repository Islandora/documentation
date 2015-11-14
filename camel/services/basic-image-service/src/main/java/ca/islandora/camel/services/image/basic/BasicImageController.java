package ca.islandora.camel.services.image.basic;

import java.io.InputStream;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.DELETE;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.PathParam;
import org.apache.cxf.jaxrs.ext.multipart.Multipart;

@Path("/")
public interface BasicImageController {

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
    public InputStream getBasicImage(@PathParam("uuid") String uuid);

    @POST
    @Path("{uuid}")
    @Consumes("multipart/form-data")
    @Produces("text/plain")
    public InputStream createBasicImage(
            @Multipart InputStream binary,
            @Multipart(value = "node") String node,
            @Multipart(value = "mimetype") String mimetype,
            @PathParam("uuid") String uuid);

    @PUT
    @Path("{uuid}")
    @Consumes("multipart/form-data")
    @Produces("text/plain")
    public InputStream updateBasicImage(
            @Multipart InputStream binary,
            @Multipart(value = "node") String node,
            @Multipart(value = "mimetype") String mimetype,
            @PathParam("uuid") String uuid);

    @DELETE
    @Path("/{uuid}")
    public InputStream deleteBasicImage(@PathParam("uuid") String uuid);
}
