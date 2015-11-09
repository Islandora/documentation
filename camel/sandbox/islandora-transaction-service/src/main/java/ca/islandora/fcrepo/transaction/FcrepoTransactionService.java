package ca.islandora.fcrepo.transaction;

import ca.islandora.fcrepo.client.request.CreateResourceRequest;
import ca.islandora.fcrepo.client.response.IFcrepoResponse;
import ca.islandora.fcrepo.client.IFcrepoClient;

import java.net.URISyntaxException;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.ArrayUtils;
import org.fcrepo.camel.FcrepoOperationFailedException;

import org.slf4j.Logger;
import static org.slf4j.LoggerFactory.getLogger;

public class FcrepoTransactionService implements IFcrepoTransactionService {

    private IFcrepoClient fcrepo;

    private static final Logger LOGGER = getLogger(FcrepoTransactionService.class);

    public FcrepoTransactionService(final IFcrepoClient fcrepo) {
        this.fcrepo = fcrepo;
    }

    public String create() throws FcrepoOperationFailedException {
        CreateResourceRequest request = fcrepo.createTransaction();

        if (request == null) {
            return null;
        }

        IFcrepoResponse response = request.execute();

        String location = response.getLocation();
        String[] split = StringUtils.split(location, "/");
        if (ArrayUtils.isEmpty(split)) {
            return null;
        }
        String transactionId = split[split.length - 1];
        if (StringUtils.startsWith(transactionId, "tx:")) {
            return transactionId.substring(3);
        }
        return transactionId;
    }

    public void extend(final String transactionId) throws FcrepoOperationFailedException {
        try {
            CreateResourceRequest request = fcrepo.extendTransaction(transactionId);
            if (request != null) {
                IFcrepoResponse response = request.execute();
            }
        } catch (FcrepoOperationFailedException ex) {
            if (ex.getStatusCode() == 410) {
                LOGGER.warn("Transaction " + transactionId + " does not exist");
            } else {
                throw ex;
            }
        }
    }

    public void commit(final String transactionId) throws FcrepoOperationFailedException {
        try {
            CreateResourceRequest request = fcrepo.commitTransaction(transactionId);
            if (request != null) {
                IFcrepoResponse response = request.execute();
            }
        } catch (FcrepoOperationFailedException ex) {
            if (ex.getStatusCode() == 410) {
                LOGGER.warn("Transaction " + transactionId + " does not exist");
            } else {
                throw ex;
            }
        }
    }

    public void rollback(final String transactionId) throws FcrepoOperationFailedException {
        try {
            CreateResourceRequest request = fcrepo.rollbackTransaction(transactionId);
            if (request != null) {
                IFcrepoResponse response = request.execute();
            }
        } catch (FcrepoOperationFailedException ex) {
            if (ex.getStatusCode() == 410) {
                LOGGER.warn("Transaction " + transactionId + " does not exist");
            } else {
                throw ex;
            }
        }
    }
}

