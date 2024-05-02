# Breadcrumbs

Breadcrumbs are a Drupal concept. They provide a hierarchical path
of links to "ancestors" of the current content item.

![example of breadcrumbs](../assets/breadcrumbs-example.png)

## Islandora Breadcrumbs

Islandora provides a module, "Islandora Breadcrumbs" (a submodule
of the Islandora module) that creates breadcrumbs based on the value
of configured reference fields (by default, `field_member_of`). 
To use Islandora Breadcrumbs, simply enable
the module. Islandora breadcrumbs will apply to nodes that have
the configured Entity Reference fields.

There are a few configuration options for this module, accessible at
**Manage** > **Configuration** > **Islandora** > **Breadcrumbs 
Settings** (`/admin/config/islandora/breadcrumbs`). These include:

* Maximum number of ancestor breadcrumbs - an optional feature to 
stop adding "ancestor" links after a certain number
* Include the current node in the breadcrumbs?
* Entity Reference fields to follow - if you're using other fields
to refer to parents, you can add them here.

![configuration screen](../assets/breadcrumbs-config-screen.png)

## Troubleshooting Breadcrumbs

Breadcrumbs are cached, so if you aren't seeing the results that 
you expect, try clearing the Drupal cache.
