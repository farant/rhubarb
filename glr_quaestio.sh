#!/bin/bash
#
# glr_quaestio.sh - GLR Table Query Tool
#
# Recompiles and runs the query tool.
#
# Usage:
#   ./glr_quaestio.sh stats
#   ./glr_quaestio.sh state 4
#   ./glr_quaestio.sh clone PLUS SPACESHIP
#

# Compiler flags
CFLAGS="-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -fbracket-depth=512"

# Source files needed for glr_quaestio
SOURCES=(
    "lib/piscina.c"
    "lib/chorda.c"
    "lib/chorda_aedificator.c"
    "lib/friatio.c"
    "lib/internamentum.c"
    "lib/xar.c"
    "lib/tabula_dispersa.c"
    "lib/arbor.c"
    "lib/arbor_lexema.c"
    "lib/arbor_praeparator.c"
    "lib/arbor_syntaxis.c"
    "lib/filum.c"
    "lib/via.c"
    "tools/glr_quaestio.c"
)

mkdir -p bin

echo "Compiling glr_quaestio..."
if ! clang $CFLAGS -Iinclude -o bin/glr_quaestio "${SOURCES[@]}" 2>&1; then
    echo "Compile failed!"
    exit 1
fi

echo ""
./bin/glr_quaestio "$@"
