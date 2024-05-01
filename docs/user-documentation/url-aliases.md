# URL Aliases

A URL alias is an alternate URL pattern that resolves to a Drupal entity,
such as a node, media, taxonomy term, or user. For example, you could set
"/welcome" as an alias for "/node/1". Aliases are part of Drupal Core and
can be extended by contrib modules. One such module is Pathauto, which 
enables automatic alias generation based on patterns, and the patterns may
involve "Tokens" (such as [node:title]). 

This page will attempt to cover the Islandora Starter Site's use of aliases
and what we consider to be best practices. A full description of creating 
and managing aliases is out of scope.


## Best practices with URL aliases

While every site may choose to set up their aliases differently, we cannot
prescibe a universal setup.

A common "nice-to-have" is the presence of the slug `/islandora/` in the
URL which identifies the content as "Islandora". 

A potential "best practice" is that if your site uses persistent identifiers
such as DOIs or Handles, that those identifiers make up part of the URL alias.


## Use of URL aliases in Islandora Starter Site

The Islandora Starter Site includes the Pathauto module, which we consider
that most sites will want to use in some way. However its default configuration
should not be interpreted as prescriptive. You are encouraged to use persistent
identifiers if you have them!

The default Pathauto pattern for Repository Items is `/islandora/[node:title]`
with the pathauto configuration trimming the alias at 100 characters.
 
