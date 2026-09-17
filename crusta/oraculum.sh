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
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/crusta_*.o 2>/dev/null)" ]; then
    echo "oraculum: obiecta absunt - ./crusta/compile_probationes.sh registrum primum" >&2
    exit 2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$(find "$BUILD_DIR" -name 'crusta_*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 1
fi
cd "$RADIX_DIR" || exit 2
export RHUBARB_RADIX="$RADIX_DIR"
if [ "${1:-}" = "-domus" ] && [ "$#" -eq 1 ]; then
    # viae domus sine spatiis (git ls-files)
    set -- -domus $(git ls-files '*.sh')
fi
exec "$BIN" "$@"
