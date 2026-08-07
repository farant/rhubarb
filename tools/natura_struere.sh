#!/bin/bash

# tools/natura_struere.sh - fabrica natura_examen (mos aedilis_struere.sh)
#
# Porta velox bibliothecae semanticae: onerator lib/natura.c cum
# facie CLI. Obiecta in build/natura/, binarium bin/natura_examen.
#
# Usus: ./tools/natura_struere.sh

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/natura"
mkdir -p "$BUILD_DIR" "$RADIX_DIR/bin"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
)
declare -a INCLUDE_FLAGS=( "-I$RADIX_DIR/include" )

# clausura oneratoris (bin/aedilis --partes natura.c congruens)
FONTES=(
    natura canon stml similitudo chorda chorda_aedificator piscina xar
    internamentum tabula_dispersa friatio selectio
    filum iter_directoria via
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
    "$SCRIPT_DIR/natura_examen.c" \
    $obj_files -o "$RADIX_DIR/bin/natura_examen" || exit 1
echo "bin/natura_examen paratum" >&2

clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SCRIPT_DIR/natura_canones.c" \
    $obj_files -o "$RADIX_DIR/bin/natura_canones" || exit 1
echo "bin/natura_canones paratum" >&2
