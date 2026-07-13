#!/bin/bash

# officina/legatus.sh - LEGATUS: servus LSP silvae (chunk B)
#
# Usage:
#   ./officina/legatus.sh          # stdio LSP (cliens spawnat)
#
# Aedificatio incrementalis (exemplar colloquium.sh) - garrulitas
# ad stderr SOLUM (stdout purus est protocollo LSP). Aedificatio
# IUDICIS SOLIUS: amalgama silvae + praeparator + legatus - NULLA
# fontes officinae (machinula/regio absunt consulto).

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build/legatus"
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
    "-I$RADIX_DIR/silva/instrumenta"
    "-I$OFF_DIR/instrumenta"
)

declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio" "internamentum" "json" "tabellarius" "similitudo"
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

src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

src="$RADIX_DIR/silva/instrumenta/nexus_ordines.c"
obj="$BUILD_DIR/nexus_ordines.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$RADIX_DIR/silva/instrumenta/nexus_ordines.h" -nt "$obj" ]; then
    echo "  [ordines] nexus_ordines.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for unit in praeparator legatus; do
    src="$OFF_DIR/instrumenta/$unit.c"
    obj="$BUILD_DIR/$unit.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
        || [ "$OFF_DIR/instrumenta/$unit.h" -nt "$obj" ]; then
        echo "  [$unit] $unit.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

BIN="$BUILD_DIR/legatus"
src="$OFF_DIR/instrumenta/principalia/legatus.c"
if [ ! -f "$BIN" ] || [ "$src" -nt "$BIN" ] || [ -n "$obj_files" ]; then
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$src" $obj_files -o "$BIN" || exit 1
fi

cd "$RADIX_DIR" && exec "$BIN" "$@"
