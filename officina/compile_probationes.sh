#!/bin/bash

# officina/compile_probationes.sh - Probationes officinae compilare
# et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh medulla    # substring filter
#
# Saltuarius's template. THE AMALGAM BOUNDARY IS THE API: silva
# enters this build ONLY as amalgama/silva.c object with the
# amalgama include dir — silva/fontes/ is deliberately absent from
# INCLUDE_FLAGS (a fontes include is a compile error here; that is
# the pin working). Medulla itself is silva-free (SilvaNodus tag
# forward-declared); demissio (M1b) is the real silva consumer.

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89"
    "-pedantic"
    "-Wall"
    "-Wextra"
    "-Werror"
    "-Wconversion"
    "-Wsign-conversion"
    "-Wcast-qual"
    "-Wstrict-prototypes"
    "-Wmissing-prototypes"
    "-Wwrite-strings"
    "-Wno-long-long"
    "-Wno-overlength-strings"
    "-fbracket-depth=512"
)

declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$OFF_DIR/fontes"
    "-I$OFF_DIR/probationes"
)

# Rhubarb libs officina consumes as a monorepo citizen
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "credo"
)

FILTER="${1:-}"

newest_header () {
    find "$RADIX_DIR/include" "$OFF_DIR/fontes" \
        "$RADIX_DIR/silva/amalgama" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
}

# ---- 1. dependency objects (incremental) ----
obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $f.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 2. the silva amalgam as object (demissio's substrate) ----
src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] silva.c"
    if ! clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj"; then
        echo "FRACTA: amalgama silva" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# ---- 3. officina fontes ----
shopt -s nullglob
for src in "$OFF_DIR"/fontes/*.c; do
    [ -f "$src" ] || continue
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [officina] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 4. discover, compile, run probationes ----
total=0 ; passed=0 ; failed_names=""
for test_file in "$OFF_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    total=$((total + 1))
    bin="$BUILD_DIR/$name"
    echo ""
    echo "=== $name ==="
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$test_file" $obj_files -o "$bin"; then
        echo "FRACTA (compilatio): $name"
        failed_names="$failed_names $name"
        continue
    fi
    if RHUBARB_RADIX="$RADIX_DIR" "$bin"; then
        passed=$((passed + 1))
    else
        failed_names="$failed_names $name"
    fi
done
shopt -u nullglob

echo ""
echo "========================================"
echo "OFFICINA PROBATIONES: $passed/$total praeteritae"
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi
exit 0
