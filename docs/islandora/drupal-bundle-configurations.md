## Drupal Bundle Configurations

In Islandora, [content models](https://github.com/Islandora/islandora/wiki/Content-Models) are primarily created using content types (also known as node bundles) and media bundles. [Bundles](https://www.drupal.org/docs/8/api/entity-api/bundles) are defined by [YAML](http://befused.com/drupal/yaml) configuration files. [To create new content models](https://www.drupal.org/docs/8/api/entity-api/creating-a-custom-content-type-in-drupal-8), one would create the needed content types and media bundles via UI, then export the yml files related for those bundles using Configuration Synchronization (`http://localhost:8000/admin/config/development/configuration`) or [Features](https://www.drupal.org/project/features). An understanding about the structure of a bundle and various configuration files used to define it helps in creating and updating it.

Content types and media bundles can be thought of as web [forms](https://www.drupal.org/docs/user_guide/en/structure-widgets.html) consisting of fields. Drupal provides [widgets](https://www.drupal.org/docs/8/creating-custom-modules/create-a-custom-field-widget) to define the behavior of a field and field storage to define how the data is stored in the database. Drupal provides various [display modes](https://www.drupal.org/docs/8/api/entity-api/display-modes-view-modes-and-form-modes) to show the forms to user when they are editing (Manage form display) or viewing (Manage display).

A content model is packaged as a module for installation. All yml files are put in `config/install` folder of the module. Note that not all content models would contain media bundles.  

The following files define the bundles themselves. It contains some metadata about the bundle and lists its dependencies.  
```
node.type.your_content_type.yml
media_entity.bundle.your_media_bundle.yml
```

The following files define the fields attached to the bundle forms. There must be one config file for each field in your bundle, except for the default drupal fields.  
```
field.field.node.your_content_type.field_name1.yml
field.field.node.your_content_type.field_name2.yml
...
field.field.media.your_media_bundle.field_name1.yml
field.field.media.your_media_bundle.field_name2.yml
```

If the new bundle contains new fields, then field storage configurations for the newly created fields would be needed as well. Note that if you reused existing fields, storage definitions should not be defined again. Storage config contains information about the number of values allowed for that field (cardinality).  
```
field.storage.node.field_new_name3.yml
field.storage.media.field__new_name3.yml
```

There is a configuration file for each combination of bundle / display mode when managing form displays. Usually, form displays will have `default` and `inline` modes.
```
core.entity_form_display.media.your_media_bundle.default.yml
core.entity_form_display.media.your_media_bundle.inline.yml
---
core.entity_form_display.node.your_content_type.default.yml
core.entity_form_display.node.your_content_type.inline.yml
```

There is a configuration file for each combination of bundle / display mode when managing displays. Usually, displays will have `default` and `teaser` modes for content types and `default` and `content` modes for media bundles.
```
core.entity_view_display.media.your_media_bundle.default.yml
core.entity_view_display.media.your_media_bundle.content.yml
---
core.entity_view_display.node.your_content_type.default.yml
core.entity_view_display.node.your_content_type.teaser.yml
```

In addition, Islandora needs a RDF mapping to express the content in RDF and to sync to fedora. There will be one RDF mapping per bundle.
```
rdf.mapping.media.your_media_bundle.yml
rdf.mapping.node.your_content_type.yml
```
