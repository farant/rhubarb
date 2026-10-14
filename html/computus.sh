#!/bin/bash
# html/computus.sh - imago memoriae et temporis parseris HTML (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem quintum
#
# usus: ./html/computus.sh <plagula.html> [-machina] [-iter N]
#
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta html/build/*.o quae cursor vetus struebat - cursores ad
# gradus, html 2026-10-14; exemplar css/computus.sh); binarium in
# html/build/computus.
set -u
HTML_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$HTML_DIR/.." && pwd)"
BUILD_DIR="$HTML_DIR/build"
BIN="$BUILD_DIR/computus"
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 1
cursor_instrumenta_parare >&2 || exit 1
cursor_instrumentum_struere "$HTML_DIR/instrumenta/computus.c" "$BIN" >&2 || exit 1
exec "$BIN" "$@"
