#!/bin/bash

# silva/differre.sh - differentia semantica unitatum C89
#
# Usage:
#   ./silva/differre.sh <vetus.c> <novum.c> [-machina]
# Exit:  0 cucurrit | 2 usus/plagula illegibilis
#
# QUID mutatum est in unitatibus (functiones, typi, macra, data):
# MUTATA [substantiva|documentaria|cosmetica] cum textu unificato,
# MOTA (contentum idem, sede alia), ADDITA, REMOTA. -machina = TSV
# (genus, titulus, status, classificatio, +N, -M) - par fistulis
# (nexus.sh, vocantes). Versiones e VCS vocator materializat
# (git show HEAD:x.c > /tmp/x_vetus.c; silex e massis).
#
# NB: semper ex radice repositorii currit (cd infra).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/differre_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" "$SILVA_DIR/instrumenta" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# moduli instrumentorum communes (scansor unitatum)
for base in silva_unitates; do
    src="$SILVA_DIR/instrumenta/$base.c"
    obj="$BUILD_DIR/instr_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [instr] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

DIFFERRE_SRC="$SILVA_DIR/instrumenta/principalia/differre.c"
DIFFERRE_BIN="$BUILD_DIR/differre"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$DIFFERRE_SRC" $obj_files \
    -o "$DIFFERRE_BIN" || exit 1

cd "$RADIX_DIR"
exec "$DIFFERRE_BIN" "$@"
