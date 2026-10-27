#!/bin/bash

# silva/transformare.sh - formae vocationum super arbores silvae
#
# Usage:
#   ./silva/transformare.sh -census <functio> <viae...> [-machina]
#   ./silva/transformare.sh -regulae <r.stml> <viae...> [-machina] [-scribere]
# Exit:  census 0 situs inventi | 1 nulli; regulae 0 omnes rescripti
#        (scripti) | 1 recusati aut non scripti; 2 apparatus/usus
#
# Census: omnes vocationes functionis per formam consumentis gregatae
# (spec project-specs/transformare-spec.md III). Directoria recursive
# (.c/.h). -machina = TSV per situm.
#
# NB: semper ex radice repositorii per HOC involucrum currendum
# (recompilatio si stala); viae relativae ad radicem.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
# radices -I ex aedilis (vexilla VI: nullae manu scriptae)
vexilla_radices "$RADIX_DIR/" || { echo "FRACTA: radices inclusae (bin/aedilis --radices)" >&2; exit 2; }
declare -a INCLUDE_FLAGS=("${RADICES_C89[@]}")
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/transformare_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" "$RADIX_DIR/materia/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
    fi
    obj_files="$obj_files $obj"
done

source "$RADIX_DIR/silva/materia_substratum.sh"
materia_substratum_struere "$BUILD_DIR" "${GCC_FLAGS[@]}" || exit 2
obj_files="$obj_files $MATERIA_OBIECTA"

TRANSFORMARE_SRC="$SILVA_DIR/instrumenta/principalia/transformare.c"
TRANSFORMARE_BIN="$BUILD_DIR/transformare"
source "$RADIX_DIR/tools/instrumentum_nectere.sh"
instrumentum_nectere "$TRANSFORMARE_BIN" "$TRANSFORMARE_SRC" "$SILVA_DIR/instrumenta/silva_iudicium.c" "$SILVA_DIR/instrumenta/silva_lexicon.c" "$SILVA_DIR/instrumenta/silva_emendatio.c" $obj_files \
    "$SILVA_DIR"/instrumenta/*.h "$SILVA_DIR"/instrumenta/principalia/*.h -- \
    "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$TRANSFORMARE_SRC" \
    "$SILVA_DIR/instrumenta/silva_iudicium.c" \
    "$SILVA_DIR/instrumenta/silva_lexicon.c" \
    "$SILVA_DIR/instrumenta/silva_emendatio.c" $obj_files || exit 2

cd "$RADIX_DIR"
# -scribere: custodia clang vexillis domus et radicibus utitur
exec "$TRANSFORMARE_BIN" "$@" -- "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}"
