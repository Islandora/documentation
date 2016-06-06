# Intro to Islandora CLAW

Islandora CLAW is the project name for development of Islandora to work with Fedora 4. The current release of Islandora, known as 7.x-1.x, works as a bridge between Drupal 7.x and Fedora 3. Put simply, Islandora 7.x-1.x is middleware between Fedora 3 and Drupal 7.x, sometimes expressed as a hamburger:
## Islandora 7.x-1.x (with Fedora 3)
![image](https://cloud.githubusercontent.com/assets/2371345/15615690/20e0a050-2416-11e6-813b-509fd2e197ed.png)

Islandora CLAW (or Islandora 7.x-2.x) does more than simply replace that base layer with Fedora 4. It is a total re-architecting of the interaction between the various pieces. Rather than a hamburger, Islandora CLAW is a chimera:
## Islandora CLAW/Islandora 7.x-2.x (with Fedora 4)
![image](https://cloud.githubusercontent.com/assets/2371345/15516273/76704a5c-21c8-11e6-9ca0-8c188313dbd1.png)

Or, for a diagram that doesn't involve food or animals: 
![image](https://cloud.githubusercontent.com/assets/2371345/15831747/702256cc-2bf5-11e6-828b-7e3bc81d6c7f.png)

This new structure has several advantages:

* Parcelling out the various services and dependencies allows for more horizontal scalability
* Changing the relationship between Drupal and Fedora allows for a more flexible approach to front-end management (i.e, it need not be Drupal) while also taking much greater advantage of features available from Drupal (i.e, Fedora objects are treated more like nodes, for the purposes of using Drupal contrib modules. Many Islandora 7.x-1.x modules are redundant in Islandora CLAW because they reproduce existing Drupal contrib modules that can be used out of the box in Islandora CLAW).
* Easier to keep up-to-date as not all pices must be versioned together.
* Installations tools (Ansible, Docker, Vagrant) can automate uniting the pieces in a simple, clean installation process. Islandora has always beena turn-key repository solution, in the sense that it needs no customization once installed. With Islandora CLAW, that installtion process is much smoother, with automated solutions that can be used in production.

## Ecosystem

#### [Alpaca](https://github.com/Islandora-CLAW/Alpaca)
Event driven middleware based on Apache Camel that synchronizes a Fedora 4 with Drupal.

#### [Chullo](https://github.com/Islandora-CLAW/chullo)
A PHP client for Fedora 4 built using Guzzle and EasyRdf.

#### [CLAW](https://github.com/Islandora-CLAW/CLAW)
A container with references to all of the other components that make up Islandora CLAW. Also contains documentation and project discussions.

#### [Crayfish](https://github.com/Islandora-CLAW/Crayfish)
Top level container for the various Islandora CLAW microservices.

#### [Islandora](https://github.com/Islandora-CLAW/islandora)
The Drupal modules associated with CLAW. Work in underway to have this as a project on [drupal.org](https://www.drupal.org/).

#### [PDX](https://github.com/Islandora-CLAW/pdx)
Top level container for the various PCDM specific Islandora CLAW microservices.

## Why Switch?

Islandora 7.x-1.x is a tried and tested product with a vibrant user and contributor community and many tools and add-ons available to customize and expand its uses. Islandora CLAW is a major departure, representing a great deal of work both in development of the software, and to migrate and upgrade existing Islandora sites. So why make the change?

### Fedora
The primary motive for moving to Islandora CLAW is to stay current with the repository layer of our ecosystem. Moving to Fedora 4 is vital for the long-term utility of the project.

* Fedora 3 is End-of-Life and has not been supported since 2015. This means that there will be no improvements, bug fixes, or security patches released for the software, drastically increasing individual maintenance needs for adopters. 
* Fedora 3 experiences significant slowdown when repositories reach a large (millions) number of objects. Fedora 4 is much more scalable, and is specifically engineered to be horizontally scalable in ways that Fedora 3 cannot.

### Drupal
[Drupal 8](https://www.drupal.org/8) has been officially released and development has begun on Drupal 9. Official Drupal policy will see Drupal 7 become unsupported when Drupal 9 is released, putting it in the same precarious territory as Fedora 3. 

Current Islandora CLAW development works with Drupal 7 as a front-end, but Islandora's CLAW's structure has been built with a pivot to Drupal 8 in mind from its very inception, and work is underway to ensure that when the Islandora community is ready to switch to Drupal 8, Islandora will be there with a solid Drupal 8/Fedora 4 platform. 

### Community-Driven Design

The Islandora community has grown significantly since the project began, both in terms of users and contributors. For the first time, we are in a position to have a truly community-driven development process, in which any Islandora user can participate at whatever level fits their interests and abilities. Islandora CLAW has worked with the broader Islandora community to solicit use-cases, put forward prospectuses and project plans for review and editing, and has been developed with a mix of funding from Islandora Foundation members and volunteer developers working on sprints.

Islandora CLAW is developed _by_ the Islandora community, _for_ the Islandora community. As a member of the Islandora community, you can help to steer the direction it takes.

### Linked Data

### Interoperability

Islandora CLAW works with the [Portland Common Data Model](https://github.com/duraspace/pcdm/wiki), "a flexible, extensible domain model that is intended to underlie a wide array of repository and DAMS applications." What does this mean in practice? Objects stored in Islandora CLAW will have a a data model that can be recognized by Hydra and other custom Fedora 4 based repository heads, and vice versa. By working together with the Hydra and Fedora communities, we can leverage a broader community of developers, librarians, and other digital repository users to build better tools and share reosurces.
