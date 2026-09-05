#!/bin/bash
# briar/fabrica.sh - thistle -> proiectum silicis in directorio (sine clang)
#
# Usage:  ./briar/fabrica.sh <plagula.thistle> <directorium> [-fons <radix>]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Obiecta briar/build/*.o poscit (cursor briar/compile_probationes.sh
# ea struit); instrumentum in briar/build/fabrica.
set -u
BRIAR_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
MD_DIR="$(cd "$BRIAR_DIR/../md" && pwd)"
RADIX_DIR="$(cd "$BRIAR_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$BRIAR_DIR/build"
BIN="$BUILD_DIR/fabrica"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$MD_DIR/fontes" "-I$BRIAR_DIR/fontes" "-I$RADIX_DIR/silva/amalgama")
SRC="$BRIAR_DIR/instrumenta/fabrica.c"
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/briar_*.o 2>/dev/null)" ]; then
    echo "fabrica: obiecta absunt - ./briar/compile_probationes.sh registrum primum" >&2
    exit 2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || [ "$SRC" -nt "$BIN" ] || [ -n "$(find "$BUILD_DIR" -name '*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 1
fi
exec "$BIN" "$@"
