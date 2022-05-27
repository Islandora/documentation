# Accessibility

Accessibility is the ablity of a site to be used fully by all users, including those using screen reader technologies or keyboard navigation. 

## Drupal Documentation

Islandora's accessibility features are currently provided by Drupal and contributed Drupal modules. These pages within the Accessibility section of the Drupal documentation can provide guidance:

- [Drupal Accessibility Features](https://www.drupal.org/docs/accessibility/drupal-accessibility-features) (included in core Drupal)
- [Contributed Modules for Extending Accessibility in Drupal](https://www.drupal.org/docs/accessibility/contributed-modules-for-extending-accessibility-in-drupal)
- [Hiding Content Properly (for all users including users with screen readers)](https://www.drupal.org/docs/accessibility/hide-content-properly)
- [How to do an accessibility review?](https://www.drupal.org/docs/accessibility/how-to-do-an-accessibility-review)
- List of [External Accessibility Resources](https://www.drupal.org/docs/accessibility/external-accessibility-resources)

## Accessible Themes

Much of the accessibility of a website is dependent on how specific HTML tags and 
attributes are used, thus falls largely into the realm of Drupal Themes. The 
[Olivero](https://www.drupal.org/docs/core-modules-and-themes/core-themes/olivero) theme
and the [Claro](https://www.drupal.org/docs/core-modules-and-themes/core-themes/claro-theme)
 admin theme were designed by the Drupal community with accessibility 
as a guiding principle. 

## Automatic alt-text

When creating image media, alt-text is a required attribute. An Islandora function 
currently automatically populates the alt-text, if that media is the "media of" a node,
with the respective node's title.

This is not considered good accessibility as the node's title will likely be already
on the page, and it doesn't add anything to describing the image itself. Please fill 
out the alt text with something meaningful when adding image content to Islandora.

