# Islandora Data Model

In Islandora, your content is Drupal content, and you have the power to arrange it as you wish. However, Islandora assumes the following organizing data model:

- The primary metadata objects of your repository are represented as Drupal nodes, which contain descriptive metadata
- The files to store in your repository are added as Drupal media
- Nodes are connected to the Media that they are about
- Nodes may be members of other nodes, to create hierarchical arrangements within the repository
- You may wish to automatically generate auxilliary files, called derivatives, from uploaded files or other from derivatives
- You may wish to change how objects are displayed under certain conditions
- The repository may be represented as RDF
  
To meet these requirements, Islandora provides implimentations of the following concepts to help build your repository:

- Islandora Models, within a single content type
- Media "belonging to" nodes
- Membership of nodes within other nodes
- Media tagged with their intended "use".
- a microservices-based system to generate derivatives
- increased functionality of Contexts to perform if-then logic
- synchronizing of files and metadata into RDF-based systems

Each of these tools will be described in the pages following.

An example of using these tools provided by Islandora is provided in Islandora Defaults, which creates content types, fields, contexts, and other configurations to make these work together. See Islandora Content Models for more information.


