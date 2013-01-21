<?php

/**
 * @file
 * Handles incoming requests to fire off regularly-scheduled tasks (cron jobs).
 */

include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
db_query("DELETE FROM {cache_form} where expire < UNIX_TIMESTAMP()");
drupal_cron_run();
