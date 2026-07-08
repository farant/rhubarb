#!/bin/bash

# silva/haruspex.sh - oraculum formarum (M0a Chunk D)
#
# Usage:
#   ./silva/haruspex.sh [directoria...] [-v]
#
# Gradus I: haruspex TUs assertionum emittit (semantica contra
# fontes). Gradus II: quaeque TU per clang -fsyntax-only compilatur
# - COMPILATIO EST IUDICIUM. Exitus non-zephyrus = formae nostrae a
# clang dissentiunt (aut TU generata invalida est).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
HX_DIR="$BUILD_DIR/exta"
mkdir -p "$BUILD_DIR"
rm -rf "$HX_DIR"
mkdir -p "$HX_DIR"

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
    "friatio" "internamentum"
)

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

echo "  [haruspex] haruspex.c" >&2
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SILVA_DIR/instrumenta/principalia/haruspex.c" $obj_files \
    -o "$BUILD_DIR/haruspex" || exit 1

cd "$RADIX_DIR"
"$BUILD_DIR/haruspex" "$@" || exit 1

# Gradus II: compilatio est iudicium
declare -a TU_FLAGS=(
    "-I$RADIX_DIR"
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/probationes"
    "-I$RADIX_DIR/book_assets"
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
    "-I$RADIX_DIR/tessera/fontes"
    "-I$RADIX_DIR/saltuarius/fontes"
    "-I$SILVA_DIR/amalgama"
    "-I$RADIX_DIR/tessera/amalgama"
)
fracta=0
summa=0
for tu in "$HX_DIR"/*.c; do
    [ -e "$tu" ] || continue
    summa=$((summa + 1))
    if ! clang "${GCC_FLAGS[@]}" "${TU_FLAGS[@]}" -fsyntax-only "$tu" 2> "$tu.err"; then
        fracta=$((fracta + 1))
        echo "[HARUSPEX DISSENTIT] $tu"
        head -12 "$tu.err"
    fi
done
echo "haruspex: $summa TU compilatae, $fracta dissentientes"
[ "$fracta" -eq 0 ] || exit 1
echo "haruspex: FORMAE VERIFICATAE (clang consentit)"
