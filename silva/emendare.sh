#!/bin/bash

# silva/emendare.sh - emendator codicum (actio scriptoria prima)
#
# Usage:
#   ./silva/emendare.sh 85 <plagulae...> [-scribere]
#   ./silva/emendare.sh 85 -census [-scribere]   (plagulae e censu)
# Exit:  0 omnia sana(re)ntur | 1 aliqua fracta | 2 apparatus/usus
#
# Consilium ORDINARIUM (nihil scribitur, verificatio in memoria);
# -scribere applicat POST verificationem per plagulam. -census legit
# silva/build/census.tsv (codex 85) - censum primum curre:
#   ./silva/examen.sh -census 85
#
# NB: semper ex radice repositorii per HOC involucrum currendum
# (recompilatio si stala; binarium directum vigilia non custodit).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/emendare_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
    fi
    obj_files="$obj_files $obj"
done

EMENDARE_SRC="$SILVA_DIR/instrumenta/principalia/emendare.c"
EMENDARE_BIN="$BUILD_DIR/emendare"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    -I"$SILVA_DIR/instrumenta" \
    "$EMENDARE_SRC" \
    "$SILVA_DIR/instrumenta/silva_iudicium.c" \
    "$SILVA_DIR/instrumenta/silva_lexicon.c" $obj_files \
    -o "$EMENDARE_BIN" || exit 2

cd "$RADIX_DIR"

# -census: plagulae e tabula census (codex in columna 5)
args=()
census_modus=0
for a in "$@"; do
    if [ "$a" = "-census" ]; then
        census_modus=1
    else
        args+=("$a")
    fi
done
if [ "$census_modus" = "1" ]; then
    CENSUS_TSV="$BUILD_DIR/census.tsv"
    if [ ! -f "$CENSUS_TSV" ]; then
        echo "emendare: census.tsv deest - primum curre:" >&2
        echo "  ./silva/examen.sh -census 85" >&2
        exit 2
    fi
    while IFS= read -r via; do
        args+=("$via")
    done < <(awk -F'\t' '$5==85 {print $1}' "$CENSUS_TSV" | sort -u)
fi

exec "$EMENDARE_BIN" "${args[@]}"
