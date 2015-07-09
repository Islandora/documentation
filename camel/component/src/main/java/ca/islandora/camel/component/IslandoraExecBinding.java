package ca.islandora.camel.component;

import java.io.File;
import java.io.InputStream;
import java.util.List;

import org.apache.camel.Exchange;
import org.apache.camel.component.exec.ExecCommand;
import org.apache.camel.component.exec.impl.DefaultExecBinding;
import org.apache.camel.util.ObjectHelper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import static org.apache.camel.component.exec.impl.ExecParseUtils.splitToWhiteSpaceSeparatedTokens;

public class IslandoraExecBinding extends DefaultExecBinding {
    private static final Logger LOG = LoggerFactory.getLogger(DefaultExecBinding.class);

    @SuppressWarnings("unchecked")
    public ExecCommand readInput(Exchange exchange, IslandoraEndpoint endpoint) {
        ObjectHelper.notNull(exchange, "exchange");
        ObjectHelper.notNull(endpoint, "endpoint");

        // do not convert args as we do that manually later
        Object args = exchange.getIn().removeHeader(EXEC_COMMAND_ARGS);
        String cmd = getAndRemoveHeader(exchange.getIn(), EXEC_COMMAND_EXECUTABLE, endpoint.getExecutable(), String.class);
        String dir = getAndRemoveHeader(exchange.getIn(), EXEC_COMMAND_WORKING_DIR, endpoint.getWorkingDir(), String.class);
        long timeout = getAndRemoveHeader(exchange.getIn(), EXEC_COMMAND_TIMEOUT, endpoint.getTimeout(), Long.class);
        String outFilePath = getAndRemoveHeader(exchange.getIn(), EXEC_COMMAND_OUT_FILE, endpoint.getOutFile(), String.class);
        boolean useStderrOnEmptyStdout = getAndRemoveHeader(exchange.getIn(), EXEC_USE_STDERR_ON_EMPTY_STDOUT, endpoint.isUseStderrOnEmptyStdout(), Boolean.class);
        InputStream input = exchange.getIn().getBody(InputStream.class);

        // If the args is a list of strings already..
        List<String> argsList = null;
        if (isListOfStrings(args)) {
            argsList = (List<String>) args;
        }

        if (argsList == null) {
            // no we could not do that, then parse it as a string to a list
            String s = endpoint.getArgs();
            if (args != null) {
                // use args from header instead from endpoint
                s = exchange.getContext().getTypeConverter().convertTo(String.class, exchange, args);
            }
            LOG.debug("Parsing argument String to a List: {}", s);
            argsList = splitToWhiteSpaceSeparatedTokens(s);
        }

        File outFile = outFilePath == null ? null : new File(outFilePath);
        return new ExecCommand(cmd, argsList, dir, timeout, input, outFile, useStderrOnEmptyStdout);
    }

    private boolean isListOfStrings(Object o) {
        if (o == null) {
            return false;
        }
        if (!(o instanceof List)) {
            return false;
        }
        @SuppressWarnings("rawtypes")
        List argsList = (List)o;
        for (Object s : argsList) {
            if (s.getClass() != String.class) {
                return false;
            }
        }
        return true;
    }
}
