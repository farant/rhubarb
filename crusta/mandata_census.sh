#!/bin/bash
# crusta/mandata_census.sh - census titulorum imperii in scriptis domus
# (effectus-plan.md T2: semen tabulae crusta/effectus_mandata.stml)
#
# Usage:  ./crusta/mandata_census.sh            # omnia .sh arboris
#         ./crusta/mandata_census.sh a.sh b.sh  # plagulae nominatae
# Effusio: <classis>\t<titulus>\t<situs>\t<plagulae>, situs descendens
# (classes in crusta/instrumenta/mandata_census.c). oracula/ excluditur
# (glutinum dialecti alienae, non scripta domus).
# Exit:   0 sanum | 2 usus / aedificatio fracta
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$CRUSTA_DIR/build"
BIN="$BUILD_DIR/mandata_census"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$CRUSTA_DIR/fontes")
SRC="$CRUSTA_DIR/instrumenta/mandata_census.c"
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || {
    echo "mandata_census: aedificatio fracta" >&2; exit 2; }
if [ "$#" -gt 0 ]; then
    printf '%s\n' "$@" | "$BIN" -radix "$RADIX_DIR"
else
    (cd "$RADIX_DIR" && git ls-files '*.sh' | grep -v '^oracula/') \
        | "$BIN" -radix "$RADIX_DIR"
fi
