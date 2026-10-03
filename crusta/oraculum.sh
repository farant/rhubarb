#!/bin/bash
# crusta/oraculum.sh - oraculum bash crustae (P11b)
#
# Usage:  ./crusta/oraculum.sh -scribere          # aura expectata.txt, sanitas.txt
#         ./crusta/oraculum.sh -probare           # bash iterum, aura comparata
#         ./crusta/oraculum.sh -domus [plagulae]  # sine plagulis: git ls-files '*.sh'
# Exit:   0 sanum | 1 aurum differt | 2 usus / bash non 5.2 / absens
# Obiecta crusta/build/*.o poscit (cursor crusta/compile_probationes.sh ea
# struit); instrumentum in crusta/build/oraculum. Bash vivum currit (ambitus
# restrictus, vide instrumenta/oraculum.c); portae aurum solum legunt.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$CRUSTA_DIR/build"
BIN="$BUILD_DIR/oraculum"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$CRUSTA_DIR/fontes")
SRC="$CRUSTA_DIR/instrumenta/oraculum.c"
# INSTRUMENTUM SE STRUIT (migratio ad compilatorem): clausura sua ab
# aedile, obiecta per bin/compilator in build/instrumenta/oraculum/, nexus
# solum si obiectum mutatum. Olim obiecta cursoris (crusta/build/*.o,
# omnia) poscebat et excubitore (mtime) custodiebatur - cursor nunc
# solas clausuras probationum compilat.
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || {
    echo "oraculum: aedificatio fracta" >&2; exit 1; }
cd "$RADIX_DIR" || exit 2
export RHUBARB_RADIX="$RADIX_DIR"
if [ "${1:-}" = "-domus" ] && [ "$#" -eq 1 ]; then
    # viae domus sine spatiis (git ls-files)
    set -- -domus $(git ls-files '*.sh')
fi
exec "$BIN" "$@"
