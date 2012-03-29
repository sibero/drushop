uc_extra_fields_pane module
------------------------
by blackice78, MegaChriz and panthar

This modules allows an administrator to define additional (billing and 
shipping) address fields (i.e. VAT) as well as custom order fields in 
Ubercart e-commerce suite. These fields will be available during checkout 
process and in order handling pages.

With this module an administrator has a flexible way to define one or
more address fields at
/admin/store/settings/checkout/edit/fields
and custom order fields at
/admin/store/settings/checkout/edit/extrafields

These fields will appear to customers during the checkout process and
will be saved at the end of this process on a separate database table for
later use.
The additional address fields will appear in the original delivery 
and billing pane, provided by Ubercart. Custom order fields will appear
in it's own checkout pane.

DEPENDENCIES
------------
This module requires Ubercart 2.3 or higher.

INSTALLATION
------------
  * Copy the module's directory to your modules directory
    and activate it.
  * Go to /admin/store/settings/checkout/edit/fields
    for defining extra address fields
    or to /admin/store/settings/checkout/edit/extrafields
    for defining custom order fields.

The module will add two tables: uc_extra_fields and uc_extra_fields_values.

ABOUT THE FIELD TYPES
------------
  * Textfield
    This field type adds a simple text field to the form. This field can be
    pre-filled with a default value.
  * Select list
    This field type adds a selection field to the form (users can select a value
    from a dropdown menu). In the value section you can define the available
    options in the format "safe_key|readable part". The *safe_key* part is the
    part that will be saved in the database. The *readable* part is what will be
    presented to the user. IMPORTANT NOTE: if you want to make this field
    required, make sure that the first option of the list has an *empty* safe key.
    You can insert an empty safe key by typing a space.
    Example:
     |Please select
    option1|Option 1
    option2|Option 2
  * Checkbox
    This field type adds a checkbox to the form. Note: setting this field to
    required has no effect, because of a core bug in Drupal. The module "Checkbox 
    Validate" corrects this bug.
  * Constant
    This field type adds a value to the form which can not be changed
    by the customer. It is just displayed as plain text. However, admins who
    can change the Ubercart order are able to adjust the value of this field, 
    because then it's displayed as a text field.
  * PHP string
    This field type is similar to the constant field type. The difference is that
    the shown value can be defined with PHP code, which means you could get this
    value from everywhere. In the value section you should return a string, for
    example:
    <?php return "A string"; ?>
  * PHP select list
    This field type is similar to the select list field type. The difference is that
    you can build the option list with PHP. Be sure to return an array with 'key' => 'value'.
    IMPORTANT NOTE: if you want to make this field required, make sure that the first
    option has an *empty* key. This may be a space, but it can also be an empty string.
    Example:
    <?php
      return array(
        '' => 'Please select',
        'option1' => 'Option 1',
        'option2' => 'Option 2',
      );
    ?>

UBERCART ADDRESSES INTEGRATION
------------
Ubercart Addresses integration is completed! It currently integrates full
with Ubercart Addresses 6.x-2.x-dev from November 25, 2011. As the code of
Ubercart Addresses is not stable yet, this version of Extra Fields Pane
could be incompatible with future versions of Ubercart Addresses 6.x-2.x.

This version of Extra Fields Pane contains also integration code for
Ubercart Addresses 6.x-1.x, but this is not a full integration.

VIEWS INTEGRATION
------------
If you want to use this module in combination with Views, you will need
to install the Ubercart Views module:
http://drupal.org/project/uc_views
For order related views, activate the module 'Ubercart Views'.
For Ubercart Addresses related views, activate the module 'uc_views_addresses'.

MAINTAINERS
------------
blackice78
MegaChriz
panthar
