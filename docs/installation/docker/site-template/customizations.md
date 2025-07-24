# Customizations

The following sections describe optional customizations you can make to further modify your Islandora site.

## Blocking Connections

### Blocking IP Addresses

#### iptables

If your server uses iptables, you can block a range of IP addresses using the following command

`iptables -I DOCKER-USER -s XXX.XXX.XXX.0/24 -j DROP`

### Blocking by User Agent

You can modify how nginx handles certain user agents by modifying `/etc/nginx/shared/drupal.defaults.conf` inside your Drupal container. For example, by adding:
```
if ($http_user_agent ~ (Bytespider|ClaudeBot|Sogou|SemrushBot|AcademicBotRTU|PetalBot|GPTBot|DataForSeoBot|test-bot) ) {
    return 403;
}
```

## Traefik customizations

### TLS Settings

Traefik allows you to modify security settings by doing things like setting a minimum TLS version or specifying cipher suites. Isle Site Template ships with two versions of the TLS settings file, one [for development](https://github.com/Islandora-Devops/isle-site-template/blob/main/dev-tls.yml), and one [for production](https://github.com/Islandora-Devops/isle-site-template/blob/main/prod-tls.yml).

These files are mounted as volumes in the Traefik containers, so you can modify them as desired.

More information is available in the [Traefik documentation](https://doc.traefik.io/traefik/https/tls/#tls-options)

### Hiding Fedora from the public

By default, your Fedora repository will be exposed to the public at fcrepo.${DOMAIN}. If you don't want the public to be able to access your repository you can add the `traefik-disable` label to the fcrepo-prod service in your docker-compose.yml.

```
         labels:
            <<: [*traefik-disable, *fcrepo-labels]
```

If you do this, you can remove the fcrepo domain from your DNS records.

### Exposing ActiveMQ, Blazegraph, and Solr

By default, these services are hidden by Traefik. If you want to allow access to them, you need to remove the `traefik-disable` label from your docker-compose.yml.

You will also need to add their URL to your DNS record. The URLs are defined in the docker-compose.yml, and by default will be activemq.{DOMAIN}, blazegraph.{DOMAIN}, and solr.{DOMAIN}
