#!/bin/bash
# md/computus.sh - imago memoriae et temporis parsatoris markdown (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem tertium
#
# usus: ./md/computus.sh <plagula.md> [-machina] [-iter N]
#
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta cursoris veteris - cursores ad gradus T4); binarium in
# md/build/computus.
set -u
MD_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"
BUILD_DIR="$MD_DIR/build"
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$RADIX_DIR/materia/fontes" "-I$MD_DIR/fontes")
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 1
cursor_instrumentum_struere "$MD_DIR/instrumenta/computus.c" "$BUILD_DIR/computus" >&2 || exit 1
exec "$BUILD_DIR/computus" "$@"
