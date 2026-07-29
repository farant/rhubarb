#!/bin/bash

# gesta/frigida.sh - via scripturae frigida (residens absens):
# nota_frigida construit et currit. Usus:
#   ./gesta/frigida.sh <res|titulus> <textus...>
#   ./gesta/frigida.sh -crea <genus> <titulus> [textus]

set -u

GESTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$GESTA_DIR/.." && pwd)"
BUILD_DIR="$GESTA_DIR/build"
BIN="$BUILD_DIR/nota_frigida"
mkdir -p "$BUILD_DIR" "$GESTA_DIR/annales"

# obiecta per launcher servi aedificantur (eadem vexilla; </dev/null
# = servus statim EOF exit 0 post constructionem, scrinium intactum)
"$GESTA_DIR/tabularium.sh" </dev/null >/dev/null 2>&1 || true

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
    "-fbracket-depth=512"
)

main_src="$GESTA_DIR/instrumenta/nota_frigida.c"
obj_files="$BUILD_DIR/sqlite3.o"
for o in piscina chorda chorda_aedificator xar friatio tabula_dispersa internamentum utf8 json similitudo sigillum scrinium moneta tabellarius vigilia filum via iter_directoria gesta tabularium; do
    obj_files="$obj_files $BUILD_DIR/$o.o"
done

obj_recentius=""
for o in $obj_files; do
    if [ "$o" -nt "$BIN" ]; then obj_recentius="$o"; break; fi
done
if [ ! -f "$BIN" ] || [ "$main_src" -nt "$BIN" ] || [ -n "$obj_recentius" ]; then
    echo "  [nexus] nota_frigida" >&2
    clang "${GCC_FLAGS[@]}" -I"$RADIX_DIR/include" -I"$GESTA_DIR/fontes" "$main_src" $obj_files -o "$BIN" >&2 || exit 1
fi

cd "$RADIX_DIR" && exec "$BIN" "$@"
