#!/bin/bash

# tessera/spectaculum.sh - Demonstratio interactiva (terminal verum!)
#
# Usage: ./tessera/spectaculum.sh
# Compilat et currit spectaculum - vectis manualis Phase B (colores,
# ornamenta, quadra, claves, mus, magnitudo, Ctrl-Z).

set -u

TESSERA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$TESSERA_DIR/.." && pwd)"
BUILD_DIR="$TESSERA_DIR/build"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$TESSERA_DIR/fontes"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$TESSERA_DIR/spectaculum_fontes_generata.sh"
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
for src in "$TESSERA_DIR"/fontes/*.c; do
    f="$(basename "$src" .c)"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [tessera] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

echo "  [tessera] spectaculum.c"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$TESSERA_DIR/instrumenta/principalia/spectaculum.c" $obj_files \
    -o "$BUILD_DIR/spectaculum" || exit 1

exec "$BUILD_DIR/spectaculum"
