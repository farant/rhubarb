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

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$TESSERA_DIR/fontes"
)

declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "utf8"
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
