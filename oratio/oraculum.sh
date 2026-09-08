#!/bin/bash
# oratio/oraculum.sh - oraculum treebank UD: classes orationis contra CoNLL-U (T13)
#
# Usage:  ./oratio/oraculum.sh [-machina] [-exempla] [-discrepantiae] [-regulae [-ab N]] [-clausulae [-sententiae N [-causa T]]] [-semina]
#         [-errata [-auctor T]] [-crudus] [-petere] [plagula.conllu ...]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Obiecta oratio/build/*.o poscit (cursor oratio/compile_probationes.sh ea
# struit); instrumentum in oratio/build/vocabula.
set -u
ORATIO_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$ORATIO_DIR/build"
# SERA suitae orationis (tools/sera.sh, 2026-09-07): obiecta et
# binaria in oratio/build sub cursore currenti non tanguntur; cursor
# ab hoc vocatus reentrat; exitus 2 si tenta post SERA_TECTUM.
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
BIN="$BUILD_DIR/oraculum"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$ORATIO_DIR/fontes")
SRC="$ORATIO_DIR/instrumenta/oraculum.c"
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/oratio_*.o 2>/dev/null)" ]; then
    echo "arbor: obiecta absunt - ./oratio/compile_probationes.sh registrum primum" >&2
    exit 2
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$(find "$BUILD_DIR" -name 'oratio_*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 1
fi
# -petere: treebanks NC (relatae solum, numquam venditae) in oratio/build/ud
ARGS=()
for a in "$@"; do
    if [ "$a" = "-petere" ]; then
        mkdir -p "$BUILD_DIR/ud"
        for u in UD_Latin-ITTB/master/la_ittb-ud-test.conllu UD_Latin-PROIEL/master/la_proiel-ud-test.conllu UD_Latin-Perseus/master/la_perseus-ud-test.conllu UD_Latin-UDante/master/la_udante-ud-test.conllu; do
            f="$BUILD_DIR/ud/$(basename "$u")"
            [ -s "$f" ] || curl -sL -o "$f" "https://raw.githubusercontent.com/UniversalDependencies/$u" || echo "oraculum: $u non petitum" >&2
            [ -s "$f" ] && ARGS+=("$f")
        done
    else
        ARGS+=("$a")
    fi
done
# non exec: crusta manet ut sera suitae usque ad finem instrumenti
# teneatur et in exitu dimittatur (exec trap EXIT praeterit -
# sera cum pid mortuo relicta, 2026-09-07)
# ${ARGS[@]+...}: bash 3.2 sub set -u tabulam vacuam "non ligatam" vocat
RHUBARB_RADIX="$RADIX_DIR" "$BIN" ${ARGS[@]+"${ARGS[@]}"}
exit $?
