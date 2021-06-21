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

The secrets themselves are stored in the `secrets` folder of `isle-dc`.  If you navigate to that directory, you'll see several small
files, where each file represents a different password.  By default, they are all set to `password` and you can change them to set your
passwords for the system.

## TLS

All public facing sites need to use HTTPS, and it's definitely a stumbling block for the uninitiated.  Fortunately, `isle-dc` is
set up to  use HTTPS by default.  Even when running `make demo`, your site runs over HTTPS at `https://islandora.traefik.me`. The
default certificates are stored in the `certs` folder of `isle-dc`, and you can simply overwrite them with certificates from your
certificate authority.  As long as the certificates match the `DOMAIN` variable in your `.env` file, that is.

Both the private key and certificate need to be PEM encoded, and the certificate needs to contain the issuer's certificate as well.
Most certificate authorities offer "Full Chain" or "With Issuer" certificates that contain everything you need.  Occassionally,
you may find yourself needing to manually concatenate your certificate with the issuer certificate by hand. In that case, the
certificate for your site goes first, and the issuer's certificate is appended afterwards.   

TODO: Explain Using Let's Encrypt

## Building and Deploying Your Custom Container

First, set your `ENVIRONMENT` variable to `custom` in  your .env file

```
ENVIRONMENT=custom
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
