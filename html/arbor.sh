#!/bin/bash
# html/arbor.sh - proiectio STML plagulae HTML
#
# Usage:  ./html/arbor.sh <plagula.html> [-tacitus] [-sedes]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Obiecta html/build/*.o poscit (cursor html/compile_probationes.sh ea
# struit); instrumentum in html/build/arbor.
set -u
HTML_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$HTML_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$HTML_DIR/build"
BIN="$BUILD_DIR/arbor"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$HTML_DIR/fontes")
SRC="$HTML_DIR/instrumenta/arbor.c"
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/html_*.o 2>/dev/null)" ]; then
    echo "arbor: obiecta absunt - ./html/compile_probationes.sh registrum primum" >&2
    exit 2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$(find "$BUILD_DIR" -name 'html_*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 1
fi
exec "$BIN" "$@"
