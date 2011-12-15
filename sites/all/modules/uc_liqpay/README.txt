$Id$

Drupal module for payment through the system of Liqpay for  ubercart:
----------------------

Author - Drozdik Dmitry dmitriydrozdik@gmail.com
Requires - Drupal 6
License - GPL (see LICENSE)


Overview:
--------
This module provides Liqpay payment gateway through liqpay.com website,
when using Drupal Ubercart.

Features:
 - quick installation 
 - supports API & hooks to interact with other modules 
 - adjustable list of currencies 
 - MySQL and PostgreSQL support 
 - all payments are stored in stand-alone table 

Installation:
------------
1. Place this module directory in your modules folder (this will
   usually be "sites/all/modules/").
2. Go to "administer" -> "modules" and enable the module.
3. Check the "Administer" -> "Store administration" -> 
   "Configuration" -> "Liqpay" page to add settings for this 
   payment method.
4. Add on chopping cart products
5. Visit http://example.site/cart/checkout.