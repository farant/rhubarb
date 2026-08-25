#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/gesta.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ratio plena: compile_tests.sh radicis.
if [ -z "${GESTA_PROBATIONES_EFFUSIO:-}" ]; then
    export GESTA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/gesta.log"
    exit "${PIPESTATUS[0]}"
fi

# gesta/compile_probationes.sh - Probationes gestae compilare et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh gesta      # filtrum substringae
#
# Tessera's template. SQLite VENDICATUM compilatur LAXUM in obiecto
# proprio (compile_tests.sh exemplar - numquam sub vexillis domus);
# obiectum in gesta/build/ cachetur.

set -u

GESTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$GESTA_DIR/.." && pwd)"
BUILD_DIR="$GESTA_DIR/build"
mkdir -p "$BUILD_DIR"

# ANCORA: fixturae probationum et scripta daemonum viis radicis
# relativis utuntur - cursus e directorio alio OMNIA a salutatione
# frangit (aspectus catastrophae, causa CWD; mensuratum 2026-07-30)
cd "$RADIX_DIR" || exit 1

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
    "-I$GESTA_DIR/fontes"
    "-I$GESTA_DIR/probationes"
)

# Fontes bibliothecarum radicis quibus gesta nititur - GENERATUM
# AB AEDILE (gesta/fontes_generare.sh regenerat; lista manualis
# prima retirata, aedilis Phasis C 2026-07-20)
source "$GESTA_DIR/fontes_generata.sh"

# SQLite vendicatum - LAXUM, obiectum proprium (exemplar
# compile_tests.sh VENDOR_FLAGS)
declare -a VENDOR_FLAGS=(
    "-O2"
    "-DSQLITE_ENABLE_FTS5"
    "-DSQLITE_THREADSAFE=0"
    "-DSQLITE_DQS=0"
    "-DSQLITE_DEFAULT_MEMSTATUS=0"
    "-DSQLITE_OMIT_LOAD_EXTENSION"
    "-DSQLITE_OMIT_DEPRECATED"
    "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"
)

FILTER="${1:-}"

newest_header () {
    find "$RADIX_DIR/include" "$GESTA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

# ---- 0. vendor sqlite3.o (laxum; source-newer solum) ----
sqlite_obj="$BUILD_DIR/sqlite3.o"
if [ ! -f "$sqlite_obj" ] || [ "$RADIX_DIR/vendor/sqlite3.c" -nt "$sqlite_obj" ]; then
    echo "  [vendor] sqlite3.c (laxum)"
    if ! clang -c "${VENDOR_FLAGS[@]}" "$RADIX_DIR/vendor/sqlite3.c" -o "$sqlite_obj"; then
        echo "FRACTA: vendor/sqlite3.c" ; exit 1
    fi
fi
obj_files="$sqlite_obj"

# ---- 1. obiecta dependentiarum (incrementalia) ----
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

# ---- 2. fontes gestae ----
shopt -s nullglob
for src in "$GESTA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [gesta] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 3. probationes invenire, compilare, currere ----
total=0 ; passed=0 ; failed_names=""
for test_file in "$GESTA_DIR"/probationes/probatio_*.c; do
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
echo "GESTA PROBATIONES: $passed/$total praeteritae"
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
"$RADIX_DIR/excubitor.sh" -tacitus "gesta/build/" >&2 || true
exit 0
