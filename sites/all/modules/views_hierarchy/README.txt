$Id: README.txt,v 1.1 2010/06/04 11:13:26 joachim Exp $

The Views Hierarchy module provides a Views style plugin which outputs the result
as a hierarchy of items. 

This is similar to the normal list style plugin, except items are put into 
nested lists according to their parentage.

So supposing your view was listing taxonomy terms, you could get:

  - cake
    - chocolate cake
      - sachertorte
      - chocolate fudge cake
    - tart
      - strawberry tart
      - tarte tatin
  ... and so on.

To set this up, you need to have a field in your view that indicates the parent 
item. In other words, if 'cake' has tid 47, the 'chocolate cake' row needs a field
whose value will be that 47.

You need to add this field yourself, and then select it in the options for the
style plugin. You may exclude this field from output if you do not want it to
be visible.

Examples
--------

Taxonomy terms:

1. Add the 'Parent term' relationship
2. Add the 'Taxonomy: Term ID' field on this relationship and select this as your
   parent field.

Nodes (untested):

1. Add a nodereference field and select this as your parent field.

Notes
-----

You should not use a pager with this style, as there is no guarantee that the 
parent or all the children of any given item will be on the same page.
