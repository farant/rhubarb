#!/bin/bash

# tools/canon_struere.sh - fabrica canon_examen (mos natura_struere.sh)
#
# Canon = schema STML generale (gradus II). Binarium bin/canon_examen,
# obiecta in build/canon/.
#
# Usus: ./tools/canon_struere.sh

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/canon"
mkdir -p "$BUILD_DIR" "$RADIX_DIR/bin"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
)
declare -a INCLUDE_FLAGS=( "-I$RADIX_DIR/include" )

# stml_macros: canon.c stml_expandere vocat (iudicium expansum)
FONTES=(
    canon stml stml_macros similitudo chorda chorda_aedificator
    piscina xar
    internamentum tabula_dispersa friatio selectio filum
)

newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SCRIPT_DIR/canon_examen.c" \
    $obj_files -o "$RADIX_DIR/bin/canon_examen" || exit 1
echo "bin/canon_examen paratum" >&2

clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SCRIPT_DIR/canon_coquere.c" \
    $obj_files -o "$RADIX_DIR/bin/canon_coquere" || exit 1
echo "bin/canon_coquere paratum" >&2
