#!/bin/sh
# Build project from scratch

# Concurrent build threads
[[ -z "$CONCURRENT_BUILD_THREADS" ]] && CONCURRENT_BUILD_THREADS=2

# Raise an error
error()
{
	print "ERROR: $*" >&2
	exit 1
}

indent()
{
	typeset line
	while read line
	do
		echo "   $line"
	done
}

# Generate Makefiles
echo "Generating Makefiles..."
premake4 gmake | indent || error "Can't generate Makefiles"

# Build project
echo "Building project..."
make -j $CONCURRENT_BUILD_THREADS | indent || error "Can't build project"

# End of build
echo "Build done"
