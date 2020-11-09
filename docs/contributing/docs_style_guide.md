# Documentation Style Guide
_last updated on 09-11-2020_

## Do's

- Use a GitHub Pull Request to submit documentation.
- Add or update a line at the top of the page, just under the header, with the format `_last updated on DD-MM-YYYY_` when you add a page or make changes.
- Make it clear if the documentation is based on a particular configuration (such as Islandora Defaults) or if it applies to any deployment of Islandora.
- Submit documentation formatted in [Markdown](https://en.wikipedia.org/wiki/Markdown) format.
    - Include a top-level heading for the whole page (using `#`)
    - Please add Markdown headings (`#` and `##`) to the content sections.

- Use the "bold/emphasis" style in Markdown by enclosing text in double asterisks or underscores, `**bold text**` or `__bold text__`, for UI elements that users will interact with. For example, a button label for a button that must be pressed should be made bold in Markdown.
- Use the "italics" style in Markdown by enclosing text in single asterisks or underscores, `*italic text*` or `_italic text_`, for UI elements that have a label or title if you need to reference them in the documentation. For example, a title of a screen or page that will visit should be made italic in Markdown.
- Use `>>` and `**bold text**` to indicate clicking through nested menu items, and also include the direct path. _Example:_
```
**Administration** >> **Structure** >> **Views** (/admin/structure/views)
```
- Use `-` instead of `*` for bulleted lists. Indent four (4) spaces for nested lists (Github renders nesting in markdown with 2 spaces, but mkdocs needs 4).
_Example:_
```
- I am a list item
    - And I am a sub-item.
```
- Upload images to the 'assets' folder and reference them from there. 
    - For file naming, use underscores between words and prefix all file names with the page name, e.g. context_display_hints.jpg for the image showing how to set display hints in the context menu.
- Use the [Admonition syntax](https://squidfunk.github.io/mkdocs-material/reference/admonitions/) to create notes like this (four-space indent required):

_Example:_

```
!!! note "Helpful Tip" 
    I am a helpful tip!
```

_Result:_

!!! note "Helpful Tip" 
    I am a helpful tip!
    
- Use the [Callout syntax](https://rdmd.readme.io/docs/callouts) with a lobster emoji 🦞 to call attention to areas where Islandora configuration differs from standard Drupal configuration:

_Example:_

```
> 🦞 Islandora
> 
> This setting is specific to Islandora and is not standard to Drupal.
```

_Result:_

> 🦞 Islandora
> 
> This setting is specific to Islandora and is not standard to Drupal.

## Don'ts

- Do not leave any "trailing spaces" at the end of lines of content.
- Do not use "curly" quotes and apostrophes, use only "straight" quotes and apostrophes.
- Do not upload images that are excessively large in file size (remember, these docs are part of the software!)
