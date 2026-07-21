#!/bin/bash

# officina/interpretare.sh - functionem corporis in machinula currere
# (instrumenta prima #5; cursor.sh exemplar)
#
# Usage:
#   ./officina/cursor.sh            # ex radice repositorii
#   ./officina/cursor.sh -celer ... # variante VELOCI (-O2 + LTO,
#       ~3.6x mensuratum; obiecta in build/celer/ - numquam mixta
#       cum -O0!) - pro cursibus benedictionis lapifex-classis.
#       Aedificatio -O0 norma manet (celeritas compilandi +
#       debugabilitas).

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build"

CELER=0
if [ "${1:-}" = "-celer" ]; then
    CELER=1
    shift
    BUILD_DIR="$OFF_DIR/build/celer"
fi
mkdir -p "$BUILD_DIR"

# -derivare (EXPERIMENTALE, aedilis Phasis C): clausuram functionis
# per aedilem derivare et ut -plagulae tradere (mundus angustus pro
# toto). Usus: ./officina/interpretare.sh [-celer] -derivare
# <functio> [argumenta...]. Sedes ex build/nexus.tsv; conventio
# include/X.h -> lib/X.c; irregularia -> recusatio (da -plagulae
# manu). Filtrum substring: "<basis>.c" exacte congruit.
DERIVATA_PLAGULAE=""
if [ "${1:-}" = "-derivare" ]; then
    shift
    functio="${1:?functio requiritur post -derivare}"
    if [ ! -f "$RADIX_DIR/build/nexus.tsv" ]; then
        echo "derivare: build/nexus.tsv deest" \
            "(./silva/nexus.sh -renovare)" >&2
        exit 1
    fi
    sedes="$(awk -F'\t' -v f="$functio" \
        '$1==f && $2=="sedes" && $3=="functio" {print $4; exit}' \
        "$RADIX_DIR/build/nexus.tsv")"
    if [ -z "$sedes" ]; then
        echo "derivare: functio '$functio' in nexu non inventa" >&2
        exit 1
    fi
    case "$sedes" in
        include/*.h) fons="lib/$(basename "$sedes" .h).c" ;;
        lib/*.c)     fons="$sedes" ;;
        *)
            echo "derivare: sedes irregularis '$sedes'" \
                "- da -plagulae manu" >&2
            exit 1 ;;
    esac
    if [ ! -f "$RADIX_DIR/$fons" ]; then
        echo "derivare: fons '$fons' abest (caput irregulare?)" \
            "- da -plagulae manu" >&2
        exit 1
    fi
    if [ ! -x "$RADIX_DIR/bin/aedilis" ]; then
        "$RADIX_DIR/tools/aedilis_struere.sh" || exit 1
    fi
    DERIVATA_PLAGULAE="$( (cd "$RADIX_DIR" \
        && { echo "$fons"; ./bin/aedilis "$fons" --enumerare; }) \
        | awk '/^lib\/.*\.c$/ {n=split($0,a,"/"); print a[n]}' \
        | sort -u | paste -sd, -)"
    if [ -z "$DERIVATA_PLAGULAE" ]; then
        echo "derivare: clausura vacua - recusatum" >&2
        exit 1
    fi
    echo "  [derivata] -plagulae $DERIVATA_PLAGULAE" >&2
fi

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
if [ $CELER -eq 1 ]; then
    GCC_FLAGS+=("-O2" "-flto")
fi
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$OFF_DIR/fontes"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$OFF_DIR/interpretare_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$OFF_DIR/fontes" \
        "$RADIX_DIR/silva/amalgama" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
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

src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for src in "$OFF_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [officina] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# Semper renectere (obiecta mutata aliter in binario vetusto latent)
INTERPRETARE_SRC="$OFF_DIR/instrumenta/principalia/interpretare.c"
INTERPRETARE_BIN="$BUILD_DIR/interpretare"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$INTERPRETARE_SRC" $obj_files \
    -o "$INTERPRETARE_BIN" || exit 1

cd "$RADIX_DIR"
if [ -n "$DERIVATA_PLAGULAE" ]; then
    exec "$INTERPRETARE_BIN" -plagulae "$DERIVATA_PLAGULAE" "$@"
fi
exec "$INTERPRETARE_BIN" "$@"
