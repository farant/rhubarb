#!/bin/bash
# crusta/computus.sh - imago memoriae et temporis parseris crustae (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem sextum
#
# usus: ./crusta/computus.sh <plagula.sh> [-machina] [-iter N]
#
# Obiecta crusta/build/*.o poscit (./crusta/compile_probationes.sh primum) -
# ut materia/shim_probare.sh; binaria probationum (sine .o) non nectit.
set -u
CRUSTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD="$CRUSTA_DIR/build"
cd "$RADIX_DIR"
source "$RADIX_DIR/tools/vexilla.sh"

OBIECTA=$(ls "$BUILD"/*.o 2>/dev/null | grep -v -E "/probatio_")
if [ -z "$OBIECTA" ]; then
    echo "DEEST: crusta/build/*.o - curre ./crusta/compile_probationes.sh primum" >&2
    exit 2
fi
BIN="$BUILD/computus"
SRC="$CRUSTA_DIR/instrumenta/computus.c"
recens=""
if [ -f "$BIN" ]; then
    recens="$(find "$BUILD" -name '*.o' -newer "$BIN" 2>/dev/null | head -1)"
fi
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$recens" ]; then
    echo "  [computus] crusta" >&2
    clang "${VEXILLA_C89[@]}" -I"$RADIX_DIR/include" -I"$MATERIA_DIR/fontes" \
        -I"$CRUSTA_DIR/fontes" -I"$CRUSTA_DIR/probationes" \
        "$SRC" $OBIECTA -o "$BIN" || exit 1
fi
exec "$BIN" "$@"
