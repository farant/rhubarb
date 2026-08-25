#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/silva.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ratio plena: compile_tests.sh radicis.
if [ -z "${SILVA_PROBATIONES_EFFUSIO:-}" ]; then
    export SILVA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/silva.log"
    exit "${PIPESTATUS[0]}"
fi

# silva/compile_probationes.sh - Probationes silvae compilare et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh lexema     # probationes quarum titulus "lexema" continet
#
# Mirrors the repo-root compile_tests.sh pattern (glob discovery, incremental
# objects, substring filter) but self-contained to silva/ and without the
# Cocoa/Security frameworks — silva is a pure library.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
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
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
    "-I$SILVA_DIR/probationes"
)

# Rhubarb lib sources silva depends on during development (vendoring happens
# only at amalgamation). credo is the test framework. stml + internamentum
# are ENGINE deps since silva_annotationes (2026-07-21, vendored at
# amalgamation like the rest); the generator (instrumenta) also uses stml.
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "tabula_dispersa"
    "friatio"
    "internamentum"
    "selectio"
    "stml"
    "credo"
    "differentia"
    "sigillum"
    # canon + similitudo: probatio_silva_canon canonem proiectum
    # (silva/c89.canon) contra canon.canon iudicat - schema
    # schematum schema generatum iudicans (arbor M2.3 / T5)
    "canon"
    "similitudo"
)

FILTER="${1:-}"

# ---- 1. compile dependency objects (incremental) ----
newest_header () {
    # newest mtime among include headers + silva headers
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

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

# ---- 2. compile silva's own sources (fontes/*.c + instrumenta/*.c) ----
shopt -s nullglob
for src in "$SILVA_DIR"/fontes/*.c "$SILVA_DIR"/instrumenta/*.c; do
    base="$(basename "$src" .c)"
    # nexus_ordines est mundi AMALGAMATIS (silva.h includit; communis
    # cum officina/legatus) — contra capita fontium non compilat.
    if [ "$base" = "nexus_ordines" ]; then continue; fi
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 2b. adiumenta probationum (probationes/*.c non probatio_*) ----
# Porta apparatus BIS incendit et utroque numerum peperit qui datum
# simulabat; exemplar eius SECUNDUM in probatione altera tertiam
# occasionem erroris INDEPENDENTIS daret. Ergo unum exemplar hic
# compilatur et in probationes omnes nectitur.
for src in "$SILVA_DIR"/probationes/*.c; do
    base="$(basename "$src" .c)"
    case "$base" in probatio_*) continue ;; esac
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [adiumentum] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 3. discover, compile, run probationes ----
total=0 ; passed=0 ; failed_names=""
for test_file in "$SILVA_DIR"/probationes/probatio_*.c; do
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
echo "SILVA PROBATIONES: $passed/$total praeteritae"
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi

# canalis excubitoris: verdictum post-constructionem (tacet nisi stala)
"$RADIX_DIR/excubitor.sh" -tacitus "silva/build/" >&2 || true
exit 0
