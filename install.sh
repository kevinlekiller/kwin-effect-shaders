#!/bin/bash

# BUILD_DIRECTORY=build RELEASE_TYPE=Release ./install.sh

[[ $EUID == 0 ]] && echo "This script must not be run as root. Exiting." && exit 1

BUILD_DIRECTORY=${BUILD_DIRECTORY:-build}
RELEASE_TYPE=${RELEASE_TYPE:-Release}
MAKEFLAGS=${MAKEFLAGS:--j$(nproc)}
SHADERSPATH="$HOME/.local/share/kwin-effect-shaders_shaders"
CONFIGPATH="$HOME/.config/kevinlekiller"
CONFIGFILE="ShadersEffect.conf"
export MAKEFLAGS=$MAKEFLAGS
CXXFLAGS=${CXXFLAGS:--march=native -mtune=native -O2 -pipe -fstack-protector-strong -fno-plt}
export CXXFLAGS=$CXXFLAGS

cd "$(realpath "$(dirname "$0")")" || exit $?

[[ -d "$BUILD_DIRECTORY" ]] && rm -rf "$BUILD_DIRECTORY"

mkdir -p "$BUILD_DIRECTORY"

cd "$BUILD_DIRECTORY" || exit $?

cmake -DCMAKE_BUILD_TYPE="$RELEASE_TYPE" .. || exit $?

make || exit $?

sudo make install

[[ $@ =~ UNINSTALL ]] && sudo make uninstall && exit

if [[ ! -d ${SHADERSPATH} ]]; then
    git clone https://github.com/kevinlekiller/kwin-effect-shaders_shaders "$SHADERSPATH" || exit $?
fi

mkdir -p "$CONFIGPATH"
if [[ ! -f $CONFIGPATH/$CONFIGFILE ]]; then
    echo -ne "[General]\nShaderPath=$SHADERSPATH\n" >> "$CONFIGPATH/$CONFIGFILE"
fi
