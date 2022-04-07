#!/bin/bash
cat > /dev/null <<LICENSE
    Copyright (C) 2022  kevinlekiller

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
    https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
LICENSE

# BUILD_DIRECTORY=build RELEASE_TYPE=Release ./install.sh

[[ $EUID == 0 ]] && echo "This script must not be run as root. Exiting." && exit 1

BUILD_DIRECTORY=${BUILD_DIRECTORY:-build}
RELEASE_TYPE=${RELEASE_TYPE:-Release}
MAKEFLAGS=${MAKEFLAGS:--j$(nproc)}
SHADERSPATH="$HOME/.local/share/kwin-effect-shaders_shaders"
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

