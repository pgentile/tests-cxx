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

# Generate Makefiles
echo "[1/3] Generating Makefiles..."
premake4 gmake || error "Can't generate Makefiles"

# Build project
echo "[2/3] Building project..."
make -j $CONCURRENT_BUILD_THREADS || error "Can't build project"

# Build project, debug mode
echo "[3/3] Building debug project"
make -j $CONCURRENT_BUILD_THREADS config=debug || error "Can't build project in debug mode"

# End of build
echo "Build done"
