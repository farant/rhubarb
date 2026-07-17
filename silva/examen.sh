#!/bin/bash

# silva/examen.sh - effusor iudicii C89 (examen, M4a chunk D)
#
# Usage:
#   ./silva/examen.sh <plagula.c> [-posix|-nudum] [-machina] [-v]
# Exit:  0 ACCIPE | 1 REICE | 2 INFRA (apparatus)
#
# NB: semper ex radice repositorii currit (cd infra) - viae datae
# relativae ad radicem sunto.

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
)
declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio" "internamentum" "vigilia" "sigillum"
)

# Obiecta vetusta capitibus novis = valores enum falsi (exemplar
# percursus.sh; obiecta communia in silva/build/ conpartita)
newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
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

EXAMEN_SRC="$SILVA_DIR/instrumenta/principalia/examen.c"
EXAMEN_BIN="$BUILD_DIR/examen"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$EXAMEN_SRC" $obj_files \
    -o "$EXAMEN_BIN" || exit 1

# manifestum vigiliae (invarians: aedificator indicem VERUM scribit -
# binarium directe vocatum contra hunc fontes suos vigilat)
{
    for f in "${RADIX_FONTES[@]}"; do echo "$RADIX_DIR/lib/$f.c"; done
    ls "$SILVA_DIR"/fontes/*.c "$SILVA_DIR"/fontes/*.h
    echo "$EXAMEN_SRC"
    ls "$RADIX_DIR"/include/*.h
} > "$BUILD_DIR/examen.manifestum"

cd "$RADIX_DIR"
exec "$EXAMEN_BIN" "$@"
