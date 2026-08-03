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
# (Sole exception: the silva_lexicon.o step passes a private
# -Ifontes on its own clang line - that unit is silva-side code
# needing SILVA_LIMES_POSIX_TITULUS, not officina code.)

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
    "-I$RADIX_DIR/silva/instrumenta"
    "-I$RADIX_DIR/tessera/amalgama"
    "-I$OFF_DIR/fontes"
    "-I$OFF_DIR/instrumenta"
    "-I$OFF_DIR/probationes"
)

# Rhubarb libs officina consumes as a monorepo citizen
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "tabula_dispersa"
    "friatio"
    "credo"
    "internamentum"
    "json"
    "tabellarius"
    "similitudo"
    "sigillum"
    "vigilia"
    "processus_posix"
)

FILTER="${1:-}"

newest_header () {
    find "$RADIX_DIR/include" "$OFF_DIR/fontes" \
        "$RADIX_DIR/silva/amalgama" "$RADIX_DIR/silva/instrumenta" "$RADIX_DIR/silva/fontes" \
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

# ---- 2b. tessera amalgam + vindex visum (M3 chunk 6: probationes
#          cellularum sine capite; obiecta inutilia innocua) ----
src="$RADIX_DIR/tessera/amalgama/tessera.c"
obj="$BUILD_DIR/amalgama_tessera.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] tessera.c"
    if ! clang "${GCC_FLAGS[@]}" "-I$RADIX_DIR/include" -c "$src" -o "$obj"; then
        echo "FRACTA: amalgama tessera" ; exit 1
    fi
fi
obj_files="$obj_files $obj"
src="$OFF_DIR/instrumenta/vindex_visum.c"
obj="$BUILD_DIR/vindex_visum.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$OFF_DIR/instrumenta/vindex_visum.h" -nt "$obj" ]; then
    echo "  [vindex] vindex_visum.c"
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
        -I"$RADIX_DIR/tessera/amalgama" -I"$OFF_DIR/instrumenta" \
        -c "$src" -o "$obj"; then
        echo "FRACTA: vindex_visum" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# ---- 2c-0. nexus_ordines (unitas ordinum communis - legatus) ----
src="$RADIX_DIR/silva/instrumenta/nexus_ordines.c"
obj="$BUILD_DIR/nexus_ordines.o"
# newest_header: silva.h mutatio formae (SilvaMacroVista v0.2!) sine
# ea obiectum vetus + amalgama recens = corruptio ABI in acervo
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$RADIX_DIR/silva/instrumenta/nexus_ordines.h" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [ordines] nexus_ordines.c"
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
        -c "$src" -o "$obj"; then
        echo "FRACTA: nexus_ordines" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# ---- 2c-0b. silva_lexicon (compositio systematis - praeparator) ----
src="$RADIX_DIR/silva/instrumenta/silva_lexicon.c"
obj="$BUILD_DIR/silva_lexicon.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$RADIX_DIR/silva/instrumenta/silva_lexicon.h" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [lexicon] silva_lexicon.c"
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "-I$RADIX_DIR/silva/fontes" \
        -c "$src" -o "$obj"; then
        echo "FRACTA: silva_lexicon" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# ---- 2c. praeparator (unitas oneratorum communis) + sessio ----
src="$OFF_DIR/instrumenta/praeparator.c"
obj="$BUILD_DIR/praeparator.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$OFF_DIR/instrumenta/praeparator.h" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [praeparator] praeparator.c"
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
        -c "$src" -o "$obj"; then
        echo "FRACTA: praeparator" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

src="$OFF_DIR/instrumenta/sessio.c"
obj="$BUILD_DIR/sessio.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$OFF_DIR/instrumenta/sessio.h" -nt "$obj" ] \
    || [ "$OFF_DIR/instrumenta/praeparator.h" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [sessio] sessio.c"
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
        -c "$src" -o "$obj"; then
        echo "FRACTA: sessio" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

src="$OFF_DIR/instrumenta/legatus.c"
obj="$BUILD_DIR/legatus.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$OFF_DIR/instrumenta/legatus.h" -nt "$obj" ] \
    || [ "$OFF_DIR/instrumenta/praeparator.h" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [legatus] legatus.c"
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
        -c "$src" -o "$obj"; then
        echo "FRACTA: legatus" ; exit 1
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

# canalis excubitoris: verdictum post-constructionem (tacet nisi
# stala - obiecta stala POST constructionem = foramen regularum)
"$RADIX_DIR/excubitor.sh" -tacitus "officina/build/" >&2 || true
exit 0
