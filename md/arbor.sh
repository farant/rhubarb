#!/bin/bash
# md/arbor.sh - proiectio STML plagulae markdown
#
# Usage:  ./md/arbor.sh <plagula.md> [-tacitus] [-sedes]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta cursoris veteris - cursores ad gradus T4);
# binarium in md/build/arbor.
set -u
MD_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"
BUILD_DIR="$MD_DIR/build"
BIN="$BUILD_DIR/arbor"
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$RADIX_DIR/materia/fontes" "-I$MD_DIR/fontes")
SRC="$MD_DIR/instrumenta/arbor.c"
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 1
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 1
exec "$BIN" "$@"
