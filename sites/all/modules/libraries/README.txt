-- SUMMARY --

Libraries API provides external library handling for Drupal modules.

Usage


In your module use libraries_get_path($name) to get the location of the library files. $name is the unique name of the library you chose in (1.). A common pattern is:

if ($path = libraries_get_path($name)) {
  // Do something with the library, knowing the path, for instance:
  // drupal_add_js($path . '/example.js');
}

Note that with this example $path will have a value even if the library in question has not yet been downloaded. It will even have a value if the /sites/all/libraries folder does not yet exist. 