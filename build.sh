#!/bin/sh
# Build project from scratch

# Generate Makefiles
echo "Generation Makefiles..."
premake4 gmake || exit 1

# Build project
echo "Building project..."
make || exit 1

# End of build
echo "Build done"
