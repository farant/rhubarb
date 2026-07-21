#!/bin/bash

# officina/cursor.sh - Cursorem corporis compilare et
# currere (percursus.sh exemplar)
#
# Usage:
#   ./officina/cursor.sh            # ex radice repositorii
#   ./officina/cursor.sh -celer ... # variante VELOCI (-O2 + LTO,
#       ~3.6x mensuratum; obiecta in build/celer/ - numquam mixta
#       cum -O0!) - pro cursibus benedictionis lapifex-classis.
#       Aedificatio -O0 norma manet (celeritas compilandi +
#       debugabilitas).

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build"

CELER=0
if [ "${1:-}" = "-celer" ]; then
    CELER=1
    shift
    BUILD_DIR="$OFF_DIR/build/celer"
fi
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
if [ $CELER -eq 1 ]; then
    GCC_FLAGS+=("-O2" "-flto")
fi
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$OFF_DIR/fontes"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$OFF_DIR/cursor_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$OFF_DIR/fontes" \
        "$RADIX_DIR/silva/amalgama" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
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

src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for src in "$OFF_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [officina] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# Semper renectere (obiecta mutata aliter in binario vetusto latent)
CURSOR_SRC="$OFF_DIR/instrumenta/principalia/cursor.c"
CURSOR_BIN="$BUILD_DIR/cursor"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$CURSOR_SRC" $obj_files \
    -o "$CURSOR_BIN" || exit 1

exec "$CURSOR_BIN" "$@"
