#!/bin/bash
# oratio/computus.sh - imago memoriae et temporis parsatoris orationis (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem quartum
#
# usus: ./oratio/computus.sh <plagula.txt> [-machina] [-iter N]
#
# Obiecta oratio/build/*.o poscit (./oratio/compile_probationes.sh primum) -
# ut materia/shim_probare.sh; binaria probationum (sine .o) non nectit.
set -u
ORATIO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD="$ORATIO_DIR/build"
# SERA suitae orationis (tools/sera.sh, 2026-09-07): obiecta et
# binaria in oratio/build sub cursore currenti non tanguntur; cursor
# ab hoc vocatus reentrat; exitus 2 si tenta post SERA_TECTUM.
mkdir -p "$BUILD"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD/cursor.sera" || exit 2
cd "$RADIX_DIR"
source "$RADIX_DIR/tools/vexilla.sh"

OBIECTA=$(ls "$BUILD"/*.o 2>/dev/null | grep -v -E "/probatio_")
if [ -z "$OBIECTA" ]; then
    echo "DEEST: oratio/build/*.o - curre ./oratio/compile_probationes.sh primum" >&2
    exit 2
fi
BIN="$BUILD/computus"
SRC="$ORATIO_DIR/instrumenta/computus.c"
recens=""
if [ -f "$BIN" ]; then
    recens="$(find "$BUILD" -name '*.o' -newer "$BIN" 2>/dev/null | head -1)"
fi
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$recens" ]; then
    echo "  [computus] oratio" >&2
    clang "${VEXILLA_C89[@]}" -I"$RADIX_DIR/include" -I"$MATERIA_DIR/fontes" \
        -I"$ORATIO_DIR/fontes" -I"$ORATIO_DIR/probationes" -I"$RADIX_DIR/md/fontes" \
        "$SRC" $OBIECTA -o "$BIN" || exit 1
fi
# non exec: crusta manet ut sera suitae usque ad finem instrumenti
# teneatur et in exitu dimittatur (exec trap EXIT praeterit -
# sera cum pid mortuo relicta, 2026-09-07)
"$BIN" "$@"
exit $?
