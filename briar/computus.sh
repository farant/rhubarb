#!/bin/bash
# md/computus.sh - imago memoriae et temporis parsatoris thistle (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem tertium
#
# usus: ./briar/computus.sh <plagula.md> [-machina] [-iter N]
#
# Obiecta briar/build/*.o poscit (./briar/compile_probationes.sh primum) -
# ut materia/shim_probare.sh; binaria probationum (sine .o) non nectit.
set -u
BRIAR_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$BRIAR_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD="$BRIAR_DIR/build"
cd "$RADIX_DIR"
source "$RADIX_DIR/tools/vexilla.sh"

OBIECTA=$(ls "$BUILD"/*.o 2>/dev/null | grep -v -E "/probatio_")
if [ -z "$OBIECTA" ]; then
    echo "DEEST: briar/build/*.o - curre ./briar/compile_probationes.sh primum" >&2
    exit 2
fi
BIN="$BUILD/computus"
SRC="$BRIAR_DIR/instrumenta/computus.c"
recens=""
if [ -f "$BIN" ]; then
    recens="$(find "$BUILD" -name '*.o' -newer "$BIN" 2>/dev/null | head -1)"
fi
if [ ! -f "$BIN" ] || [ "$SRC" -nt "$BIN" ] || [ -n "$recens" ]; then
    echo "  [computus] briar" >&2
    clang "${VEXILLA_C89[@]}" -I"$RADIX_DIR/include" -I"$MATERIA_DIR/fontes" \
        -I"$RADIX_DIR/md/fontes" -I"$BRIAR_DIR/fontes" -I"$BRIAR_DIR/probationes" \
        "$SRC" $OBIECTA -o "$BIN" || exit 1
fi
exec "$BIN" "$@"
