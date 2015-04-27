package ca.islandora.services.uuidmap;

public class UUIDMapBean {
    public UUIDMap generateMap() {
        UUIDMap map = new UUIDMap();
        map.setPath("HERP");
        map.setUuid("DERP");
        map.setPathHash("DOOP");
        return map;
    }
}
