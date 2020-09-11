# Alpaca Tips

[Alpaca](https://github.com/Islandora/Alpaca) is event-driven middleware based on [Apache Camel](https://camel.apache.org/) for Islandora

Currently, Alpaca ships with four event-driven components which are being run with [Apache Karaf](https://karaf.apache.org/)
- [islandora-connector-derivative](#islandora-connector-derivative)
- [islandora-http-client](#islandora-http-client)
- [islandora-indexing-fcrepo](#islandora-indexing-fcrepo)
- [islandora-indexing-triplestore](#islandora-indexing-triplestore)

## islandora-connector-derivative
This service receives requests from Drupal when it wants to create derivatives and passes that request along to a microservice in [Crayfish](https://github.com/Islandora/Crayfish). When it receives the derivative file back from the microservice, it passes the file back to Drupal.

## islandora-http-client
This service overrides the default http client with Islandora specific configuration.

## islandora-indexing-fcrepo
This service receives requests from Drupal in response to write operations on entities. These requests are passed along to [Milliner](https://github.com/Islandora/Crayfish/tree/dev/Milliner) microservice in [Crayfish](https://github.com/Islandora/Crayfish) to convert Drupal entities into Fedora resources and communicate with Fedora (via [Chullo](https://github.com/Islandora/chullo)).

## islandora-indexing-triplestore
This service receives requests from Drupal on indexing and deleting in order to persist/delete content in the triplestore.


## Steps for developing with Alpaca
1. Clone the Alpaca github repository Alpaca into /home/karaf in your vagrant vm (assuming you're developing with [islandora-playbook](https://github.com/Islandora-Devops/islandora-playbook))
    i. `git clone https://github.com/Islandora/Alpaca.git`
2. Modify java files as needed.
3. Run `sudo ./gradlew clean build install` from `/home/karaf/Alpaca` to rebuild the jar's. (This process runs the tests as well.)
4. Start the karaf client- run `./bin/client` in `/opt/karaf`.
5. Run `feature:list | grep islandora` to see all the features from islandora.
6. Run `feature:uninstall featurename` on each of those (inserting the name of the feature for featurename). An example might be `feature:uninstall islandora-indexing-fcrepo`
7. Run `repo-list` to see all of the repositories installed.
8. Run `repo-remove` the one that says islandora. It might look something like `repo-remove islandora-karaf-1.0.1`.
9. Add your new, local features file (that was made when you ran step 3). It should be somewhere like `/home/karaf/Alpaca/karaf/build/resources/main/features.xml` and would be added with `repo-add file:/home/karaf/Alpaca/karaf/build/resources/main/features.xml`.
10. Run `feature:list | grep islandora` should reveal the uninstalled features.
11. Run `feature:install featurename` on each of those.

If you encounter an issues, check the logs in `/opt/karaf/data/log/karaf.log`
You may need to uncomment the line `#org.ops4j.pax.url.mvn.localRepository` in the `/opt/karaf/etc/org.ops4j.pax.url.mvn.cfg` file in order to be able to find the local jars
