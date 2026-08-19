#!/bin/bash

# silva/formator.sh - lint formae domus (regulae formandi, G1)
#
# Usage:
#   ./silva/formator.sh <via.c> [viae ...] [-machina]
# Exit:  0 conformis | 1 divergentiae | 2 usus / plagula illegibilis
#
# Regulae: project-specs/formator-regulae.md (nomina stabilia in
# fructu - divergentiae se nominant). -machina = TSV cum linea
# titulari '#' praefixa (consumptores awk '$1!~/^#/' filtrant).
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
source "$SILVA_DIR/formator_fontes_generata.sh"

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

# moduli instrumentorum communes (machina lint)
for base in silva_formator; do
    src="$SILVA_DIR/instrumenta/$base.c"
    obj="$BUILD_DIR/instr_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [instr] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

FORMATOR_SRC="$SILVA_DIR/instrumenta/principalia/formator.c"
FORMATOR_BIN="$BUILD_DIR/formator"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$FORMATOR_SRC" $obj_files \
    -o "$FORMATOR_BIN" || exit 1

cd "$RADIX_DIR"
exec "$FORMATOR_BIN" "$@"
