#!/bin/bash

# silva/ambigua.sh - census ambiguitatis retentae trans corpus
#
# Usage:
#   ./silva/ambigua.sh <via.c> [viae ...] [-machina] [-loca] [-pinna <via.tsv>]
# Exit:  0 mensuratum | 1 regressus contra pinnam | 2 usus / plagula illegibilis
#
# Census ambiguorum RETENTORUM per plagulam (columnae: via,
# ambigua in plagula ipsa, aliena in contento incluso, errores).
# -loca = positiones singulorum; -pinna = porta regressus.
# -machina = TSV cum linea titulari '#' praefixa.
#
# NB: semper ex radice repositorii currit (cd infra) - viae datae
# relativae ad radicem sunto.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/ambigua_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" "$SILVA_DIR/instrumenta" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

AMBIGUA_SRC="$SILVA_DIR/instrumenta/principalia/ambigua.c"
AMBIGUA_BIN="$BUILD_DIR/ambigua"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$AMBIGUA_SRC" $obj_files \
    -o "$AMBIGUA_BIN" || exit 1

cd "$RADIX_DIR"
exec "$AMBIGUA_BIN" "$@"
