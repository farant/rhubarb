#!/bin/bash

# officina/fusor.sh - Instrumentum fusionis corporis compilare et
# currere (percursus.sh exemplar)
#
# Usage:
#   ./officina/fusor.sh [radix] [-omnia] [-v]     # ex radice repositorii

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$OFF_DIR/fontes"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$OFF_DIR/fusor_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$OFF_DIR/fontes" \
        "$RADIX_DIR/silva/amalgama" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
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

src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for src in "$OFF_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [officina] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# Semper renectere (obiecta mutata aliter in binario vetusto latent)
FUSOR_SRC="$OFF_DIR/instrumenta/principalia/fusor.c"
FUSOR_BIN="$BUILD_DIR/fusor"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$FUSOR_SRC" $obj_files \
    -o "$FUSOR_BIN" || exit 1

exec "$FUSOR_BIN" "$@"
