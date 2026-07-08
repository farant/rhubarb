#!/bin/bash

# silva/generare.sh - Tabulas generare et coquere (dev-time)
#
# Usage:
#   ./generare.sh                 # OMNES grammaticae commissae
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
    # silva_amalgama (mechanismus amalgamatoris) fontes postulat -
    # generator eo non utitur; amalgamare.sh id compilat
    if [ "$base" = "silva_amalgama" ]; then continue; fi
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

if [ $# -ge 1 ]; then
    "$BUILD_DIR/generator" "${1}" "${2:?PRAEFIXUM deest}" "${3:?BASIS deest}"
else
    # Sine argumentis: OMNES grammaticae commissae regenerantur
    # (post QUAMLIBET mutationem grammaticae - silva/CLAUDE.md)
    "$BUILD_DIR/generator" grammatica/sceletum.stml \
        SILVA_SCELETUM fontes/silva_tabulae_sceleti || exit 1
    "$BUILD_DIR/generator" grammatica/sceletum_imparilis.stml \
        SILVA_IMPARILIS fontes/silva_tabulae_imparilis || exit 1
    "$BUILD_DIR/generator" grammatica/c89.stml \
        SILVA_C89 fontes/silva_tabulae_c89 \
        amalgama/silva.h instrumenta/principalia/hospes.c || exit 1
fi
