# Coding Standards

The Islandora module follows the [Drupal coding standards](http://drupal.org/coding-standards). This will be tested at commit time by the [coder module](http://drupal.org/project/coder), and/or via Travis when a pull request is initiated. In order to make things easier try to test your code with the coder module before creating a pull request. There are also some great pages on the [Drupal wiki](http://drupal.org/node/147789) about how to setup your favorite IDE to use the Drupal standards.

Here are a couple:  

* [Netbeans](http://drupal.org/node/1019816) 
* [VIM](http://drupal.org/node/29325)

Some quick links to various Drupal coding standards:  

* [Javascript](http://drupal.org/node/172169)
* [PHP](http://drupal.org/coding-standards which is based on http://pear.php.net/manual/en/standards.php)

Unfortunately to be consistent this also means using US spelling (e.g., "color" not "colour"). 

## PHP Version

Islandora developers should be using PHP 5.3, in order to ensure that our code will run on CentOS boxes and we follow along with the [minimum version of PHP supported by Drupal](https://www.drupal.org/requirements). Everyone is encouraged to install this older version of PHP on development boxes as there are significant differences between PHP 5.2.x and PHP 5.3.x, and even differences in the 5.2.x line. TODO: Someone should put a guide up on how to install PHP 5.2.5 on Ubuntu and OSX. 

Code that doesn't meet this guideline will not be merged. 

## Module Theming Guidelines

Much can be summed up about the benefits of coding for themers in the document [Using the theme layer (Drupal 7.x)](http://drupal.org/node/933976) or [Using the theme layer (Drupal 6.x)](http://drupal.org/node/165706) on drupal.org. 

> A well-made Drupal module allows all elements of its presentation to be overridden by the theme of the site on which it is used. In order for this theme layer to be usable, a module must be written to take advantage of it.

**Guidelines to note:**

* Strongly avoid explicitly printing HTML tags by using drupal API calls as much as possible. This will avoid unnecessary container usage, and help ensure your module's output fits with the enabled theme's look-and-feel.
* Use theme_table and theme_list functions where they make sense such as admin tables, etc. Otherwise use table/list markup in templates when there is a case for separating presentation from logic, such as search output and collection displays. Not everyone who wants to make simple markup changes to tables/lists is going to be a pro at wrangling pre-process functions, and we should not expect them to. Further reading: http://drupal.org/node/165706 (D6) & http://drupal.org/node/933976 (D7) Examples: [Forum Lists in Drupal Core](https://github.com/drupal/drupal/blob/6.x/modules/forum/forum-list.tpl.php), [Views grid layout](http://drupalcode.org/project/views.git/blob/50247e152a7823f55038e1fedb0fe711f4e82ff9:/theme/views-view-grid.tpl.php), [Island Scholar Solr Tables](https://github.com/rwincewicz/islandora_scholar_upei/blob/master/templates/scholar.tpl.php)
* When using module css files, name/structure them properly. see [CSS Cleanup](http://drupal.org/node/1089868) (this applies to core and contrib modules)

### CSS Best Practice

* Use tools such as CSSLint to check your css http://csslint.net/ (it will hurt your feelings, but help you code better)
* avoid inline css at all costs
* use lowercase letters and dashes to seperate multiple word classes
* always prefix your module class names that are specific to the module with modulename-, or module-submodulename-
* class out fieldsets & fieldnames such as .islandora-mods-description
* use the semantic approach to naming, like classes that illustrate a certain meaning of the element such as .main and .sidebar-one as opposed to presentational or structural naming methods like .right-bar .left-content
* utilize standard classes drupal uses, such as .odd, .even, .clearfix, .title, .element-invisible, .element-hidden, .tabs (see all of the system.*.css files in d7/modules/system/ for available class names)
* use multiple css selector classes in the same element, both generic and specific.. example panels/view output ``<div class="panel-pane pane-views pane-slideshow">...</div>``

_Sources:_

* [Semantics, Structure and Presentation](http://drupal.org/node/464802)
* [Class Naming - Semantic Approach](http://jonrohan.me/guide/css/semantic-css-class-naming/)
* [Accessible CSS](http://webaim.org/techniques/css/advantage)

# Documentation

This page covers the standards for documentation in Islandora code. Most of this page is taken directly from the [Drupal Doxygen and comment formatting conventions](http://drupal.org/node/1354). There have been some modifications specifically for Islandora. Where there is question refer to the Drupal standards.

There are two types of comments: in-line and headers. In-line comments  are comments that are within functions. In Islandora, documentation headers  on functions, classes, constants, etc. are specially-formatted comments  used to build the API and developer documentation.  The system for extracting documentation from the header comments uses the [Doxygen generation system](http://www.doxygen.org/), and since the documentation is extracted  directly from the sources, it is much easier to keep the documentation consistent with the source code.

There is an excellent [Doxygen manual](http://www.stack.nl/~dimitri/doxygen/manual.html) at the Doxygen site. This page describes the Islandora implementation of Doxygen, and our standards for both in-line and header comments.

## General documentation standards

These standards apply to both in-line and header comments:  

* All documentation and comments should form proper sentences and use proper grammar and punctuation.  
* Sentences should be separated by single spaces.  
* Comments and variable names should be in English, and use US English spelling (e.g., "color" not "colour").  
* All caps are used in comments only when referencing constants, for example TRUE  
* Comments should be word-wrapped if the line length would exceed 80  characters (i.e., go past the 80th column). They should be as long as  possible within the 80-character limit.  

## In-line comment standards

Non-header or in-line comments are strongly encouraged. A general  rule of thumb is that if you look at a section of code and think "Wow, I  don't want to try and describe that", you need to comment it before you  forget how it works. Comments should be on a separate line immediately  before the code line or block they reference. For example:  
```php
<?php
// Unselect all other contact categories.
db_query('UPDATE {contact} SET selected = 0');
```
If each line of a list needs a separate comment, the comments may be  given on the same line and may be formatted to a uniform indent for  readability.

C style comments `(/\* \*/)` and standard C+\+ comments `(//)` are both fine, though the former is discouraged within functions (even for multiple lines, repeat the // single-line comment). Use of Perl/shell style comments (#) is discouraged.

## General header documentation syntax

To document a block of code, such as a file, function, class, method, constant, etc., the syntax we use is:

```php
<?php
/**
 * Documentation here.
 */
```
Doxygen will parse any comments located in such a block.

### Doxygen directives - general notes
```php
<?php
/**
 * Summary here; one sentence on one line (should not, but can exceed 80 chars).
 *
 * A more detailed description goes here.
 *
 * A blank line forms a paragraph. There should be no trailing white-space
 * anywhere.
 *
 * @param $first
 *   "@param" is a Doxygen directive to describe a function parameter. Like some
 *   other directives, it takes a term/summary on the same line and a
 *   description (this text) indented by 2 spaces on the next line. All
 *   descriptive text should wrap at 80 chars, without going over.
 *   Newlines are NOT supported within directives; if a newline would be before
 *   this text, it would be appended to the general description above.
 * @param $second
 *   There should be no newline between multiple directives (of the same type).
 * @param $third
 *   (optional) TRUE if Third should be done. Defaults to FALSE.
 *   Only optional parameters are explicitly stated as such. The description
 *   should clarify the default value if omitted.
 *
 * @return *   "@return" is a different Doxygen directive to describe the return value of
 *   a function, if there is any.
 */
function mymodule_foo($first, $second, $third = FALSE) {
}
```
### Lists
```php
<?php
/**
 * @param $variables
 *   An associative array containing:
 *   - tags: An array of labels for the controls in the pager:
 *     - first: A string to use for the first pager element.
 *     - last: A string to use for the last pager element.
 *   - element: (optional) Integer to distinguish between multiple pagers on one
 *     page. Defaults to 0 (zero). *   - style: Integer for the style, one of the following constants:
 *     - PAGER_FULL: (default) Full pager.
 *     - PAGER_MINI: Mini pager.
 *   Any further description - still belonging to the same param, but not part
 *   of the list.
 *
 * This no longer belongs to the param.
 */

```
Lists can appear anywhere in Doxygen. The documentation parser requires  you to follow a strict syntax to make them appear correctly in the  parsed HTML output:

* A hyphen is used to indicate the list bullet. The hyphen is aligned  with (uses the same indentation level as) the paragraph before it, with  no newline before or after the list.
* No newlines between list items in the same list.
* Each list item starts with the key, followed by a colon, followed by  a space, followed by the key description. The key description starts  with a capital letter and ends with a period.
* If the list has no keys, start each list item with a capital letter and end with a period.
* The keys should not be put in quotes unless they contain colons (which is unlikely).
* If a list element is optional or default, indicate (optional) or (default) after the colon and before the key description.
* If a list item exceeds 80 chars, it needs to wrap, and the following  lines need to be aligned with the key (indented by 2 more spaces).
* For text after the list that needs to be in the same block, use the same alignment/indentation as the initial text.
* Again: within a Doxygen directive, or within a list, blank lines are NOT supported.
* Lists can appear within lists, and the same rules apply recursively.

### See Also sections
```php
<?php
/**
 * (rest of function/file/etc. header)
 *
 * @see foo_bar()
 * @see ajax.inc
 * @see MyModuleClass
 * @see MyClass::myMethod()
 * @see groupname
 * @see http://drupal.org/node/1354
 */
```
The `@see` directive may be used to link to (existing) functions, files,  classes, methods, constants, groups/topics, URLs, etc.  `@see` directives  should always be placed on their own line, and generally at the bottom  of the documentation header. Use the same format in `@see` that you would  for automatic links (see below).

### Automatic links

```php
<?php
/**
 * This function invokes hook_foo() on all implementing modules.
 * It calls MyClass::methodName(), which includes foo.module as
 * a side effect.
 */
```
Any function, file, constant, class, etc. in the documentation will  automatically be linked to the page where that item is documented  (assuming that item has a doxygen header). For functions and methods,  you must include () after the function/method name to get the link. For files, just put the file name in (not the path to the file).

### Code samples
```php
<?php
/**
 * Example XML output usage:
 * @code
 *   <xml>
 *     <tag> example fedora return data </tag>
 *   </xml>
 * @endcode
 * Text to immediately follow the code block.
 */
```
Code examples can be embedded in the Doxygen documentation using `@code` and `@endcode` directives.  Any code in between will be output preformatted.

### Todos
```php
<?php
/**
 * @todo Remove this in D8.
 * @todo Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam
 *   nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed
 *   diam voluptua.
 */
```

To document known issues and development tasks in code, `@todo` statements  may be used.  Each `@todo` should form an atomic task.  They should wrap  at 80 chars, if required.  Additionally, any following lines should be indented by 2 spaces, to clarify where the `@todo` starts and ends.
```php
<?php
// @todo Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam
//   nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat,
//   sed diam voluptua.
// We explicitly delete all comments.
comment_delete($cid);
```
`@todo` statements in inline comments follow the same rules as above, especially regarding indentation of subsequent comments.

## Documenting files

Each file should start with a comment describing what the file does. For example:

```php
<?php
/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */
```

The line immediately following the `@file` directive is a *summary* that will be shown in the list of all file in the generated documentation. If the line begins with a verb, that  verb should be in present tense, e.g., "Handles file uploads."  Further  description may follow after a blank line.

## Documenting functions and methods

All functions and methods, whether meant to be private or public,  should be documented. A function documentation block should immediately  precede the declaration of the function itself. For example:

```php
<?php
/**
 * Verifies the syntax of the given e-mail address.
 *
 * Empty e-mail addresses are allowed. See RFC 2822 for details.
 *
 * @param $mail
 *   A string containing an email address.
 *
 * @return
 *   TRUE if the address is in a valid format, and FALSE if it isn't.
 */
function valid_email_address($mail) {

```

After the long description, each parameter should be listed with a `@param` directive, with a description indented by two extra spaces on the  following line or lines. If there are no parameters, omit the `@param` section entirely. Do not include any blank lines in the `@param` section.

After all the parameters, a `@return` directive should be used to document the return value if there is one. There should be a blank line between the `@param` section and `@return` directive.
If there is no return value, omit the `@return` directive entirely.

Functions that are easily described in one line may use the function summary only, for example:

    /**
     * Converts an associative array to an anonymous object.
     */
    function mymodule_array2object($array) {

If the abbreviated syntax is used, the parameters and return value must be described within the one-line summary.

If the data type of a parameter or return value is not obvious or  expected to be of a special class or interface, it is recommended to  specify the data type in the `@param` or `@return` directive:

    /**
     * Executes an arbitrary query string against the active database.
     *
     * Do not use this function for INSERT, UPDATE, or DELETE queries. Those should
     * be handled via the appropriate query builder factory. Use this function for
     * SELECT queries that do not require a query builder.
     *
     * @param object $query
     *   The prepared statement query to run. Although it will accept both named and
     *   unnamed placeholders, named placeholders are strongly preferred as they are
     *   more self-documenting.
     * @param array $args
     *   An array of values to substitute into the query. If the query uses named
     *   placeholders, this is an associative array in any order. If the query uses
     *   unnamed placeholders (?), this is an indexed array and the order must match
     *   the order of placeholders in the query string.
     * @param array $options
     *   An array of options to control how the query operates.
     *
     * @return DatabaseStatementInterface
     *   A prepared statement object, already executed.
     *
     * @see DatabaseConnection::defaultOptions()
     */
    function db_query($query, array $args = array(), array $options = array()) {
      // ...
    }

Primitive data types, such as int or string, are not specified.  It is  recommended to specify classes and interfaces. If the parameter or  return value is an array, or (anonymous/generic) object, you can specify  the type if it would add clarity to the documentation. If for any  reason, a primitive data type needs to be specified, use the lower-case  data type name, e.g. "array". Also, make sure to use the most general  class/interface possible. E.g., document the return value of db_select()  to be a SelectQueryInterface, not a particular class that implements SelectQuery.

## Documenting classes and interfaces

Each class and interface should have a doxygen documentation block,  and each member variable, constant, and function/method within the class  or interface should also have its own documentation block. Example:
```php
<?php
/**
 * Represents a prepared statement.
 */
interfaceDatabaseStatementInterfaceextendsTraversable{

  /**
   * Executes a prepared statement.
   *
   * @param array $args
   *   Array of values to substitute into the query.
   * @param array $options
   *   Array of options for this query.
   *
   * @return
   *   TRUE on success, FALSE on failure.
   */
  public functionexecute($args= array(),$options= array());
}

/**
 * Represents a prepared statement.
 *
 * Default implementation of DatabaseStatementInterface.
 */
classDatabaseStatementBaseextendsPDOStatementimplementsDatabaseStatementInterface{

  /**
   * The database connection object for this statement DatabaseConnection.
   *
   * @var DatabaseConnection
   */
  public$dbh;

  /**
   * Constructs a DatabaseStatementBase object.
   *
   * @param DatabaseConnection $dbh
   *   Database connection object.
   */
  protected function__construct($dbh) {
   // Function body here.
  }

  /**
   * Implements DatabaseStatementInterface::execute().
   *
   * Optional explanation of the specifics of this implementation goes here.
   */
  public functionexecute($args= array(),$options= array()) {
    // Function body here.
  }

  /**
   * Returns the foo information.
   *
   * @return object
   *   The foo information as an object.
   *
   * @throws MyFooUndefinedException
   */
  public functiongetFoo() {
    // Function body here.
  }

  /**
   * Overrides PDOStatement::fetchAssoc().
   *
   * Optional explanation of the specifics of this override goes here.
   */
  public functionfetchAssoc() {
    // Call PDOStatement::fetch to fetch the row.
    return$this->fetch(PDO::FETCH_ASSOC);
  }
}
```

Notes:

* Leave a blank line between class declaration and first docblock.
* Use a 3rd person verb to begin the description of a class, interface, or method (e.g. Represents not Represent).
* For a member variable, use `@var` to tell what data type the variable is.
* Use `@throws` if your method can throw an exception, followed by the  name of the exception class. If the exception class is not specific  enough to explain why the exception will be thrown, you should probably  define a new exception class.
* Make sure when documenting function and method return values, as  well as member variable types, to use the most general class/interface  possible. E.g., document the return value of db_select() to be a  SelectQueryInterface, not a particular class that implements  SelectQuery.

## Documenting Islandora interaction with Fedora

Whenever we interact with Fedora in the comments using either:

* `@throws`
* `@return`

we need to document what is returned on error and what state the Fedora is left in, so that the error can be reported through Drupal instead of failing silently and whoever is calling the API can make a choice about what to do next.

# Generating Islandora documentation with Doxygen

In order to generate the documentation you need the doxygen package installed.

Download this [file|^Doxyfile] to your modules/fedora_repository folder and run the command:

     doxygen Doxyfile

after the command finishes running the documentation can be found in `documentation/html/index.html`

Within the Doxyfile if you change the line:

    EXTRACT_ALL            = YES

to

    EXTRACT_ALL            = NO

Then doxygen will only extract information from documented files, instead of trying to extract information from all files. 

# Islandora Module File Structure and Naming

tl;dr: Filenames should contain lowercase characters and PHP files (except templates) should use underscores to separate words in the filename.

## Why define a naming convention, specifically for Islandora Drupal development?

Consistent file naming augments the ability of developers to quickly identify the location of code within modules. The Drupal community has not documented a set of conventions related to naming files although a pattern of file naming can be observed by reviewing major contributed modules.

## Modules Conventions


Modules meant for public distribution are generally expected to include a README.txt and LICENCE.txt file. README.txt files should follow the Drupal README.txt conventions found here (https://drupal.org/node/2181737). Due to our heavy use of github we will be using README.md files to take advantage of their markdown parsing on repository landing pages.

## Conventions for PHP files

PHP code will generally be placed in (.module, .install, .test, .inc, .php) files. Unless otherwise specified filenames should contain only lowercase characters and should use underscores to separate words in the filename.
<pre>
Good: islandora_something.inc   Bad: IslandoraSomething.inc
</pre>

### Module Files (*.module)

Hook implementations and functions providing API-like interfaces or extensively used utilities should be placed in the .module file of a module. Typically it is expected that if this module is part of the islandora ecosystem (meant for wide distribution and to be branded as part of islandora) the modules name will be prefixed with islandora.

### Include Files (*.inc)

Include files (*.inc) should contain all other PHP code (non-template or install files) and should be placed in a subdirectory of the module named includes. The module name should not be used as a prefix in the filename; it is redundant. These rules apply to all .inc files, including files containing classes. To further aid file discovery, include files may contain a file-type extension helper such as .form or .pages if the file contains menu callbacks grouped together (example: admin.form.inc).
<pre>
Good: includes/mime_detect.inc    Bad: includes/islandora_mime_detect.inc

Good: includes/upload_form.inc    Better: includes/upload.form.inc
</pre>

### Template Files (*.tpl.php)

Template files should have an extension of .tpl.php and should be placed in the theme subdirectory of the module. Template filenames should include the module name with words separated by a hyphen/dash (example: islandora-object.tpl.php).

The theme subdirectory can also contain files like *.theme.inc, where any theme related functions can be implemented (preprocess theme hooks, etc). Functions included in *.theme.inc files should only be used by the theme system, if a function is used else where it should be moved to a different file.
<pre>
Good: theme/module_name.theme.inc    Bad: includes/theme.inc

Good: theme/islandora-object.tpl.php        Bad: theme/islandora_object.tpl.php
</pre>

### Meaningful Locations of PHP/Module Files

The following table summarizes where files are expected to be.
<pre>
Subdirectory      Files

/                 *.module, *.info, *.api.php, *.install

/includes         *.inc

/theme            *.tpl.php, *.theme.inc

/tests            *.test, (Any related test files, regardless of extension)
</pre>

## Non PHP-Files

General purpose subdirectories may be used for related files of a similar mime type or purpose, such as images (example: *.png or *.tiff files in ../images). Unless otherwise specified filenames should contain only lowercase characters and should use underscores to separate words in the filename.
<pre>
Subdirectory    Extensions

/css            *.css

/js             *.js

/images         *.jpg, *.tiff, *.png, etc
</pre>

### CSS files (*.css)

Modules should always prefix the names of their CSS files with the module name; for example, system-menus.css rather than simply menus.css. Themes can override module-supplied CSS files based on their filenames, and this prefixing helps prevent confusing name collisions for theme developers. See drupal_get_css() where the overrides are performed. Also, if the direction of the current language is right-to-left (Hebrew, Arabic, etc.), the function will also look for an RTL CSS file and append it to the list. The name of this file should have an '-rtl.css' suffix. For example, a CSS file called 'mymodule-name.css' will have a 'mymodule-name-rtl.css' file added to the list, if exists in the same directory. This CSS file should contain overrides for properties which should be reversed or otherwise different in a right-to-left display. CSS files

can use dashes to separate words as well as underscores.
<pre>
Good: css/islandora_admin.css    Bad: css/IslandoraAdmin.css

Good: css/islandora-admin.css    Better: css/islandora.admin.css
</pre>

## References

Please refer to the [Module documentation guidelines](http://drupal.org/node/161085) For more information on conventions.
