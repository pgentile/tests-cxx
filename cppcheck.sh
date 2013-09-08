#!/bin/bash

defines_gcc=$(gcc -dM -E - < /dev/null | awk '{ print "-D" $2 "=" $3}')
defines="$defines_gcc"

includes_gcc=$(g++ -std=c++11 -E -x c++ - -v < /dev/null 2>&1 | sed -n -e '/^#include .* search starts here/,/^End of search list/p' | sed -e '/^#include .* search starts here/d' -e '/^End of search list/d' -e 's/^ *//' | awk '{ print "-I " $0 }')
includes="-I ./include -I /opt/local/include $includes_gcc"

cppcheck --check-config --std=c++11 --platform=unix64 --enable=all $defines $includes ./src ./include
