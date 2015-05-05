package ca.islandora.services.collection;

import org.hibernate.SessionFactory;


public class CollectionService {
    private final SessionFactory sessionFactory;

    public CollectionService(SessionFactory sessionFactory) {
        this.sessionFactory = sessionFactory;
    }

    public void getCollectionByUUID(String uuid) {

    }
}
