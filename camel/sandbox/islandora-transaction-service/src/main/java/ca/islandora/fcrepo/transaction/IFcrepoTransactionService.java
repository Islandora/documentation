package ca.islandora.fcrepo.transaction;

import org.fcrepo.camel.FcrepoOperationFailedException;

public interface IFcrepoTransactionService {

    public String create() throws FcrepoOperationFailedException;
    public void extend(final String transactionId) throws FcrepoOperationFailedException;
    public void commit(final String transactionId) throws FcrepoOperationFailedException;
    public void rollback(final String transactionId) throws FcrepoOperationFailedException;

}


