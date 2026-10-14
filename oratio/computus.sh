#!/bin/bash
# oratio/computus.sh - imago memoriae et temporis parsatoris orationis (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem quartum
#
# usus: ./oratio/computus.sh <plagula.txt> [-machina] [-iter N]
#
# Instrumentum ex clausura sua; binarium in oratio/build/computus.
set -u
ORATIO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
BUILD="$ORATIO_DIR/build"
BUILD_DIR="$BUILD"
# SERA suitae orationis (tools/sera.sh, 2026-09-07): obiecta et
# binaria in oratio/build sub cursore currenti non tanguntur; cursor
# ab hoc vocatus reentrat; exitus 2 si tenta post SERA_TECTUM.
mkdir -p "$BUILD"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD/cursor.sera" || exit 2
cd "$RADIX_DIR"
BIN="$BUILD/computus"
SRC="$ORATIO_DIR/instrumenta/computus.c"
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
