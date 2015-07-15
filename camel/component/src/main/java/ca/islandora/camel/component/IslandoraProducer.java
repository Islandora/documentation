package ca.islandora.camel.component;

import java.nio.charset.StandardCharsets;

import org.apache.camel.Exchange;
import org.apache.camel.component.exec.ExecCommand;
import org.apache.camel.component.exec.ExecCommandExecutor;
import org.apache.camel.component.exec.ExecResult;
import org.apache.camel.component.exec.impl.DefaultExecCommandExecutor;
import org.apache.camel.impl.DefaultProducer;
import org.apache.camel.util.ObjectHelper;
import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Islandora producer. {@link IslandoraProducer}
 * 
 * Mimics ExecProducer
 */
public class IslandoraProducer extends DefaultProducer {

    private final Logger log;

    private final IslandoraEndpoint endpoint;

    public IslandoraProducer(IslandoraEndpoint endpoint) {
        super(endpoint);
        this.endpoint = endpoint;
        this.log = LoggerFactory.getLogger(IslandoraProducer.class);
    }

    public void process(Exchange exchange) throws Exception {
        ExecCommand execCommand = getBinding().readInput(exchange, endpoint);

        ExecCommandExecutor executor = endpoint.getCommandExecutor();
        if (executor == null) {
            // create a new non-shared executor
            executor = new DefaultExecCommandExecutor();
        }

        log.info("Executing {}", execCommand);
        ExecResult result = executor.execute(execCommand);

        ObjectHelper.notNull(result, "The command executor must return a not-null result");
        if (result.getExitValue() != 0) {
            log.error("The command {} returned exit value {}", execCommand, result.getExitValue());
            String errMsg = IOUtils.toString(result.getStderr(), StandardCharsets.UTF_8);
            throw new IslandoraPHPException(errMsg);
        }
        log.info("The command {} had exit value {}", execCommand, result.getExitValue());
        getBinding().writeOutput(exchange, result);
    }

    private IslandoraExecBinding getBinding() {
        return ((IslandoraExecBinding)endpoint.getBinding());
    }
}
