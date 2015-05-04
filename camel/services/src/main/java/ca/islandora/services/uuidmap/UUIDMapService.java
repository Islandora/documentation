package ca.islandora.services.uuidmap;

import org.hibernate.Query;
import org.hibernate.SessionFactory;

public class UUIDMapService {

	protected SessionFactory sessionFactory;
	
	public UUIDMapService(SessionFactory sessionFactory) {
		this.sessionFactory = sessionFactory;
	}
	
	public UUIDMap getMappingForPath(String path) {
		Query query = sessionFactory.getCurrentSession().createQuery("from UUIDMap where path = :path");
		query.setText("path", path);
		return (UUIDMap)query.uniqueResult();
	}
	
    public UUIDMap generateMap() {
        UUIDMap map = new UUIDMap();
        map.setPath("HERP");
        map.setUuid("DERP");
        map.setPathHash("DOOP");
        return map;
    }
}
