#!/bin/bash

BUILD_DIRECTORY=build
RELEASE_TYPE=Release

cd "$(realpath "$(dirname "$0")")" || exit $?

[[ -d "$BUILD_DIRECTORY" ]] && rm -rf "$BUILD_DIRECTORY"

mkdir -p "$BUILD_DIRECTORY"

cd "$BUILD_DIRECTORY" || exit $?

cmake .. -DCMAKE_BUILD_TYPE="$RELEASE_TYPE" || exit $?

make || exit $?

sudo make install

[[ $1 == UNINSTALL ]] && sudo make uninstall
