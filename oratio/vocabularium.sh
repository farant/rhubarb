#!/bin/bash
# oratio/vocabularium.sh - tabula vocabularii Latini: coquere, recensere, sigillum (T7)
#
# Usage:  ./oratio/vocabularium.sh [-coquere] [-scribere] [-census]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Obiecta oratio/build/*.o poscit (cursor oratio/compile_probationes.sh ea
# struit); instrumentum in oratio/build/vocabularium.
set -u
ORATIO_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$ORATIO_DIR/build"
BIN="$BUILD_DIR/vocabularium"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$ORATIO_DIR/fontes")
SRC="$ORATIO_DIR/instrumenta/vocabularium.c"
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/oratio_*.o 2>/dev/null)" ]; then
    echo "arbor: obiecta absunt - ./oratio/compile_probationes.sh registrum primum" >&2
    exit 2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || [ "$SRC" -nt "$BIN" ] || [ -n "$(find "$BUILD_DIR" -name 'oratio_*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 1
fi
RHUBARB_RADIX="$RADIX_DIR" exec "$BIN" "$@"
