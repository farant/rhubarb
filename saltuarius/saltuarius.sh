#!/bin/bash

# saltuarius/saltuarius.sh - Aedificare et currere (terminal verum)
#
# Usage:
#   ./saltuarius.sh [radix]     # defalta: radix repositorii rhubarb
#
# lustrum.sh model: self-contained, project-local build/, exec at
# the end. Both amalgams enter as objects; fontes/ include dirs of
# silva and tessera are deliberately absent (the amalgam boundary
# is the API).

set -u

SALT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SALT_DIR/.." && pwd)"
BUILD_DIR="$SALT_DIR/build"
VIA_RADIX="${1:-$RADIX_DIR}"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$RADIX_DIR/tessera/amalgama"
    "-I$SALT_DIR/fontes"
)

declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio" "filum" "via" "iter_directoria" "utf8"
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

for amalgama in "tessera" "silva"; do
    src="$RADIX_DIR/$amalgama/amalgama/$amalgama.c"
    obj="$BUILD_DIR/amalgama_$amalgama.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [amalgama] $amalgama.c"
        clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

shopt -s nullglob
for src in "$SALT_DIR"/fontes/*.c; do
    f="$(basename "$src" .c)"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [saltuarius] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done
shopt -u nullglob

echo "  [saltuarius] saltuarius.c (principale)"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SALT_DIR/instrumenta/principalia/saltuarius.c" $obj_files \
    -o "$BUILD_DIR/saltuarius" || exit 1

exec "$BUILD_DIR/saltuarius" "$VIA_RADIX"
