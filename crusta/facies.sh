#!/bin/bash
# crusta/facies.sh - diagnostica crustae per faciem (gradus I et II)
#
# Usage:  ./crusta/facies.sh <plagula...> [-machina] [-cruda]
#                            [-sine-excusatione] [-sine-excerpto]
# Exit:   0 nullum erratum | 1 erratum | 2 nihil iudicatum
#
# Obiecta crusta/build/*.o poscit (cursor crusta/compile_probationes.sh
# ea struit); instrumentum in crusta/build/facies. Regulae ex
# crusta/lintrum/ leguntur, ergo hoc a RADICE repositorii currit.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$CRUSTA_DIR/build"
BIN="$BUILD_DIR/facies"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$CRUSTA_DIR/fontes")
SRC="$CRUSTA_DIR/instrumenta/facies.c"
# INSTRUMENTUM SE STRUIT (migratio ad compilatorem): clausura sua ab
# aedile, obiecta per bin/compilator in build/instrumenta/facies/, nexus
# solum si obiectum mutatum. Olim obiecta cursoris (crusta/build/*.o,
# omnia) poscebat et excubitore (mtime) custodiebatur - cursor nunc
# solas clausuras probationum compilat.
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || {
    echo "facies: aedificatio fracta" >&2; exit 2; }
export CRUSTA_LINTRUM="${CRUSTA_LINTRUM:-$RADIX_DIR/crusta/lintrum}"
cd "$RADIX_DIR" || exit 2
exec "$BIN" "$@"
