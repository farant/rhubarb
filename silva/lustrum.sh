#!/bin/bash

# silva/lustrum.sh - Census corporis solarii (dev-time, Phase 7 Chunk C)
#
# Usage:
#   ./lustrum.sh [via_solarii]     # defalta: ../solarium iuxta rhubarb
#
# Compilat lustrum (instrumenta/principalia/) contra fontes + libs,
# deinde currit. Exitus 0 = omnes plagulae fideles.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
SOLARIUM_DIR="${1:-$RADIX_DIR/../solarium}"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/lustrum_fontes_generata.sh"
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
for src in "$SILVA_DIR"/fontes/*.c; do
    f="$(basename "$src" .c)"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [silva] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

echo "  [silva] lustrum.c"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SILVA_DIR/instrumenta/principalia/lustrum.c" $obj_files \
    -o "$BUILD_DIR/lustrum" || exit 1

exec "$BUILD_DIR/lustrum" "$SOLARIUM_DIR"
