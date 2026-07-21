#!/bin/bash

# silva/auspex.sh - certificatio conversionum arithmeticarum (M0b B)
#
# Usage:
#   ./silva/auspex.sh
#
# Gradus I: auspex probam per silvam typat et TU assertionum emittit
# (eaedem expressiones verbatim). Gradus II: TU per clang
# -fsyntax-only compilatur - COMPILATIO EST IUDICIUM. TU ut C99:
# extensio longus longus ordines C99 sequitur (INTENTIO DECISUS 4) -
# ipsa electio certificatur.

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
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/auspex_fontes_generata.sh"

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
        echo "  [fons] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

echo "  [auspex] auspex.c" >&2
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SILVA_DIR/instrumenta/principalia/auspex.c" $obj_files \
    -o "$BUILD_DIR/auspex" || exit 1

cd "$RADIX_DIR"
"$BUILD_DIR/auspex" || exit 1

# Gradus II: compilatio est iudicium (C99 - vide caput)
# -Wno-constant-logical-operand: "(T)1 && (T)1" consulto constans
# -Wno-sizeof-array-decay: "sizeof(ga + 1)" - lapsus IPSE mensuratur
if ! clang -std=c99 -pedantic -Wall -Wextra -Werror \
        -Wno-constant-logical-operand \
        -Wno-sizeof-array-decay \
        -fsyntax-only "$BUILD_DIR/auspicium.c"; then
    echo "auspex: CLANG DISSENTIT"
    exit 1
fi
echo "auspex: CONVERSIONES VERIFICATAE (clang consentit, 0 dissentientes)"
