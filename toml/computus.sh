#!/bin/bash
# toml/computus.sh - imago memoriae et temporis clientis toml (gemellus
# crusta/computus.sh) - semita MATERIAE per clientem septimum
#
# usus: ./toml/computus.sh <plagula.toml> [-machina] [-iter N]
#
# Obiecta toml/build/*.o poscit (./toml/compile_probationes.sh primum) -
# ut materia/oraculum_silvae_struere.sh; binaria probationum (sine .o) non nectit.
set -u
TOML_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$TOML_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD="$TOML_DIR/build"
cd "$RADIX_DIR"
source "$RADIX_DIR/tools/vexilla.sh"

OBIECTA=$(ls "$BUILD"/*.o 2>/dev/null | grep -v -E "/probatio_")
if [ -z "$OBIECTA" ]; then
    echo "DEEST: toml/build/*.o - curre ./toml/compile_probationes.sh primum" >&2
    exit 2
fi
BIN="$BUILD/computus"
SRC="$TOML_DIR/instrumenta/computus.c"
recens=""
if [ -f "$BIN" ]; then
    recens="$(find "$BUILD" -name '*.o' -newer "$BIN" 2>/dev/null | head -1)"
fi
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$recens" ]; then
    echo "  [computus] toml" >&2
    clang "${VEXILLA_C89[@]}" -I"$TOML_DIR/fontes" -I"$RADIX_DIR/include" \
        -I"$MATERIA_DIR/fontes" -I"$TOML_DIR/probationes" \
        "$SRC" $OBIECTA -o "$BIN" || exit 1
fi
exec "$BIN" "$@"
