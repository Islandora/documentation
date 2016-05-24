# Intro to Islandora CLAW

## What is Islandora CLAW
Islandora CLAW is the project name for development of Islandora to work with Fedora 4. The current release of Islandora, known as 7.x-1.x, works as a bridge between Drupal 7.x and Fedora 3. Put simply, Islandora 7.x-1.x is middleware between Fedora 3 and Drupal 7.x, sometimes expressed as a hamburger:

![image](https://cloud.githubusercontent.com/assets/2371345/15516210/2ac2c2ec-21c8-11e6-8d34-ce6199d169a3.png)

Islandora CLAW (or Islandora 7.x-2.x) does more than simply replace that base layer with Fedora 4. It is a total re-architecting of the interaction between the various pieces. Rather than a hamburger, Islandora CLAW is a chimera:
![image](https://cloud.githubusercontent.com/assets/2371345/15516273/76704a5c-21c8-11e6-9ca0-8c188313dbd1.png)

This new structure has several advantages:

* Parcelling out the various services and dependencies allows for more horizontal scalability
* Changing the relationship between Drupal and Fedora allows for a more flexible approach to front-end management (i.e, it need not be Drupal) while also taking much greater advantage of features available from Drupal (i.e, Fedora objects are treated more like nodes, for the purposes of using Drupal contrib modules. Many Islandora 7.x-1.x modules are redundant in Islandora CLAW because they reproduce existing Drupal contrib modules that can be used out of the box in Islandora CLAW).
* Easier to keep up-to-date as not all pices must be versioned together.
* Installations tools (ansible, Docker, Vagrant) can automate uniting the pieces in a simple, clean installation process.
