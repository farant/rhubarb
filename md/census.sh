#!/bin/bash
# md/census.sh - census corporis markdown (C2): numeri ex arbore parsata
#
# Usage:  ./md/census.sh [-machina | -citata [via.md]]
#   (nihil)    tabulae markdown in stdout (RELATIO AD PETITIONEM)
#   -machina   TSV 'clavis<tab>valor'
#   -citata    verbatim (code spans) omnia: 'via<tab>linea<tab>contentum'
#              (via data = plagula illa sola, etiam extra git)
# md/CENSUS.md commissum NON iam est (Franus 2026-10-15): mensura corporis
# totius relatio est, non fons - signum generati in linea III ferebat et
# actionem fabricae nullam habebat (ingressus = omnis *.md).
# Corpus = git ls-files '*.md' (ambulatio directoriorum scripta
# invenit). Exit: 0 sanum | 2 fractura. Instrumentum ex clausura SUA
# (tools/cursor_communis.sh; cursores ad gradus T4).
set -u
MD_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"
BUILD_DIR="$MD_DIR/build"
BIN="$BUILD_DIR/census"
SRC="$MD_DIR/instrumenta/census.c"
MODUS="${1:-}"
PLAGULA="${2:-}"
case "$MODUS" in ""|-machina|-citata) ;; *)
    echo "usus: ./md/census.sh [-machina | -citata [via.md]]" >&2; exit 2 ;;
esac
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 2
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
    "")        "$BIN" -corpus "$LISTA" -nota "$NOTA" ;;
    *)         "$BIN" -corpus "$LISTA" "$MODUS" ;;
esac
