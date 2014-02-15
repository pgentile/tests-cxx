#!/bin/bash

UNCRUSTIFY='uncrustify --replace --no-backup -c uncrustify.cfg'

find ./include ./src -type f \( -name '*.[ch]' -o -name '*.[ch]pp' \) | while read f
do
  $UNCRUSTIFY $f
done
