#!/bin/bash

# officina/sonda.sh - sonda una-ictus viae LSP (desideratum
# 01KXJ2K1Z9): machina legati recens, initialize -> didOpen ->
# [-servare] -> publicatio ULTIMA impressa. Instrumentum
# reproductionis/oculorum - NON iudicii cotidiani (id examen/mcp
# diagnostica praestant).
#
# Usage:
#   ./officina/sonda.sh <via> [-servare] [-crudum]
#
# Aedificatio: obiecta cum legatus.sh COMPARTITA (idem BUILD_DIR,
# eaedem condiciones) - solum principale sonda.c proprium.

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build/legatus"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$RADIX_DIR/silva/instrumenta"
    "-I$OFF_DIR/instrumenta"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$OFF_DIR/sonda_fontes_generata.sh"
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

SILVA_H="$RADIX_DIR/silva/amalgama/silva.h"

src="$RADIX_DIR/silva/instrumenta/nexus_ordines.c"
obj="$BUILD_DIR/nexus_ordines.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$RADIX_DIR/silva/instrumenta/nexus_ordines.h" -nt "$obj" ] \
    || [ "$SILVA_H" -nt "$obj" ]; then
    echo "  [ordines] nexus_ordines.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

# silva_lexicon (compositio systematis - praeparator eam vocat)
src="$RADIX_DIR/silva/instrumenta/silva_lexicon.c"
obj="$BUILD_DIR/silva_lexicon.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$RADIX_DIR/silva/instrumenta/silva_lexicon.h" -nt "$obj" ]; then
    echo "  [lexicon] silva_lexicon.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "-I$RADIX_DIR/silva/fontes" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for unit in praeparator legatus; do
    src="$OFF_DIR/instrumenta/$unit.c"
    obj="$BUILD_DIR/$unit.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
        || [ "$OFF_DIR/instrumenta/$unit.h" -nt "$obj" ] \
        || [ "$SILVA_H" -nt "$obj" ]; then
        echo "  [$unit] $unit.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

BIN="$BUILD_DIR/sonda"
src="$OFF_DIR/instrumenta/principalia/sonda.c"
if [ ! -f "$BIN" ] || [ "$src" -nt "$BIN" ] || [ -n "$obj_files" ]; then
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$src" $obj_files -o "$BIN" || exit 1
fi

cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR"
