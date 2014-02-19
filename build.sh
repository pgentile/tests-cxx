#!/bin/sh
# Build project from scratch


# Raise an error
error()
{
	echo "ERROR: $*" >&2
	exit 1
}


# Number of parallel process

if [[ -z "$PARALLEL" ]]
then
    # Nombre de cores sur MacOS
    if [[ -x /usr/sbin/system_profiler ]]
    then
        PARALLEL=$(/usr/sbin/system_profiler -detailLevel full SPHardwareDataType | awk '/Total Number Of Cores/ { print $NF }')
    fi
fi

[[ -z "$PARALLEL" ]] && PARALLEL=1

# Generate Makefiles

echo "Generating Makefiles..."
premake4 gmake || error "Can't generate Makefiles"

# Build project

echo "Building project ($PARALLEL process)..."
make -j $PARALLEL $@ || error "Can't build project"

# End of build
echo "Build done"
