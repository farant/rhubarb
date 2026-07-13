#!/bin/bash

# silva/nexus.sh - nexus (instrumenta prima #4)
#
# Usage:
#   ./silva/nexus.sh <symbolum> [-omnia]    sedes + usus symboli
#   ./silva/nexus.sh -similis <quaestio>    nomina similia
#   ./silva/nexus.sh -renovare              tabulam regenerare
#                                           (sweep corporis, ~min)
# Exit:  0 inventum | 1 non inventum | 2 tabula deest / usus
#
# Tabula build/nexus.tsv DISPONIBILIS: numquam committitur;
# vetustas contra fontes .c/.h monetur (CAUTIO), non impeditur.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
# MUNDUS AMALGAMATIS (LEGATUS v0.1b): sweep contra amalgama
# aedificatur (fontes numquam); logica ordinum in
# instrumenta/nexus_ordines.{h,c} communis cum legato. Barra
# migrationis: paritas octetim tsv (vide legatus.worklog).
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/amalgama"
    "-I$SILVA_DIR/instrumenta"
)
declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio" "internamentum" "similitudo"
)

newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
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

src="$SILVA_DIR/amalgama/silva.c"
obj="$BUILD_DIR/nexus_amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi

src="$SILVA_DIR/instrumenta/nexus_ordines.c"
obj="$BUILD_DIR/nexus_ordines.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$SILVA_DIR/instrumenta/nexus_ordines.h" -nt "$obj" ]; then
    echo "  [ordines] nexus_ordines.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
sweep_objs="$obj_files $BUILD_DIR/nexus_amalgama_silva.o $BUILD_DIR/nexus_ordines.o"

# CLI (tabulam legit - silva non tangit; obiecta bibliothecae sola)
CLI_SRC="$SILVA_DIR/instrumenta/principalia/nexus.c"
CLI_BIN="$BUILD_DIR/nexus"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$CLI_SRC" $obj_files \
    -o "$CLI_BIN" || exit 1

cd "$RADIX_DIR"

# -renovare: sweep corporis (nexus_percursus)
if [ "${1:-}" = "-renovare" ]; then
    SWEEP_SRC="$SILVA_DIR/instrumenta/principalia/nexus_percursus.c"
    SWEEP_BIN="$BUILD_DIR/nexus_percursus"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SWEEP_SRC" $sweep_objs \
        -o "$SWEEP_BIN" || exit 1
    mkdir -p "$RADIX_DIR/build"
    exec "$SWEEP_BIN"
fi

# cautio vetustatis: tabula senior fonte aliquo recentiore
TSV="$RADIX_DIR/build/nexus.tsv"
if [ -f "$TSV" ]; then
    RECENTIOR=$(find "$RADIX_DIR" -name '*.c' -newer "$TSV" \
        -not -path '*/build/*' -not -path '*/.git/*' 2>/dev/null | head -1)
    if [ -n "$RECENTIOR" ]; then
        echo "CAUTIO: nexus.tsv vetustior quam $RECENTIOR" >&2
        echo "        (renovare: ./silva/nexus.sh -renovare)" >&2
    fi
fi

exec "$CLI_BIN" "$@"
