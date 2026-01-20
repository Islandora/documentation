# Customizations

The following sections describe optional customizations you can make to further modify your Islandora site.

## Blocking Connections

### Blocking IP Addresses

#### iptables

If your server uses iptables, you can block a range of IP addresses using the following command

`iptables -I DOCKER-USER -s XXX.XXX.XXX.0/24 -j DROP`

## Traefik customizations

### TLS Settings

Traefik allows you to modify security settings by doing things like setting a minimum TLS version or specifying cipher suites. Isle Site Template includes Traefik configuration files in the `traefik/dynamic` directory.

These configuration files control routing rules and TLS settings. You can modify them to customize your Traefik setup.

More information is available in the [Traefik documentation](https://doc.traefik.io/traefik/https/tls/#tls-options)

### Switching between HTTP and HTTPS

The template provides make commands to easily switch between different modes:

- `make traefik-http` - Switch to HTTP mode (default for local development)
- `make traefik-https-mkcert` - Switch to HTTPS using mkcert self-signed certificates
- `make traefik-https-letsencrypt` - Switch to HTTPS using Let's Encrypt ACME for production

After switching modes, restart Traefik with `make down-traefik up`

### Hiding Fedora from the public

By default, your Fedora repository will be exposed to the public at `fcrepo.${DOMAIN}`. If you don't want the public to be able to access your repository, you can modify the Traefik routing configuration in the `traefik/dynamic` directory to disable external access while keeping internal access for other services.

If you do this, you can remove the fcrepo subdomain from your DNS records.

Finally, ensure that Drupal is configured to access Fedora using the internal Docker network hostname:

```yaml
DRUPAL_DEFAULT_FCREPO_URL: "http://fcrepo:8080/fcrepo/rest/"
```

### Exposing ActiveMQ, Blazegraph, and Solr

By default in production environments, these services are hidden from public access. The visibility is controlled through Traefik configuration in the `traefik/dynamic` directory.

To expose these services:
1. Modify the Traefik dynamic configuration to enable routing for these services
2. Add their URLs to your DNS records (e.g., `activemq.${DOMAIN}`, `blazegraph.${DOMAIN}`, `solr.${DOMAIN}`)
3. Consider adding authentication middleware to protect these administrative interfaces

For local development with `DEVELOPMENT_ENVIRONMENT=true`, these services are typically accessible at subdomain URLs for convenience.
