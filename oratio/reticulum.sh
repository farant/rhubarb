#!/bin/bash
# oratio/reticulum.sh - instrumentum reticuli partitionum (T35 d): ordines
# machinae cum ordine COLUMNAE per titulum, partitiones retentae
#
# Usage:  ./oratio/reticulum.sh <x.tsv> -genus K -aurum aurum-... -sortes columna
#           [-ubi titulus=v1,v2]... [-columnae t1,t2 | -praeter t1,t2]
#           [-gradus t1,t2] [-limen N] [-lucrum N] [-greges columna|catena] [-machina]
# Exit:   0 iudicatum | 2 nihil iudicatum (causa in stderr), usus, plagula absens
# Obiecta oratio/build/*.o poscit (cursor oratio/compile_probationes.sh ea
# struit); instrumentum in oratio/build/reticulum.
set -u
ORATIO_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$ORATIO_DIR/build"
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
BIN="$BUILD_DIR/reticulum"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$ORATIO_DIR/fontes")
SRC="$ORATIO_DIR/instrumenta/reticulum.c"
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/oratio_*.o 2>/dev/null)" ] || [ ! -f "$BUILD_DIR/partitio.o" ]; then
    echo "reticulum: obiecta absunt - ./oratio/compile_probationes.sh reticulum primum" >&2
    exit 2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$(find "$BUILD_DIR" -name '*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 1
fi
# non exec: crusta manet ut sera suitae usque ad finem instrumenti teneatur
"$BIN" "$@"
exit $?
