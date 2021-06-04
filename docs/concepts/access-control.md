# Access Control in Islandora

This page is about controlling **who can view or edit your Islandora content** (metadata and binaries).

Islandora recommends using Drupal's access control features. Contributed modules such as those described below, can provide additional flexibility and configurability. However, these only apply when content is accessed through Drupal, and are not applied to data in Fedora, the Triplestore, or Solr.

Known strategies for implementing access control in Drupal include:

- Core Drupal
- Group
- Permissions by Term
- Field Permissions

!!! danger "Exposed Endpoints"
    If access control is a concern, you will want to lock down access to the following services that are part of the Islandora Suite:
    
    - Fedora (REST API and admin client)
    - Solr (API and admin client)
    - Blazegraph (API and admin client)
    
    as anyone accessing data through those services are bypassing Drupal's access control. In all out-of-the-box sandboxes and demo instances, these services are wide open.
    
## Access Control in Core Drupal

**Summary: Core Drupal's access control features include the "published"/"unpublished" states and some basic permissions that can be granted globally or at the bundle level.**
 
Drupal's concepts of "published" and "unpublished" for nodes and media apply to Islandora just as they do in Drupal. _Usually_, published content is visible to the entire world (without any authentication), while unpublished content can only be accessed by selected users - usually administrators, other privileged roles, and the user who created the content. This is configurable through the Drupal Permissions interface. 


All Drupal permissions are granted to _roles_, not individual users. Read more [documentation on Users](/user-documentation/users). 

The extent of the configurability of Drupal Core's access control (excluding revisions) is provided by the following permissions:

| View permissions (apply to _all_ content or _all_ media regardless of bundle) |
|---|
| - View all published content |
| - View own unpublished content |
| - View (published) media |
| - View own unpublished media |

| "Editing" permissions (can be granted for a specific bundle, or for all nodes or media):  |
|---  |
| - Create new content |
| - Edit (your) own content |
| - Edit any(one's) content |
| - Delete (your) own content |
| - Delete any(one's) content |

| Administrative permission (includes all the above and more, give to trusted users only): 
| --- |
| - Administer content  |


These are the basic access control options built into Drupal Core. There are many contributed modules that do access control, some are described below. Before using access control modules, please see [Comparison and Overview of Access Control modules](https://www.drupal.org/node/270000) on drupal.org.

Contributed modules are required for the following cases:

- individual nodes or media having their own access policies (in Core Drupal, access can only be configured at the content type or media type level)
- access policies that grant privileges to _users_ (in Core Drupal, access can be granted only to roles, and/or to that content's author)
- contents of specific "management" fields being reserved so that only privileged users can view or edit. 

### Islandora Resources

Given the [Collection] → Node → Media → File structure of Islandora resources, it is reasonable to wonder if access to a Node, Media, or File is influenced by the permissions on its "parent" resource. 

There is not yet a mechanism for Drupal's access control to be "inherited" from parent resources to child resources. An exception is non-public files, as described below.

### Sidebar: Access to files in Drupal

Drupal's access control applies to files __except those stored in Drupal's public filesystem__. 

If you upload a file to Drupal, it's usually stored in a field on an entity. In Islandora, that entity is a Media. Drupal's access checks mean that a user must have permission to view the Media, and (if configured) the file field on that media, to see the file. However, file fields can be configured with various forms of file storage, and the file storage determines whether access control checks are applied at all. Files stored in Drupal's public filesystem are always public, viewable by anyone in the world. 

Drupal's access control applies to files _accessed through Drupal_ where the file storage is:

- on Fedora using Flysystem  
- on a Drupal private filesystem
- on another location such as S3 or Dropbox, through flysystem.

!!! warning "Direct access to file storage"
    Files stored using Flysystem are actually located on a separate service. It is the **administrator's responsibility to implement appropriate access control on the target storage system**, as there is a way to link directly to files in Fedora, S3, or Dropbox. These direct links bypass Drupal and therefore Drupal's access control. 

The filesystem used by a field is usually configured through "Manage Fields" on the appropriate entity. However, it is possible for a field to be _configured_ to use a certain filesystem, but for _actual files_ created through back-end processes to use a different one. For example, derivatives generated by Islandora in Islandora Defaults are written to the public filesystem, despite the field configuration which points to Fedora.

To determine the actual location of a file, right-click the existing file in the Media Edit page and select "copy location". A file that is saved in flysystem will include `/_flysystem/` in the URL. These links (you can test it!) only work if you have permission to view the media. A file with `/sites/default/files/` is probably publically accessible (this is the public filesystem on most Drupal instances). 

!!! note "In Defaults, All Derivatives Are Public"
    Out of the box using Islandora Defaults, derivatives are created for all Islandora media (as long as the conditions of the contexts aree met), and they are stored in the public Drupal filesystem. This happens even if the media and/or node are not published. 
 

## Access control in Solr

The Search API Module, which connects Drupal to Solr, provides configuration so that access considerations are respected by the Drupal solr results display. The "Processors" configuration (e.g. at admin/config/search/search-api/index/default_solr_index/processors) provides checkboxes for the following options:

- "Content access": Adds content access checks for nodes and comments
- "Entity status": Exclude inactive users and unpublished entities (which have a "Published" state) from being indexed. 

Both are enabled out of the box in Islandora Defaults. This will ensure that queries through Drupal never show content that the active user shouldn't see, as well as preventing information about unpublished entities from ever being entered into Solr.

!!! warning "Solr Admin Client"
    Anyone with access to the Solr Admin Client may see the full contents of the index, regardless of permissions. In Islandora Defaults, the Solr client at http://127.0.0.1:8983/solr/#/ISLANDORA is open to the world, and all published content (and user information) is accessible.


## Group (contributed module)

"The [Group](https://www.drupal.org/project/group) module allows you to create arbitrary collections of your content and users on your site and grant access control permissions on those collections."

!!! note "Opinion"
    Group is one of the more hefty modules, and is difficult to learn. It is good if you have static or semi-static groups of users who need access to a static or semi-static group of content, but sharing an item with an arbitrary group of users is cumbersome. 

## Permissions By Term (contributed module)

"The [Permissions by Term](https://www.drupal.org/project/permissions_by_term) module extends Drupal by functionality for restricting view and edit access to single nodes  via taxonomy terms. [...] Taxonomy term permissions can be coupled to specific user accounts and/or user roles." By default, this module only affects nodes. To enable Permissions by Term for Media and other entities, enable the "Permissions by Entity" submodule.

Islandora Defaults includes an empty vocabulary called "Islandora Access", which is intended to hold such taxonomy terms. However, permissions_by_term must be installed and configured on your own.

This module is known to work, and appears to be supported well by the Drupal community.

!!! warning
    This module is known in the Islandora community to cause performance degradation when large numbers of nodes are involved. Seth Shaw describes this in a blog post, [Content Access Control Solutions Investigation](https://seth-shaw-unlv.github.io/2021-02-19/content_access_control).

!!! question "Examples Wanted"
    Do you have experience setting up Permissions By Term? We'd love some illustrative examples here.

## Field Permissions (contributed module)

"The [Field Permissions](https://www.drupal.org/project/field_permissions) module allows site administrators to set field-level permissions to edit, view and create fields on any entity."



## Other contributed modules

Workflow Participants allows for granting permissions on individual nodes or media to individual users, as an extension to the Workflow suite of modules. However, it is not well supported and the "manage workflow participants" permission should not be given to untrusted users as it may grant users the ability to add participants even to content they are otherwise not able to see or edit. 

## Access control in Fedora

In Fedora it is possible to control access to resources using Access Control Lists (ACLs) per the Fedora API Specification. ACLs are inherited through Fedora's LDP Containment relationships.

Islandora does not create customized ACLs when syncing content into Fedora. 

Islandora does not create Fedora resources in hierarchies that use LDP containment relationships. 


At this time, access control in Fedora is NOT reflective of access control in Drupal. In the sandboxes and demo installations, all resources in Fedora are open for the world to view. This includes unpublished nodes and media, as well as the files uploaded into Fedora. Access controls configured in Drupal are not synced in any way to Fedora.

Sites concerned with access control  wish to "lock down" their Fedora to only be accessible through Drupal. 

## Access control in the Triplestore

In Islandora Defaults, there is no access control on the Blazegraph triplestore. Islandora defaults is configured so that all RDF triples sent to Fedora are also populated into the triplestore. If using the Ansible Playbook, it is available at http://localhost:8080/bigdata/. Like the solr index, it is open to the world by default.


## See Also

[Meta-Issue: Access Restrictions and Embargoes](https://github.com/Islandora/documentation/issues/928)
