# Installing a Production/Staging Server

If you are deploying Islandora on a server that is publicly accessible, there are extra precautions you should take for
performance and security reasons.

For local development we bind mount our codebase (Drupal) folder so that we can do development work locally, outside of 
our Docker container. For a production site we don’t want to do this. Instead, we make changes to our development site, 
then build a Drupal image with our changes that we can use on our production site instead of the default Islandora Drupal 
image.

## Creating your Image

In order to generate that custom Drupal image we need to [set up a development environment](../docker-local). You will do this 
on your development computer, rather than your production server.

Once your development site is set up you will need somewhere to store your custom Drupal image. You should create a private 
repository in your container registry of choice (Dockerhub, GitHub, GitLab, etc.)

Once you have a place to store it, you can create your custom Drupal image by editing your `.env` to set `CUSTOM_IMAGE_NAMESPACE` 
to your dockerhub username or the URL and username for your container repository, and `CUSTOM_IMAGE_NAME` to the name of the 
repository you just created.

Once this is done you can run `make build` to create the image from a Dockerfile. If you don't have a custom Dockerfile, it will 
create one from sample.Dockerfile. This will create a new image from the Islandora Drupal image, with your codebase folder copied 
into the image during the build process. 

You should now have a custom Drupal image on your local machine. You will need to `docker login` to your container registry to push 
your image, so make sure to do that if you haven't yet (you should only need to do this once). You can then push this image to your 
container registry by running `make push-image`. 

## Set up a Git Repository

Now that you have a development site set up with your own codebase folder, you should create a new git repository for your site. 
This way you can easily spin up a new site based on your modules and configuration, instead of the Islandora Starter Site. It will 
also allow you to sync changes between your production, staging, and development sites.

You will likely want to include the Isle-dc directory as well as your codebase folder. This will allow you to make modifications to 
your Makefile, Dockerfile, docker-compose.yml, etc. Note that Isle-dc has the codebase folder in its `.gitignore`, so you will want to
remove that and change the git remote repository URL to your private code repository.

## Set up your Production / Staging Site

At this point you should have a custom git repository and a custom Drupal image, and both should be accessible by your production server.
On your production server you will need to clone your custom copy of Isle-dc, copy the `sample.env` file, and name it `.env`. 

In that `.env` you should set the following variables:
```
ENVIRONMENT=custom
COMPOSE_PROJECT_NAME=(same as dev site)
CUSTOM_IMAGE_NAMESPACE=(same as dev site)
CUSTOM_IMAGE_NAME=(same as dev site)
DOMAIN=your-domain.com
```

Once this is set up, run `make production` to install your drupal site. You can generate new secrets for your production passwords, or 
copy them from your dev server if you would like them to be the same.

At this point, your site is ready, but you won't be able to access it at your URL until you update the SSL certificates (see TLS section below).

!!! note "Codebase Folder"

    Because your codebase folder is in your git repository it will be cloned to your development server, but unlike in your development
    environment, it is not bound to the Drupal container. This means that any change you make to those files will not be represented in 
    your Drupal site.
    
    You can also use this folder to build your Drupal image on the production server instead of on your development server if you like.

## Secrets

Sensitive information, such as passwords, should never be built into a container.  It also shouldn't ever be bind-mounted in like we
do with our codebase folder.  If you use secrets, it's like bind-mounting in a file, except that file is provided from the host machine
to the container using an encrypted channel.

Secrets are on by default. They can be toggled in your `.env` file, but you should never turn them off for production sites.

```
USE_SECRETS=true
```

The secrets themselves are stored in the `secrets/live` folder of `isle-dc`.  If you navigate to that directory, you'll see several small
files, where each file represents a different password. When you run `make production` you can choose whether to generate new random 
secrets or create them yourself.

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

### Requesting Certifcates through ACME and External Account Binding

To request certificates through another Certificate Authority (CA) that supports External Accounting Binding through ACME such as InCommon or ZeroSSL you will need to add the following to your `.env` file:

```
USE_ACME=true
ACME_EMAIL=your-email@example.org
ACME_SERVER=
ACME_EAB_KID=
ACME_EAB_HMAC=
```

Where `ACME_SERVER` is the CA server to use, `ACME_EAB_KID` is the key identifer from the External CA, and `ACME_EAB_HMAC` is the HMAC key from the External CA.

Once you have added these commands you will need to run the following commands:

```
make -B docker-compose.yml
make up
```
