package ca.islandora.services.uuidmap;

public class UUIDMap {
    private int id;
    private String uuid;
    private String path;
    private String pathHash;

    /**
     * @return the id
     */
    public int getId() {
        return id;
    }

    /**
     * @param id the id to set
     */
    public void setId(int id) {
        this.id = id;
    }

    /**
     * @return the uuid
     */
    public String getUuid() {
        return uuid;
    }

    /**
     * @param uuid the uuid to set
     */
    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    /**
     * @return the path
     */
    public String getPath() {
        return path;
    }

    /**
     * @param path the path to set
     */
    public void setPath(String path) {
        this.path = path;
    }

    /**
     * @return the pathHash
     */
    public String getPathHash() {
        return pathHash;
    }

    /**
     * @param pathHash the pathHash to set
     */
    public void setPathHash(String pathHash) {
        this.pathHash = pathHash;
    }

}
