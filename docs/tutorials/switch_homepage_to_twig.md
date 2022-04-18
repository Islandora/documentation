# Format Homepage with TWIG

## TWIG Debugging
It's helpful to identify which TWIGs are available to use and where they're stored but not required use TWIGs to format the homepage.

![Screenshot from 2021-12-10 11-22-31](https://user-images.githubusercontent.com/2738244/145607034-967cc164-9d24-4f6d-aac7-9b3b93c87c4e.png)

```shell
# Copy the default service.
$ cp web/sites/default/default.services.yml web/sites/default/services.yml

# fix permissions (just in case)
$ chown nginx:nginx web/sites/default/services.yml

# I use nano to edit but you can pick whichever editor you want.
# For this example we'll install the editor
$ apk add nano

# Now open the newly created service file and set these 3 values under the TWIG config section.
$ nano web/sites/default/services.yml

...yml
twig.config:
  debug: true
  auto_reload: true
  cache: true

 # Now save and exit (in NANO it's CTRL + x)

```
For a video tutorial on this, see [Enabling Twig Debugging in Drupal 8/9](https://youtu.be/6WMr5V_LQ1w)

## Copying Templates
Copy the default TWIG into your theme's template directory.

```shell
$ cp web/themes/contrib/bootstrap/templates/node/node.html.twig web/themes/contrib/solid/templates/node--6--full.html.twig

# Clear cache
$ drush cr
```
And now if you view the home page's source code you should now see the the `X` next to the loaded TWIG file. Please note that the file name corresponds to the node number. To use the URL alias instead of the node ID requires additional work. [Here](https://www.lehelmatyus.com/1064/drupal-8-page-template-suggestion-by-path-alias)'s a tutorial on this topic.
```html
<!-- FILE NAME SUGGESTIONS:
   x node--6--full.html.twig
   * node--6.html.twig
   * node--page--full.html.twig
   * node--page.html.twig
   * node--full.html.twig
   * node.html.twig
-->
```

Now edit the TWIG file (web/themes/contrib/solid/templates/node--6--full.html.twig) to say whatever you want and it should show up immediately without needing to clear cache.

## Clean up
Don't forget to turn off TWIG debugging in config file (web/sites/default/services.yml). This will likely have unexpected consequences on production system performance.

```yml
twig.config:
  debug: false
  auto_reload: false
```
