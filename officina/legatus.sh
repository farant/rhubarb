#!/bin/bash

# officina/legatus.sh - LEGATUS: servus LSP silvae (chunk B)
#
# Usage:
#   ./officina/legatus.sh          # stdio LSP (cliens spawnat)
#
# Aedificatio incrementalis (exemplar colloquium.sh) - garrulitas
# ad stderr SOLUM (stdout purus est protocollo LSP). Aedificatio
# IUDICIS SOLIUS: amalgama silvae + praeparator + legatus - NULLA
# fontes officinae (machinula/regio absunt consulto).

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build/legatus"
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
    "-I$RADIX_DIR/silva/instrumenta"
    "-I$OFF_DIR/instrumenta"
)

declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio" "internamentum" "json" "tabellarius" "similitudo"
    "sigillum" "vigilia"
)
obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
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

# silva.h mutatio formae sine recompilo = corruptio ABI (stalum
# obiectum + amalgama recens - inventum v0.2, SilvaMacroVista)
SILVA_H="$RADIX_DIR/silva/amalgama/silva.h"

src="$RADIX_DIR/silva/instrumenta/nexus_ordines.c"
obj="$BUILD_DIR/nexus_ordines.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$RADIX_DIR/silva/instrumenta/nexus_ordines.h" -nt "$obj" ] \
    || [ "$SILVA_H" -nt "$obj" ]; then
    echo "  [ordines] nexus_ordines.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for unit in praeparator legatus; do
    src="$OFF_DIR/instrumenta/$unit.c"
    obj="$BUILD_DIR/$unit.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
        || [ "$OFF_DIR/instrumenta/$unit.h" -nt "$obj" ] \
        || [ "$SILVA_H" -nt "$obj" ]; then
        echo "  [$unit] $unit.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

BIN="$BUILD_DIR/legatus"
src="$OFF_DIR/instrumenta/principalia/legatus.c"
if [ ! -f "$BIN" ] || [ "$src" -nt "$BIN" ] || [ -n "$obj_files" ]; then
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$src" $obj_files -o "$BIN" || exit 1
fi

# -radix semper appenditur: modus MCP rootUri caret (LSP rootUri
# praevalet cum adest - _radicem_statuere cfg->radix... ordo: cfg
# vincit, ideo LSP quoque radicem rectam accipit)
# vigilia (lib/vigilia): -signum = sigillum binarii ->
# serverInfo.version; -manifestum = clausura quam HIC launcher
# compilat (invarians: aedificator indicem scribit - vigil et
# aedificator dissentire non possunt)
MANIFEST="$BUILD_DIR/legatus.vigilia"
{
    echo "# clausura launcheri (generatum a legatus.sh)"
    for f in "${RADIX_FONTES[@]}"; do echo "$RADIX_DIR/lib/$f.c"; done
    echo "$RADIX_DIR/silva/amalgama/silva.c"
    echo "$SILVA_H"
    echo "$RADIX_DIR/silva/instrumenta/nexus_ordines.c"
    echo "$RADIX_DIR/silva/instrumenta/nexus_ordines.h"
    echo "$OFF_DIR/instrumenta/praeparator.c"
    echo "$OFF_DIR/instrumenta/praeparator.h"
    echo "$OFF_DIR/instrumenta/legatus.c"
    echo "$OFF_DIR/instrumenta/legatus.h"
    echo "$OFF_DIR/instrumenta/principalia/legatus.c"
} > "$MANIFEST" 2>/dev/null
SIGNUM="$(shasum -a 256 "$BIN" 2>/dev/null | cut -c1-64)"
if [ -n "$SIGNUM" ]; then
    cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR" -signum "$SIGNUM" -manifestum "$MANIFEST"
else
    cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR"
fi
