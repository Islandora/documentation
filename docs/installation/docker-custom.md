# Installing a Production/Staging Server

If you are deploying Islandora on a server that is publicly accessible, there's extra precautions you should take for
performance and security reasons. If you follow these steps, you'll see how we can use our `codebase` folder to build
a custom Drupal container, and bake code into the container instead of bind-mounting it in. We'll also cover how to
store passwords as secrets and set up TLS.

## Getting Started

If you haven't already [made a local environment](../docker-local), you'll want to do that first.  These instructions build off of having
a codebase folder.

## Using your Domain

At this point, we're assuming that you've purchased a domain to use for your repository.  By default, `islandora.traefik.me` is used, which
is fine for `make demo` and `make local`.  But for your production site, you'll need to set the domain you own in your .env file.

```
DOMAIN=example.org
```

## Secrets

Sensitive information, such as passwords, should never be built into a container.  It also shouldn't ever be bind-mounted in like we
do with our codebase folder.  If you use secrets, it's like bind-mounting in a file, except that file is provided from the host machine
to the container using an encrypted channel.

To use secrets, set the following in your .env file

```
USE_SECRETS=true
```

The secrets themselves are stored in the `secrets/live` folder of `isle-dc`.  If you navigate to that directory, you'll see several small
files, where each file represents a different password. They are each randomly generated when you run `make local`.

## TLS

All public facing sites need to use HTTPS, and it's definitely a stumbling block for the uninitiated.  Fortunately, `isle-dc` is
set up to  use HTTPS by default.  Even when running `make demo`, your site runs over HTTPS at `https://islandora.traefik.me`.

### Using your own certificates

The default certificates are stored in the `certs` folder of `isle-dc`, and you can simply overwrite them with certificates from your
certificate authority.  As long as the certificates match the `DOMAIN` variable in your `.env` file, that is. Changing the files in the `certs` folder requires restarting the Traefik container, which you can do by running `docker-compose restart traefik`.

| File | Purpose |
| :---- | :------- |
| __cert.pem__     | A PEM encoded certificate that also contains the issuer's certificate as well. Most certificate authorities offer "Full Chain" or "With Issuer" certificates that contain everything you need.  Occassionally, you may find yourself needing to manually concatenate your certificate with the issuer certificate by hand. In that case, the certificate for your site goes first, and the issuer's certificate is appended afterwards. |
| __privkey.pem__  | A PEM encoded private key used to sign your certificate |


### Requesting Certificates through Let's Encrypt

To use Let's Encrypt to acquire your SSL Certificate, set the following in your .env file and run `make -B docker-compose.yml && make up`.

```
USE_ACME=true
ACME_EMAIL=your-email@example.org
```

Be sure to replace `your-email@example.org` with the email address you've associated with Let's Encrypt.

The way this is setup, is it performs an HTTP Challenge to verify you are in control of the domain. So your system will need to be accessible at `http://DOMAIN/`.

??? warning  "Let's Encrypt Rate Limit"
    If you aren't careful, you can hit Let's Encrypt's rate limit, and you'll be locked out for up to a week!  If you want to use their staging server instead while testing things out, add the following to your .env file

    ```
    ACME_SERVER=https://acme-staging-v02.api.letsencrypt.org/directory
    ```

You'll still get security exceptions when it's working, but you should be able to check the certificate from the browser and confirm you are getting it from the staging server.

```
```

### Troubleshooting Certificate Issues

If you are still getting security exceptions, check what certificate is being used through your browser.  Setting `TRAEFIK_LOG_LEVEL=DEBUG` in your `.env` file will help out greatly when debugging Traefik.  You can tail the logs with `docker-compose logs -tf traefik`.

#### traefik.me SSL certificate expired or revoked
The _*.traefik.me_ certificate that covers `islandora.traefik.me` will need to be redownloaded ocassionally, due to the certificate expiring or possibly being revoked. You can download the updated certificates by performing the following commands:

```
rm certs/cert.pem
rm certs/privkey.pem
make download-default-certs
docker-compose restart traefik
```

!!! note "traefik.me Certificate Note"

    Please note that sometimes the upstream provider of the traefik.me certificate takes a couple of days to update the certificiate after it expires or is accidently revoked.

## Building and Deploying Your Custom Container

First, set your `ENVIRONMENT` variable to `custom` in  your .env file in addition to the changes outlined above

```
ENVIRONMENT=custom
USE_SECRETS=true
DOMAIN=your-domain.org
```

Then rebuild your dockerfile to have your changes take effect

```
make -B docker-compose.yml
```

After this, you can build your custom container with

```
make build
```

You then deploy the container with

```
docker-compose up -d
```
