#!/bin/bash
# crusta/fontationes.sh - quae scripta scriptum fontat aut exsequitur
# (fabrica spec 3 par. III.2: clavis iudicii portae)
#
# Usage:  ./crusta/fontationes.sh <scriptum>
# Exit:   0 omnia resoluta | 1 irresolutum ullum | 2 usus / scriptum absens
#
# Effusio: <genus>\t<via>\t<plagula>:<linea>\t<fontatum|exsecutum>, viae
# radici relativae; scriptum relativum radici, non directorio operis.
# Genera et regulae: crusta/fontes/crusta_fontationes.h.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$CRUSTA_DIR/build"
BIN="$BUILD_DIR/fontationes"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$CRUSTA_DIR/fontes")
SRC="$CRUSTA_DIR/instrumenta/fontationes.c"
# instrumentum se struit (exemplar crusta/facies.sh): clausura sua ab
# aedile, obiecta per bin/compilator, nexus solum si obiectum mutatum
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || {
    echo "fontationes: aedificatio fracta" >&2; exit 2; }
exec "$BIN" -radix "$RADIX_DIR" "$@"
