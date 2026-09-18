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
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/crusta_*.o 2>/dev/null)" ]; then
    echo "facies: obiecta absunt - ./crusta/compile_probationes.sh registrum primum" >&2
    exit 2
fi
# Obiecta stala = instrumentum contra codicem veterem. Excubitor
# graphum inclusionum derivatum legit: stala = refusio nominata.
excubitor_exitus=0
excubitor_relatio="$("$RADIX_DIR/excubitor.sh" crusta/build/ -tacitus 2>&1)" \
    || excubitor_exitus=$?
if [ "$excubitor_exitus" -eq 1 ]; then
    echo "$excubitor_relatio" >&2
    echo "facies: obiecta stala - ./crusta/compile_probationes.sh registrum primum" >&2
    exit 2
elif [ "$excubitor_exitus" -ne 0 ]; then
    echo "$excubitor_relatio" >&2
    echo "CAUTIO: excubitor exitus $excubitor_exitus - vetustas obiectorum NON iudicata" >&2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$(find "$BUILD_DIR" -name '*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" >&2 || {
        echo "facies: ligatio fracta" >&2; exit 2; }
fi
export CRUSTA_LINTRUM="${CRUSTA_LINTRUM:-$RADIX_DIR/crusta/lintrum}"
cd "$RADIX_DIR" || exit 2
exec "$BIN" "$@"
