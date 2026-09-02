#!/bin/bash

# tools/aedilis_struere.sh - fabrica aedilis (mos silva/nexus.sh)
#
# Aedilis amalgama silvae nexum eget (-Isilva/amalgama), quod ansa
# generica instrumentorum non praebet - ergo scriptum fabricae
# proprium. Obiecta in build/aedilis/, binarium bin/aedilis.
#
# Usus: ./tools/aedilis_struere.sh

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/aedilis"
mkdir -p "$BUILD_DIR" "$RADIX_DIR/bin"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
)
# SE-HOSPITANS: dependentiae propriae ab aedile ipso derivatae
# (snippet commissum - ovum bootstrap fractum; regeneratio:
# ./tools/fontes_generare.sh tools/aedilis_fontes_generata.sh
#  tools/aedilis.c)
source "$SCRIPT_DIR/aedilis_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
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

# amalgama se-continens: sine -I compilatur; silva.h mutatio formae
# sine recompilo = corruptio ABI (lectio nexus.sh). -O2: parsatio
# 1.108 -> 0.661 ms/KB mensurata, compilatio tamen sub-secundo.
SILVA_H="$RADIX_DIR/silva/amalgama/silva.h"
src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ "$SILVA_H" -nt "$obj" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -O2 -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SCRIPT_DIR/aedilis.c" \
    $obj_files -o "$RADIX_DIR/bin/aedilis" || exit 1
echo "bin/aedilis paratum" >&2
