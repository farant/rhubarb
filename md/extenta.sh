#!/bin/bash
# md/extenta.sh - extenta structuralia markdown per selectorem (C1)
#
# Usage:  ./md/extenta.sh <plagula.md> '<selector>'
# Effusum TSV: via index tag b-initium b-finis linea columna linea-finis
# Exit:   0 congruentia | 1 nulla | 2 fractura (usus/plagula/selector)
# Obiecta md/build/*.o poscit (cursor md/compile_probationes.sh ea
# struit); instrumentum in md/build/extenta. Gemellum selecta.sh pro
# markdown; silva.Prosa hoc legit.
set -u
MD_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$MD_DIR/build"
BIN="$BUILD_DIR/extenta"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$MD_DIR/fontes")
SRC="$MD_DIR/instrumenta/extenta.c"
if [ "$#" -ne 2 ]; then
    echo "usus: ./md/extenta.sh <plagula.md> '<selector>'" >&2
    exit 2
fi
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/md_*.o 2>/dev/null)" ]; then
    echo "extenta: obiecta absunt - ./md/compile_probationes.sh registrum primum" >&2
    exit 2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || [ "$SRC" -nt "$BIN" ] || [ -n "$(find "$BUILD_DIR" -name 'md_*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 2
fi
exec "$BIN" "$@"
