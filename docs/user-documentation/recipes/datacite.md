# Automatically Minting DOIs with DataCite

Islandora can be configured to automatically mint DOIs with DataCite when saving an object (either a new object or an existing one). It does this by sending the data to the DataCite API to create the DOI and link it to the Islandora object URL, then updating the Islandora Object's metadata to contain that DOI.

In order to implement this, we need to add two modules:

- [DGI Actions](https://packagist.org/packages/discoverygarden/dgi_actions)
- [Islandora DataCite DOI](https://packagist.org/packages/islandora-rdm/islandora_datacite_doi)

And activate them:

![Screenshot showing the Drupal extend page with only the DGI Actions and Islandora DataCite DOI modules activated](../assets/dgi_actions.png)

## Configuration

The configuration for minting a DOI is set up in multiple places. Configuration examples can be found in the [Islandora Datacite DOI README](https://github.com/roblib/islandora_datacite_doi/blob/2.x/README.md). You should reference this as you work through the following steps.

### DGI Actions Configuration

The DGI Actions module adds 3 new configuration options, which can be found at `/admin/config/dgi_actions/identifiers`.

#### Data Profile Entities

This is where we configure which fields in our node will be sent to DataCite.

#### Service Data Entities

This is where we configure our DataCite options. We can specify the API URL so we can use either the live or test server, add our username & password, and choose which DOI prefix to use when minting.

#### Identifier Entities

This is where we configure what field the newly minted DOI should be added to, and choose which data profile and service data to use. Choose the data profile and service data you added in the previous two steps.

### Actions

A Drupal action is the thing that triggers the minting. You can add a new action at `/admin/config/system/actions`.

If you have configured things correctly in the previous steps, you should see an action corresponding to the DOI minting for the nodes you specified.

### Contexts

This is where we specify when the DOI should be minted. You can add a new context at `/admin/structure/context/`.

You need to add the condtion "Entity Bundle" in order to trigger this on adding/editing a node. Beyond that, you can configure this however you like, but will most likely want to add the condition "Entity has a persistent identifier" and negate it. This makes the system only mint DOIs if one doesn't exist, so that you won't create a DOI for something that already has one.

You may also want to use the "request domain" condition so that you only mint DOIs for content on your live site, and not on your development or staging sites.

In the reaction section you will choose the action you created in the previous step.