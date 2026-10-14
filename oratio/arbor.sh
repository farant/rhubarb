#!/bin/bash
# oratio/arbor.sh - proiectio STML plagulae orationis
#
# Usage:  ./oratio/arbor.sh <plagula.txt> [-tacitus] [-sedes]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Instrumentum ex clausura sua; binarium in oratio/build/arbor.
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
BIN="$BUILD_DIR/arbor"
SRC="$ORATIO_DIR/instrumenta/arbor.c"
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, 2026-10-14):
# olim obiecta oratio/build/*.o ligabantur quae cursor ULTIMO struxerat
# (vetustas nulla iudicata - instrumentum contra fontes veteres)
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 2
# non exec: crusta manet ut sera suitae usque ad finem instrumenti
# teneatur et in exitu dimittatur (exec trap EXIT praeterit -
# sera cum pid mortuo relicta, 2026-09-07)
"$BIN" "$@"
exit $?
