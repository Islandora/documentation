package ca.islandora.services.uuid;

import org.hibernate.Query;
import org.hibernate.SessionFactory;

public class UUIDService {

    protected SessionFactory sessionFactory;

    public UUIDService(SessionFactory sessionFactory) {
        this.sessionFactory = sessionFactory;
    }

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
