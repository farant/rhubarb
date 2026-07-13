#!/bin/bash

# officina/vindex.sh - VINDEX: debugger TUI (M3)
#
# Usage:
#   ./officina/vindex.sh <suita.c> [-plagulae a,b,c] [-imago via:linea]
#
# App super amalgamata TRIA (saltuarius exemplar): officina + silva +
# tessera obiecta intrant; fontes/ include dirs CONSULTO absunt
# (limes amalgamatis = API). Currendum ex radice repositorii.

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build/vindex"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$RADIX_DIR/tessera/amalgama"
    "-I$OFF_DIR/amalgama"
    "-I$OFF_DIR/instrumenta"
)

declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio"
)
obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for nom in "tessera" "silva" "officina"; do
    src="$RADIX_DIR/$nom/amalgama/$nom.c"
    obj="$BUILD_DIR/amalgama_$nom.o"
    # officina amalgam: silva.h = the ONE documented external dep
    extra=""
    [ "$nom" = "officina" ] && extra="-I$RADIX_DIR/silva/amalgama"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [amalgama] $nom.c"
        clang "${GCC_FLAGS[@]}" $extra -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for f in "praeparator" "vindex_onerator" "vindex_visum"; do
    src="$OFF_DIR/instrumenta/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
        || [ "$OFF_DIR/instrumenta/$f.h" -nt "$obj" ]; then
        echo "  [vindex] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

src="$OFF_DIR/instrumenta/principalia/vindex.c"
bin="$BUILD_DIR/vindex"
if [ ! -f "$bin" ] || [ "$src" -nt "$bin" ] || [ -n "$(find $obj_files -newer "$bin" 2>/dev/null | head -1)" ]; then
    echo "  [vindex] vindex.c (principale)"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$src" $obj_files -o "$bin" || exit 1
fi

cd "$RADIX_DIR"
exec "$bin" "$@"
