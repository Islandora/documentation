package ca.islandora.camel.component;

import java.net.URLDecoder;
import java.util.Map;

import org.apache.camel.Endpoint;
import org.apache.camel.impl.UriEndpointComponent;
import org.apache.camel.util.ObjectHelper;

/**
 * Represents the component that manages {@link IslandoraEndpoint}. With the
 * component it is possible to execute system commands.
 */
public class IslandoraComponent extends UriEndpointComponent {

    private String executable;
    private String workingDir;
    private String islandoraScript;
    
    public IslandoraComponent(String workingDir) {
        super(IslandoraEndpoint.class);

        this.executable = "php";

        ObjectHelper.notEmpty(workingDir, "workingDir");
        this.workingDir = workingDir;

        this.islandoraScript = "islandora.php";
    }

    @Override
    protected Endpoint createEndpoint(String uri, String remaining, Map<String, Object> parameters) throws Exception {
        IslandoraEndpoint endpoint = new IslandoraEndpoint(uri, this);
        setProperties(endpoint, parameters);
        endpoint.setArgs(URLDecoder.decode(remaining, "UTF-8"));
        return endpoint;
    }
    
    public String getExecutable() {
        return this.executable;
    }
    
    public String getWorkingDir() {
        return this.workingDir;
    }
    
    public String getIslandoraScript() {
        return this.islandoraScript;
    }
}
