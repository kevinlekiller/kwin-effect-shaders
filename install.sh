#!/bin/bash

# BUILD_DIRECTORY=build RELEASE_TYPE=Release ./install.sh

[[ $EUID == 0 ]] && echo "This script must not be run as root. Exiting." && exit 1

BUILD_DIRECTORY=${BUILD_DIRECTORY:-build}
RELEASE_TYPE=${RELEASE_TYPE:-Release}
MAKEFLAGS=${MAKEFLAGS:--j$(nproc)}
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

[[ ! -d ~/.local/share/shaders ]] && cp -r /usr/share/kwin/shaders ~/.local/share/

if ! grep "^\[Effect-Shaders\]" ~/.config/kwinrc &> /dev/null; then
    echo -ne "\n[Effect-Shaders]\nShaderPath=$HOME/.local/share/shaders\n" >> ~/.config/kwinrc
fi

[[ $@ =~ UNINSTALL ]] && sudo make uninstall
