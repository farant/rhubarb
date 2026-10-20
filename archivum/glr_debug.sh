#!/bin/bash
#
# glr_debug.sh - Quick GLR parser debugging
#
# Recompiles and runs the debug tool.
#
# Usage:
#   ./glr_debug.sh "int x;"           # declaration (default)
#   ./glr_debug.sh -e "a + b * c"     # expression mode
#

if [ $# -lt 1 ]; then
    echo "Usage: ./glr_debug.sh [options] <input>"
    echo "Options:"
    echo "  -e, --expr    Parse as expression only"
    echo ""
    echo "Examples:"
    echo "  ./glr_debug.sh \"int x;\"         # declaration"
    echo "  ./glr_debug.sh -e \"a + b * c\"   # expression"
    exit 1
fi

# Compiler flags
CFLAGS="-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -fbracket-depth=512"

# Source files needed for GLR parsing
SOURCES=(
    "lib/piscina.c"
    "lib/chorda.c"
    "lib/chorda_aedificator.c"
    "lib/friatio.c"
    "lib/internamentum.c"
    "lib/xar.c"
    "lib/tabula_dispersa.c"
    "lib/arbor2_lexema.c"
    "lib/arbor2_token.c"
    "lib/arbor2_expandere.c"
    "lib/arbor2_glr.c"
    "lib/arbor2_glr_tabula.c"
    "tools/glr_debug.c"
)

mkdir -p bin

echo "Compiling glr_debug..."
if ! clang $CFLAGS -Iinclude -o bin/glr_debug "${SOURCES[@]}" 2>&1; then
    echo "Compile failed!"
    exit 1
fi

echo ""
./bin/glr_debug "$@"
