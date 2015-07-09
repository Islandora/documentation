package ca.islandora.camel.component;

import org.apache.camel.RuntimeCamelException;

/**
 * Exception thrown when there is an execution failure in an Islandora PHP script.
 */
public class IslandoraPHPException extends RuntimeCamelException {

    private static final long serialVersionUID = 12345L;
    
    public IslandoraPHPException() {
    }

    public IslandoraPHPException(String message) {
        super(message);
    }

    public IslandoraPHPException(String message, Throwable cause) {
        super(message, cause);
    }

    public IslandoraPHPException(Throwable cause) {
        super(cause);
    }
}