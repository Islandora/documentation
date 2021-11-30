## Overview

This tutorial will walk you through modifying the homepage. Installing the Islandora module will set the homepage to use the _welcome.html.twig_ file and the default alias for this is `/welcome`. This will copy the original files into the current default theme's template directory making it easier to modify.

### To modify the page
Please note that if the default sandbox is used the slide show is a block and is not managed by the TWIG file. See blocks for more information.

Within the default theme should be 2 files
```text
welcome_base.html.twig
welcome.html.twig
```

welcome_base.html.twig sets a header, the title and some block examples but by default does nothing critical and came be overridden or removed entirely.

welcome.html.twig this is the twig file that represents the custom page content. This is the easiest way to modify the homepage. 

### Overide Welcome page
If you no longer want `welcome.html.twig` to be your homepage just set the system's default **FRONT PAGE: Default front page** value to what ever node/page you want.
This setting found at `/admin/config/system/site-information`

### Original Files
var/www/drupal/web/modules/contrib/islandora/templates/welcome_base.html.twig
var/www/drupal/web/modules/contrib/islandora/templates/welcome.html.twig
