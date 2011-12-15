<?php

// Print out header row, if option was selected.
if ($options['header']) {
$csv_output = chr(239) . chr(187) . chr(191);
print $csv_output;
  print implode($separator, $header) . "\r\n";
}
