#!/bin/bash

# silva/generare.sh - Tabulas generare et coquere (dev-time)
#
# Usage:
#   ./generare.sh                 # sceletum (default)
#   ./generare.sh <grammatica.stml> <PRAEFIXUM> <basis_exitus>
#
# Compiles the generator (instrumenta + principalia/generator.c) and
# runs it. Output .{h,c} land in fontes/ and are COMMITTED (they are
# part of the eventual amalgamated silva.c).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

# Viae relativae ad silva/ - commentum GENERATUM viam stabilem fert
cd "$SILVA_DIR"
GRAMMATICA="${1:-grammatica/sceletum.stml}"
PRAEFIXUM="${2:-SILVA_SCELETUM}"
BASIS="${3:-fontes/silva_tabulae_sceleti}"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
)
declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio" "internamentum" "selectio" "stml"
)

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/instrumenta/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [silva] $base.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

echo "  [silva] generator.c"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SILVA_DIR/instrumenta/principalia/generator.c" $obj_files \
    -o "$BUILD_DIR/generator" || exit 1

"$BUILD_DIR/generator" "$GRAMMATICA" "$PRAEFIXUM" "$BASIS"
