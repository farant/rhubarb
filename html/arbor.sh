#!/bin/bash
# html/arbor.sh - proiectio STML plagulae HTML
#
# Usage:  ./html/arbor.sh <plagula.html> [-tacitus] [-sedes]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta html/build/*.o quae cursor vetus struebat - cursores ad
# gradus, html 2026-10-14); binarium in html/build/arbor.
set -u
HTML_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$HTML_DIR/.." && pwd)"
BUILD_DIR="$HTML_DIR/build"
BIN="$BUILD_DIR/arbor"
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$HTML_DIR/instrumenta/arbor.c" "$BIN" >&2 || exit 2
exec "$BIN" "$@"
