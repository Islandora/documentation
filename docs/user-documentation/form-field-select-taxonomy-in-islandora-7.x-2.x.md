# Creating a Taxonomy you can use in your Islandora Content Type

## Taxonomies

### Drupal vs Islandora XML Forms
Taxonomies and the ability to incorporate them into Islandora XML Forms have been something the community has mostly worked around.  Drupal's taxonomy module allows the user to create a standard set of terms that can be leveraged in a content type.  The select field type in Islandora XML Forms becomes unmanageable when you had more than 10 items (it was easier to add the items in plain XML, than it was to add them through the XML Forms interface).  Drupal taxonomies will hopefully simplify the standardization, maintenance and use of vocabularies within the Islandora framework.

### Examples

#### Adding a Drupal Taxonomy based on DublinCore's DCMI Type Vocabulary
"The [DCMI Type Vocabulary](http://dublincore.org/documents/2000/07/11/dcmi-type-vocabulary/) provides a general, cross-domain list of approved terms that may be used as values for the Type element to identify the genre of a resource."<sup>1</sup>  

The goal of this task is to create a taxonomy that can be used as a field with the field type of `Term Reference` and uses the `Select` Widget.  `Term Reference` will draw on our DCMI Type taxonomy.

**Create a new Taxonomy**

  1. To create a new taxonomy, select `Structure > Taxonomy > Add taxonomy`. I'm going to call my taxonomy `DCMI Type`.
    ![Add a new taxonomy](https://lh3.googleusercontent.com/_wAZTJPbWsVt8EebdvlX7xkQbujV-8QJsVGeZqmPqQQ=s700 "Add a new taxonomy")
  2. The next thing you'll want to do is select `+Add term` to add terms to your taxonomy. The DCMI has [provided a list](http://dublincore.org/documents/2012/06/14/dcmi-terms/?v=elements#H7) with definitions.
   ![Add Taxonomy Term](https://lh3.googleusercontent.com/hz_Ybz9XmbYBM8a5QaTaCcXykt-SH88VBUX3ubuQxhc=s700 "Add Taxonomy Term")
  3. Keep adding terms until you've captured the terms and save your taxonomy.
   ![DCMI Taxonomy](https://lh3.googleusercontent.com/hYY0ldThWXSMQyaG6aiPY00fpy031H1Hd7ReWJNAvPY=s700 "DCMI Taxonomy")

See [Editing an Ingest form](editing-basic-image-form-in-islandora-7.x-2.x.md) to see how we create a new field in the Basic Image content type.

----

<sup>1</sup> http://dublincore.org/documents/2000/07/11/dcmi-type-vocabulary/
