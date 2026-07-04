#!/bin/bash

# silva/arbor.sh - impressor arborum compilare et currere (dev-time)
#
# Usage:
#   ./arbor.sh '2+3*4;'          # fons in linea mandati
#   ./arbor.sh -f via/plagula.c  # plagulam legere

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
)
declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio" "internamentum"
)

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# Semper renectere: obiecta mutata (tabulae regeneratae!) aliter
# in binario vetusto latent - fons erroris classicus
ARBOR_SRC="$SILVA_DIR/instrumenta/principalia/arbor.c"
ARBOR_BIN="$BUILD_DIR/arbor"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$ARBOR_SRC" $obj_files \
    -o "$ARBOR_BIN" || exit 1

exec "$ARBOR_BIN" "$@"
