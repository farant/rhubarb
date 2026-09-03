#!/bin/bash
# css/computus.sh - imago memoriae et temporis parseris CSS (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem primum
#
# usus: ./css/computus.sh <plagula.css> [-machina] [-iter N]
#
# Obiecta css/build/*.o poscit (./css/compile_probationes.sh primum) -
# ut materia/shim_probare.sh; binaria probationum (sine .o) non nectit.
set -u
CSS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$CSS_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD="$CSS_DIR/build"
cd "$RADIX_DIR"
source "$RADIX_DIR/tools/vexilla.sh"

OBIECTA=$(ls "$BUILD"/*.o 2>/dev/null | grep -v -E "/probatio_")
if [ -z "$OBIECTA" ]; then
    echo "DEEST: css/build/*.o - curre ./css/compile_probationes.sh primum" >&2
    exit 2
fi
BIN="$BUILD/computus"
SRC="$CSS_DIR/instrumenta/computus.c"
recens=""
if [ -f "$BIN" ]; then
    recens="$(find "$BUILD" -name '*.o' -newer "$BIN" 2>/dev/null | head -1)"
fi
if [ ! -f "$BIN" ] || [ "$SRC" -nt "$BIN" ] || [ -n "$recens" ]; then
    echo "  [computus] css" >&2
    clang "${VEXILLA_C89[@]}" -I"$RADIX_DIR/include" -I"$MATERIA_DIR/fontes" \
        -I"$CSS_DIR/fontes" -I"$CSS_DIR/probationes" \
        "$SRC" $OBIECTA -o "$BIN" || exit 1
fi
exec "$BIN" "$@"
