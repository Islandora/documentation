This folder holds xslt files that can be used to transform the
display of a collection.  These files are not used by the module from
this location but should be added as datastreams to objects that have a
content model of Collection.

The datastream id should be COLLECTION_VIEW

NOTE: If you add a invalid xslt to a as a collection view you will
no longer have access to that object or collection.  You may have to
fire up the fedora-admin utility and move or modify the Collection_View datastream.  This
is a bug but not sure when it will be fixed.