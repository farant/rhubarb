#!/bin/bash
# md/census.sh - census corporis markdown (C2): numeri ex arbore parsata
#
# Usage:  ./md/census.sh [-machina | -citata [via.md] | -scribere]
#   (nihil)    tabulae markdown in stdout
#   -machina   TSV 'clavis<tab>valor'
#   -citata    verbatim (code spans) omnia: 'via<tab>linea<tab>contentum'
#              (via data = plagula illa sola, etiam extra git)
#   -scribere  md/CENSUS.md scribit (mensura COMMISSA, regenerabilis)
# Corpus = git ls-files '*.md' (ambulatio directoriorum scripta
# invenit). Exit: 0 sanum | 2 fractura. Obiecta md/build/*.o poscit.
set -u
MD_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$MD_DIR/build"
BIN="$BUILD_DIR/census"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$MD_DIR/fontes")
SRC="$MD_DIR/instrumenta/census.c"
MODUS="${1:-}"
PLAGULA="${2:-}"
case "$MODUS" in ""|-machina|-citata|-scribere) ;; *)
    echo "usus: ./md/census.sh [-machina | -citata [via.md] | -scribere]" >&2; exit 2 ;;
esac
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/md_*.o 2>/dev/null)" ]; then
    echo "census: obiecta absunt - ./md/compile_probationes.sh registrum primum" >&2
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
cd "$RADIX_DIR" || exit 2
LISTA="$BUILD_DIR/corpus_md.txt"
if [ -n "$PLAGULA" ]; then
    LISTA="$BUILD_DIR/census_una.txt"
    printf '%s\n' "$PLAGULA" > "$LISTA"
else
    git ls-files '*.md' > "$LISTA" || { echo "census: git ls-files fractum" >&2; exit 2; }
fi
NOTA="$(date +%Y-%m-%d) HEAD $(git rev-parse --short HEAD 2>/dev/null || echo '?')"
case "$MODUS" in
    -scribere) "$BIN" -corpus "$LISTA" -nota "$NOTA" > "$MD_DIR/CENSUS.md" && echo "census: md/CENSUS.md scriptum" ;;
    "")        "$BIN" -corpus "$LISTA" -nota "$NOTA" ;;
    *)         "$BIN" -corpus "$LISTA" "$MODUS" ;;
esac
