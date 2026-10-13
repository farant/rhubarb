#!/bin/bash
# md/extenta.sh - extenta structuralia markdown per selectorem (C1)
#
# Usage:  ./md/extenta.sh <plagula.md> '<selector>'
# Effusum TSV: via index tag b-initium b-finis linea columna linea-finis
# Exit:   0 congruentia | 1 nulla | 2 fractura (usus/plagula/selector)
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta cursoris veteris - cursores ad gradus T4);
# binarium in md/build/extenta. Gemellum selecta.sh pro
# markdown; silva.Prosa hoc legit.
set -u
MD_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"
BUILD_DIR="$MD_DIR/build"
BIN="$BUILD_DIR/extenta"
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$RADIX_DIR/materia/fontes" "-I$MD_DIR/fontes")
SRC="$MD_DIR/instrumenta/extenta.c"
if [ "$#" -ne 2 ]; then
    echo "usus: ./md/extenta.sh <plagula.md> '<selector>'" >&2
    exit 2
fi
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 2
exec "$BIN" "$@"
