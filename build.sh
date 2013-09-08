#!/bin/sh
# Build project from scratch

# Concurrent build threads
[[ -z "$CONCURRENT_BUILD_THREADS" ]] && CONCURRENT_BUILD_THREADS=1

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
env CXXFLAGS=-std=c++11 make -j $CONCURRENT_BUILD_THREADS -k verbose=1 $@ || error "Can't build project"

# End of build
echo "Build done"
