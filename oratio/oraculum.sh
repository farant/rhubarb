#!/bin/bash
# oratio/oraculum.sh - oraculum treebank UD: classes orationis contra CoNLL-U (T13)
#
# Usage:  ./oratio/oraculum.sh [-machina] [-exempla] [-discrepantiae] [-regulae [-ab N]] [-clausulae [-sententiae N [-causa T]]] [-semina]
#         [-errata [-auctor T]] [-crudus] [-petere] [plagula.conllu ...]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Instrumentum ex clausura sua in oratio/build/instrumenta/oraculum.
set -u
ORATIO_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
BUILD_DIR="$ORATIO_DIR/build"
# SERA suitae orationis (tools/sera.sh, 2026-09-07): obiecta et
# binaria in oratio/build sub cursore currenti non tanguntur; cursor
# ab hoc vocatus reentrat; exitus 2 si tenta post SERA_TECTUM.
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
SRC="$ORATIO_DIR/instrumenta/oraculum.c"
BIN="$BUILD_DIR/instrumenta/oraculum/oraculum"
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, 2026-10-14):
# aedilis fontes ex inclusionibus derivat, bin/compilator per
# thesaurum compilat, binarium religatur solum si obiectum mutatum.
# Antea obiecta cursoris oratio/build/*.o ligabantur, vetustas per
# 'excubitor.sh oratio/build/' iudicata - XIII s per vocationem (find
# et stat super totum repositorium), oraculum ipsum 0.04 s.
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 2
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
