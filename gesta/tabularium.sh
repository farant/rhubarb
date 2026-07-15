#!/bin/bash

# gesta/tabularium.sh - launcher servi MCP tabularii
# (exemplar officina/legatus.sh: garrulitas stderr SOLA - stdout
# protocollum purum; constructio mtime-podata; -radix SEMPER
# appenditur - MCP rootUri caret)
#
# Registratio (.mcp.json): {"tabularii": {"command":
# "./gesta/tabularium.sh", "args": ["-mcp"]}}
# Praestruere semel: ./gesta/tabularium.sh </dev/null

set -u

GESTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$GESTA_DIR/.." && pwd)"
BUILD_DIR="$GESTA_DIR/build"
BIN="$BUILD_DIR/tabularium"
mkdir -p "$BUILD_DIR" "$GESTA_DIR/annales"

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
declare -a RADIX_FONTES=(
    piscina chorda chorda_aedificator xar friatio tabula_dispersa
    internamentum utf8 json similitudo sigillum scrinium
    tabellarius
)
declare -a VENDOR_FLAGS=(
    "-O2" "-DSQLITE_ENABLE_FTS5" "-DSQLITE_THREADSAFE=0"
    "-DSQLITE_DQS=0" "-DSQLITE_DEFAULT_MEMSTATUS=0"
    "-DSQLITE_OMIT_LOAD_EXTENSION" "-DSQLITE_OMIT_DEPRECATED"
    "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"
)

newest_header () {
    find "$RADIX_DIR/include" "$GESTA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

sqlite_obj="$BUILD_DIR/sqlite3.o"
if [ ! -f "$sqlite_obj" ] || [ "$RADIX_DIR/vendor/sqlite3.c" -nt "$sqlite_obj" ]; then
    echo "  [vendor] sqlite3.c (laxum)" >&2
    clang -c "${VENDOR_FLAGS[@]}" "$RADIX_DIR/vendor/sqlite3.c" -o "$sqlite_obj" >&2 || exit 1
fi
obj_files="$sqlite_obj"

for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" >&2 || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$GESTA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [gesta] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" >&2 || exit 1
    fi
    obj_files="$obj_files $obj"
done

main_src="$GESTA_DIR/instrumenta/tabularium_principale.c"
# obiectum quodvis recentius binario -> renexus (lectio excubitoris:
# conditio capitum sola obiecta recompilata non videt)
obj_recentius=""
for o in $obj_files; do
    if [ "$o" -nt "$BIN" ]; then obj_recentius="$o"; break; fi
done
if [ ! -f "$BIN" ] || [ "$main_src" -nt "$BIN" ] || [ -n "$obj_recentius" ] || [ -n "$(newest_header "$BIN")" ]; then
    echo "  [nexus] tabularium" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$main_src" $obj_files -o "$BIN" >&2 || exit 1
fi

cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR"
