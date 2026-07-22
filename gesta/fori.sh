#!/bin/bash

# gesta/fori.sh - launcher proxyis MCP fori (F1 forum)
# (exemplar tabulariumd.sh SINE vigilia - proxy tenuis machinam non
# fert; garrulitas stderr SOLA - stdout protocollum purum)
#
# Registratio (.mcp.json): {"fori": {"command": "./gesta/fori.sh",
# "args": ["-mcp"]}}

set -u

GESTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$GESTA_DIR/.." && pwd)"
BUILD_DIR="$GESTA_DIR/build"
BIN="$BUILD_DIR/fori"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
    "-fbracket-depth=512"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include" "-I$GESTA_DIR/fontes"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$GESTA_DIR/fori_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

# ── clausura vera inclusionum (exemplar tabularium.sh) ──────────
FONTES_OMNES=()
for f in "${RADIX_FONTES[@]}"; do FONTES_OMNES+=("$RADIX_DIR/lib/$f.c"); done
FONTES_OMNES+=("$GESTA_DIR/instrumenta/fori_principale.c")

DEPENDENTIAE="$(clang -MM "${INCLUDE_FLAGS[@]}" "${FONTES_OMNES[@]}" 2>/dev/null \
    | awk '{ if (sub(/\\$/,"")) printf "%s ", $0; else print }')"

deps_obiecti () {
    printf '%s\n' "$DEPENDENTIAE" | grep "^$1\.o:" | tr ' ' '\n' | grep '^/'
}
recentius_ex () {
    while IFS= read -r via; do
        [ -n "$via" ] && [ "$via" -nt "$1" ] && { echo "$via"; return 0; }
    done
    return 0
}
fons_stalus () {
    if [ -n "$DEPENDENTIAE" ]; then
        [ -n "$(deps_obiecti "$1" | recentius_ex "$2")" ]
    else
        [ -n "$(newest_header "$2")" ]
    fi
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus "$f" "$obj"; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" >&2 || exit 1
    fi
    obj_files="$obj_files $obj"
done

main_src="$GESTA_DIR/instrumenta/fori_principale.c"
obj_recentius=""
for o in $obj_files; do
    if [ "$o" -nt "$BIN" ]; then obj_recentius="$o"; break; fi
done
if [ ! -f "$BIN" ] || [ "$main_src" -nt "$BIN" ] || [ -n "$obj_recentius" ] || fons_stalus fori_principale "$BIN"; then
    echo "  [nexus] fori" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$main_src" $obj_files -o "$BIN" >&2 || exit 1
fi

# -struere: aedifica et exi (semen fumi + praestructio)
if [ "${1:-}" = "-struere" ]; then
    exit 0
fi

# cd radix: start-if-absent intra proxy ./gesta/tabulariumd.sh vocat
cd "$RADIX_DIR" && exec "$BIN" "$@"
