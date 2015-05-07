package ca.islandora.services.uuid;

import org.hibernate.Query;
import org.hibernate.SessionFactory;

/**
 * Provides utilities for working with UUID / Path mappings.
 * 
 * @author danny
 */
public class UUIDService {

    /**
     * For Hibernate.
     */
    private final SessionFactory sessionFactory;
    
    /**
     * Ctor.  Gets Hibernate sessionFactory injected via Spring.
     * 
     * @param sessionFactory
     */
    public UUIDService(SessionFactory sessionFactory) {
        this.sessionFactory = sessionFactory;
    }

    /**
     * Gets the path in Fedora for the Drupal node represented by the supplied UUID.
     * 
     * @param uuid
     * @return Path in Fedora if it exists, otherwise null.
     */
    public String getPathForUUID(String uuid) {
        if (uuid == null) {
            return null;
        }
        final Query query = sessionFactory.getCurrentSession().createQuery("from UUIDMap where uuid = :uuid");
        query.setString("uuid", uuid);
        final UUIDMap map = (UUIDMap)query.uniqueResult();
        if (map == null) {
            return null;
        }
        return map.getPath();
    }
}
