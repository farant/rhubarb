#!/bin/bash

# officina/colloquium.sh - COLLOQUIUM: sessio interactiva (M4b)
#
# Usage:
#   ./officina/colloquium.sh [-plagulae a,b,c] [-posix]
#
# Familia oneratorum fontes-lateris (cursor/interpretare): obiecta
# fontium officinae + amalgama silvae + sessio. Currendum ex radice
# repositorii (aut -radix via).

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build/colloquium"
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
    "-I$OFF_DIR/fontes"
    "-I$OFF_DIR/instrumenta"
)

declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio"
)

# capita mutata sine recompilo = corruptio ABI (inventum v0.2;
# EXCUBITOR hanc plagulam ARMATAM nominavit - sessio.o stalum +
# amalgama recens = mixtura corrumpens in proximo cursu)
newest_header () {
    find "$RADIX_DIR/include" "$OFF_DIR/fontes" \
        "$OFF_DIR/instrumenta" "$RADIX_DIR/silva/amalgama" \
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

src="$OFF_DIR/instrumenta/praeparator.c"
obj="$BUILD_DIR/praeparator.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [praeparator] praeparator.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

src="$OFF_DIR/instrumenta/sessio.c"
obj="$BUILD_DIR/sessio.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [sessio] sessio.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

BIN="$BUILD_DIR/colloquium"
src="$OFF_DIR/instrumenta/principalia/colloquium.c"
if [ ! -f "$BIN" ] || [ "$src" -nt "$BIN" ] || [ -n "$obj_files" ]; then
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$src" $obj_files -o "$BIN" || exit 1
fi

# canalis excubitoris: verdictum post-constructionem (tacet nisi stala)
"$RADIX_DIR/excubitor.sh" -tacitus "officina/build/colloquium/" >&2 || true

cd "$RADIX_DIR" && exec "$BIN" "$@"
