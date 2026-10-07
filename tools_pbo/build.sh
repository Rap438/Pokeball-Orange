#!/bin/sh
# Build the headless test runner against a libmgba build.
# usage: MGBA=/path/to/mgba tools_pbo/build.sh   (MGBA is an mGBA source tree with a cmake build in build/)
# The runner must see the same feature defines libmgba was built with, or struct mCore's layout differs.
set -e
MGBA=${MGBA:-$HOME/mgba}
D=$(sed -n 's/^C_DEFINES = //p' "$MGBA/build/CMakeFiles/mgba.dir/flags.make" | sed 's/-Dmgba_EXPORTS//; s/-DMGBA_DLL//')
cd "$(dirname "$0")"
gcc -O2 -std=c11 $D -o runner runner.c -I"$MGBA/include" -I"$MGBA/build/include" -L"$MGBA/build" -lmgba -Wl,-rpath,"$MGBA/build"
