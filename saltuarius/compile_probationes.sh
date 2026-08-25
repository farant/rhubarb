#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/saltuarius.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ratio plena: compile_tests.sh radicis.
if [ -z "${SALTUARIUS_PROBATIONES_EFFUSIO:-}" ]; then
    export SALTUARIUS_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/saltuarius.log"
    exit "${PIPESTATUS[0]}"
fi

# saltuarius/compile_probationes.sh - Probationes saltuarii compilare
# et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh salve      # substring filter
#
# Tessera's template. THE AMALGAM BOUNDARY IS THE API: silva and
# tessera enter this build ONLY as amalgama/*.c objects with
# amalgama include dirs — their fontes/ are deliberately absent
# from INCLUDE_FLAGS (a fontes include is a compile error here;
# that is the pin working). Suite is headless: memoria pons +
# fixture listings; probationes may build real /tmp fixture trees
# only for the iter_directoria integration pledges.

set -u

SALT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SALT_DIR/.." && pwd)"
BUILD_DIR="$SALT_DIR/build"
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
    "-I$RADIX_DIR/tessera/amalgama"
    "-I$SALT_DIR/fontes"
    "-I$SALT_DIR/probationes"
)

# Rhubarb libs saltuarius consumes as a monorepo citizen
# (spec-v2 par 3). friatio is tabula_dispersa's dependency.
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "tabula_dispersa"
    "friatio"
    "filum"
    "via"
    "iter_directoria"
    "utf8"
    "credo"
)

FILTER="${1:-}"

newest_header () {
    find "$RADIX_DIR/include" "$SALT_DIR/fontes" \
        "$RADIX_DIR/silva/amalgama" "$RADIX_DIR/tessera/amalgama" \
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

# ---- 2. the amalgams as objects (tessera now; silva from Phase B) ----
for amalgama in "tessera" "silva"; do
    src="$RADIX_DIR/$amalgama/amalgama/$amalgama.c"
    obj="$BUILD_DIR/amalgama_$amalgama.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [amalgama] $amalgama.c"
        if ! clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: amalgama $amalgama" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 3. saltuarius fontes + proba helpers ----
shopt -s nullglob
for src in "$SALT_DIR"/fontes/*.c "$SALT_DIR"/probationes/saltuarius_proba.c; do
    [ -f "$src" ] || continue
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [saltuarius] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 4. discover, compile, run probationes ----
total=0 ; passed=0 ; failed_names=""
for test_file in "$SALT_DIR"/probationes/probatio_*.c; do
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
echo "SALTUARIUS PROBATIONES: $passed/$total praeteritae"
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi

# NULLA probatio cursa = error operantis (filtrum male
# scriptum), NON successus - exitus II ut radix (01M0XDNTN6;
# contractus 0/1/2, vide compile_tests.sh radicis).
if [ "$total" -eq 0 ]; then
    echo "NULLA PROBATIO CURSA (filtrum sine paribus?) - exitus II"
    exit 2
fi

# canalis excubitoris: verdictum post-constructionem (tacet nisi stala)
"$RADIX_DIR/excubitor.sh" -tacitus "saltuarius/build/" >&2 || true
exit 0
