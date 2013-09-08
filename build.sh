#!/bin/sh
# Build project from scratch

# Concurrent build threads
[[ -z "$CONCURRENT_BUILD_THREADS" ]] && CONCURRENT_BUILD_THREADS=2

# Raise an error
error()
{
	echo "ERROR: $*" >&2
	exit 1
}

# Generate Makefiles
echo "Generating Makefiles..."
premake4 gmake || error "Can't generate Makefiles"

# Build project
echo "Building project..."
make -j $CONCURRENT_BUILD_THREADS $@ || error "Can't build project"

# End of build
echo "Build done"
