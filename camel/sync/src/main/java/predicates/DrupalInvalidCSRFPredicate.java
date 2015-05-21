/**
 * 
 */
package predicates;

import org.apache.camel.Exchange;
import org.apache.camel.Predicate;
import org.apache.camel.component.http4.HttpOperationFailedException;

/**
 * @author danny
 *
 */
public class DrupalInvalidCSRFPredicate implements Predicate {

    /* (non-Javadoc)
     * @see org.apache.camel.Predicate#matches(org.apache.camel.Exchange)
     */
    @Override
    public boolean matches(Exchange exchange) {
        HttpOperationFailedException exception = (HttpOperationFailedException) exchange.getProperty(Exchange.EXCEPTION_CAUGHT);
 
        if (exception.getStatusCode() != 401) {
            return false;
        }
        
        String response = exception.getResponseBody();
        
        if(!"[\"CSRF validation failed\"]".equals(response)) {
            return false;
        }
        
        return true;
    }

}
