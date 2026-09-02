#!/bin/bash

# silva/censor.sh - censor latinae (instrumenta prima #1)
#
# Usage:
#   ./silva/censor.sh <plagula.c> [plagula2 ...]   plagulas datas censere
#   ./silva/censor.sh -corpus [radix]              repositorium totum
# Flags: -omnia (tectum mensurae sublatum), -v (verbosa)
# Exit:  0 purus | 1 flagrationes | 2 sine arbore / usus
#
# NB: semper ex radice repositorii currit (cd infra) - viae datae
# relativae ad radicem sunto.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/censor_fontes_generata.sh"

# Obiecta vetusta capitibus novis = valores enum falsi (exemplar
# percursus.sh; obiecta communia in silva/build/ conpartita)
newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
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

CENSOR_SRC="$SILVA_DIR/instrumenta/principalia/censor.c"
CENSOR_BIN="$BUILD_DIR/censor"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$CENSOR_SRC" $obj_files \
    -o "$CENSOR_BIN" || exit 1

cd "$RADIX_DIR"
exec "$CENSOR_BIN" "$@"
