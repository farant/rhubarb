#!/bin/bash

# tessera/effigies.sh - Spectator imaginum (quadrans Q5) (terminal verum!)
#
# Usage: ./tessera/effigies.sh [imago ...]
# Sine argumentis: imagines probationis (probationes/fixa/quadrans/*.jpg,
# photographiae Frani). Pars pura (aptatio, pictura) in
# instrumenta/effigies/, probata in probatio_tessera_effigies.

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
    "-I$TESSERA_DIR/instrumenta/effigies"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$TESSERA_DIR/effigies_fontes_generata.sh"
obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done
for src in "$TESSERA_DIR"/fontes/*.c "$TESSERA_DIR"/instrumenta/effigies/*.c; do
    f="$(basename "$src" .c)"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
        echo "  [tessera] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

echo "  [tessera] effigies.c"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$TESSERA_DIR/instrumenta/principalia/effigies.c" $obj_files \
    -o "$BUILD_DIR/effigies" || exit 1

if [ "$#" -eq 0 ]; then
    exec "$BUILD_DIR/effigies" "$RADIX_DIR"/probationes/fixa/quadrans/*.jpg
fi
exec "$BUILD_DIR/effigies" "$@"
