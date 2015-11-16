# Editing the Basic Image Form

## Goodbye Islandora XML Forms ... Hello Drupal Field UI
Those Islandorians that have used (and love) the XML Form Builder will notice a substantial difference in how forms are constructed and [how the fields are displayed](#). The full functionality of the XML Form Builder is not replicated in the 7.x-2.x version of Islandora yet, so help the community with working that out.

## Drupal Field UI

To use Islandora-speak the ingest form is now a Drupal content type and a content type is built using Drupal's Field UI. Readers should first review the [associated Drupal documentation](https://www.drupal.org/documentation/modules/field-ui) as it provides a foundational understanding of content types and their fields.  
> Once you have created content with your content type, it is very difficult to change so planning and modelling your data before hand is important.

### Each field has:

 - a label
	 - eg. `DC Title`
 - a machine name
   - eg. `field_dc_title`
   - note Drupal powered vs Islandora XML Forms:
     - once you have created/defined a field in a content type, **you can reuse that field in other content types**  
     -  As an example you can create a field for Dublin Core DCMI types and then use that field in any other content type.
 - the type of data to be stored
	  - eg. `Long Text`
 - the widget or form element used to enter/edit the data
	  - eg. `Text area (multiple rows)`
  
  ![Basic Image Content Type Fields](https://lh3.googleusercontent.com/KNYo8ZcId9on25asEj-fXGJb7QyGlpGzl29khZBlze8=s700 "Basic Image Form")
  
  *Screenshot of Basic Image Fields*

  Depending on the Widget selected, you will define further properties for the field you've created.

## Editing the Basic Image Content Type

### Rearranging fields
The Drupal Field UI allows you to easily rearrange fields in the order you wish to present them to the user. In the screenshot above you'll note that I've dragged the DC Title field to the top of the list of fields. Don't forget to `Save` your changes.

### Editing an existing field

As an example if we want to edit the DC Title field, select `Edit` from the operations column for that field.  The Edit tab for the field displays the editable properties including:

 - is it a Required Field?
 - an option to provide or update Help text for the field to guide user or explain the data entry practice for the field.
 - the type of text filter to apply.
 - provide a default value for the field
 - permissions for the field (who can edit and/or view it)
 - rdf mapping for the field

  ![Field Edit Tab](https://lh3.googleusercontent.com/b9mrNy6Sb7I0mFb03AsgHUq27vmtF6uVVP8qz9DdoTA=s700 "Field Edit Tab")

  *Field Edit Tab*

If your field has existing data (eg. you created content using the content type already), then the changes you can apply to the field are somewhat limited.

The Field Settings tab for the DC Title field warns the user that it can't be changed because data already exists in that field. That's why planning your fields and defining their properties is so important.

  ![Field Settings](https://lh3.googleusercontent.com/p5EQBfo3Rrm0BmEeFab-kSTDVtCw6GJnLwNoOmzPTW8=s700 "Field Settings")

  *Field Settings Tab*

Depending on the field type selected, different widgets may be displayed.

  ![Field Widget](https://lh3.googleusercontent.com/KjaUMYWwE6gtk_pzoeElXyeswZiR1y4k98kBkGpWpUU=s700 "Field Widget")

  *Field Widget Tab*

### Adding a new field
#### Adding a simple select field

One of the ways to improve the user experience, data quality, and faceted displays with forms is to provide select or autocomplete options for your users.  As an example, the goal of these steps is to add a select field to your content type.
  
  1. Create a new field called Image Type, with a Field Type of List (Text) and a Widget of Select List.
    ![Add a field called Image Type](https://lh3.googleusercontent.com/IjqJMVoRnAqg3cHSWFHW1NXEOlIXMu9RLL-T96H_T8U=s700 "Image Type Field" )

  2. We'll need some terms for our Image Type field. Whenever possible it is best to use a standard vocabulary (and if it is linked data friendly even better). As an example we'll use the Library of Congress' [Resource Types vocabulary for Still Image](http://id.loc.gov/vocabulary/resourceTypes/img.html) to provide three terms for our select list: Drawing, Photograph, and Print. These are added as key value pairs - eg. Drawing|Drawing.
    ![Key Value Pair List of Terms](https://lh3.googleusercontent.com/ynX8GW75IPcoDdF8NFXyR-_V82T3gO-R-bIUd4tR9A4=s700 "Key Value Pair List of Terms")

  3. After you've added your terms, you can define other properties of the field like its label, help text, whether it is required or not, etc.
    ![Select Field Properties](https://lh3.googleusercontent.com/nqeEDqgE_zpA3tQM3v6KrfMVHCQoUow67rMdS2jisgg=s700 "Select Field Properties")

  4. **TO DO** Map to RDF.

  5. Move the field to an appropriate location.

#### Adding a select field using a Drupal taxonomy

To add a select field using a Drupal taxonomy you first need to [Add a Drupal Taxonomy](form-field-select-taxonomy-in-islandora-7.x-2.x.md). A taxonomy is useful when you have many terms or when you want to provide search/displays based on a controlled vocabulary. The purpose of these steps are to add a new field called DCMI Type that uses a Field Type of Term Reference and that term reference uses a taxonomy, DCMI Type, to provide a list of checkboxes for the user to select. By default Image is checked.

  1. Add a new field.
    ![Add a field that uses term reference.](https://lh3.googleusercontent.com/uvcp3RQ9JS1uRxbhiMKqkDIIgENIENZQE3IhUk_1otc=s700 "Add a field that uses term reference.")

  2. Edit field properties - add some help text, since this is the Basic Image content type select StillImage as the default value for the field. The field could potentially have multiple values (eg. Image and StillImage)
    ![Edit Field Properties](https://lh3.googleusercontent.com/7IAAiq56QREr8weAINmRZrnLKfs8bep88uFBkWy1bnA=s700 "Edit Field Properties")

  3. Select the field widget Check boxes / radio buttons.
    ![Select the field widget - checkboxes](https://lh3.googleusercontent.com/NoS_Tgpz3RuThNr6h-bavN6ZY5mrH9iNQsTInR869OQ=s700 "Select the field widget - checkboxes.")

  4. Save your field.

  5. Move it to an appropriate spot in your content type.
