# Remove docker compose profiles from ISLE Site Template

ISLE Site Template used to utilize docker compose profiles to manage a `dev` and `prod` profile for docker services.

Removing compose profiles if completely optional. If you are happy with your setup, you do not need to remove profiles. However, if you'd like to remove the profiles and align with the latest community version, the steps to do so are outlined below.

## 1. Bring in the new config

In your dev environment, bring in the new config from ISLE Site Template

```
cd path/to/your/site/template
git checkout -B rm-compose-profiles
git remote add ist https://github.com/islandora-devops/isle-site-template
git fetch ist
git checkout ist/main -- conf Makefile sample.env scripts docker-compose.dev.yml
docker compose --profile dev down
```

## 2 - Remove compose profiles

You now have two options to remove profiles. Choose either 2.a or 2.b

### 2.a - Vanilla overwrite

If you've made no changes to docker-compose.yml since you made your own copy of isle-site-template you can simply overwrite the docker-compose.yml with the latest version 

```
git checkout ist/main -- docker-compose.yml
```

### 2.b - Attempt to merge

If you have made changes since copying the repo, you can attempt to convert it using `sitectl`. Install sitectl and run the `sitectl isle migrate merge-compose-profiles` command

```
brew tap libops/homebrew
brew install libops/homebrew/sitectl-isle
sitectl isle migrate merge-compose-profiles -i docker-compose.yml -o docker-compose.yml -f
```

This will overwrite your `docker-compose.yml` with the new profiles-less version.

## 3 - Review

Very carefully review your changes locally

```
git diff
```

Or if you prefer viewing the diff in your git provider

```
git add .
git commit -m "Removing profiles"
git push origin rm-compose-profiles
```

Ensure your services have the same environment variables, secrets, volumes your old services had. Ensure your volume names stayed the same

Familiarize yourself with the new make command helpers

```
make help
```

Ensure the new values you see in `sample.env` are set in your `.env`. Namely

```
URI_SCHEME=http
TLS_PROVIDER="self-managed"
ACME_URL=https://acme-v02.api.letsencrypt.org/directory
ACME_EMAIL=postmaster@example.com
```

## 4 - Test

If you want to keep running TLS in your dev environment you can run this make helper to switch to `https`:

```
make traefik-https-mkcert
```

Otherwise, be sure your `DOMAIN` value in `.env` is `islandora.traefik.me`.

Now, bring up your services. In a development environment, after running `make up` your ISLE site should be available at `http://$DOMAIN`.


## 5 - Cleanup (optionally revert)

Switch to your main branch

```
git checkout main
```

If the changes didn't apply smoothly you can simply checkout back out main branch and delete the git branch we created

!!! warning
    This will delete any config settings made in previous steps
    ```
    git branch -D rm-compose-profiles
    ```

Instead, if you like this new setup and all looks well, merge the `rm-compose-profiles` git branch into main

```
git merge rm-compose-profiles
git push origin main
```

Finally, remove the git remote we made

```
git remote remove ist
```

## 6 - Deploy to production

If you committed the changes, deploy to production by:

First, update .env with the new `.env` values. If you're using letsencrypt it will probably look like

```
URI_SCHEME=https
TLS_PROVIDER="letencrypt"
ACME_URL=https://acme-v02.api.letsencrypt.org/directory
ACME_EMAIL=postmaster@example.com
```

If you're using your own TLS certs, it'll probably look like

```
URI_SCHEME=https
TLS_PROVIDER="self-managed"
```

Next, bring down the site, pull in the changes, and bring everything back up

```
docker compose --profile prod down
git pull origin main
make up
```

Your site should be online now using the new setup!
