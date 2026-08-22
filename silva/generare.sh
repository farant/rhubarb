#!/bin/bash

# silva/generare.sh - Tabulas generare et coquere (dev-time)
#
# Usage:
#   ./generare.sh                 # OMNES grammaticae commissae
#   ./generare.sh <grammatica.stml> <PRAEFIXUM> <basis_exitus>
#
# Compiles the generator (instrumenta + principalia/generator.c) and
# runs it. Output .{h,c} land in fontes/ and are COMMITTED (they are
# part of the eventual amalgamated silva.c).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

# Viae relativae ad silva/ - commentum GENERATUM viam stabilem fert
cd "$SILVA_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/generare_fontes_generata.sh"

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# CLAUSURA GENERATORIS - index ADMITTENS, non excludens.
#
# Erat index excludens ('salta silva_amalgama, salta nexus_ordines').
# Ter putruit: quotiens instrumentum novum MOTOREM (fontes/) tetigit,
# nexus generatoris tacite fractus est - symbola indefinita LXXXIII
# ad diem 2026-08-22 (silva_differre, silva_formator, silva_iudicium
# post aeram LEGATUS accesserunt, nemine excludente).
#
# Index admittens putrescere NON potest eodem modo: instrumentum
# novum nexum generatoris non intrat nisi quis id HIC nominet, et
# tunc symbola eius consulto ponderantur. Generator instrumenta
# NUMQUAM adhibet praeter haec nominata.
declare -a GENERATOR_FONTES=(
    "silva_generare"        # grammaticam legere, tabulas construere
    "silva_coquere"         # tabulae -> fons C
    "silva_canon_coquere"   # grammatica -> canon STML (arbor M2.3)
)
for base in "${GENERATOR_FONTES[@]}"; do
    src="$SILVA_DIR/instrumenta/$base.c"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$src" ]; then
        echo "  [silva] FONS ABEST: $src" >&2
        exit 1
    fi
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [silva] $base.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# silva_token: FOLIUM motoris (latina/piscina/chorda/xar solum), quod
# nomina generum lexematum tenet - eadem quae canon in tags vertit.
# Sola plagula fontium quam generator nectit; directio non invertitur
# quia folium est, non motor.
for base in "silva_token"; do
    src="$SILVA_DIR/fontes/$base.c"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        echo "  [silva] fontes/$base.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

echo "  [silva] generator.c"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SILVA_DIR/instrumenta/principalia/generator.c" $obj_files \
    -o "$BUILD_DIR/generator" || exit 1

if [ $# -ge 1 ]; then
    "$BUILD_DIR/generator" "${1}" "${2:?PRAEFIXUM deest}" "${3:?BASIS deest}"
else
    # Sine argumentis: OMNES grammaticae commissae regenerantur
    # (post QUAMLIBET mutationem grammaticae - silva/CLAUDE.md)
    "$BUILD_DIR/generator" grammatica/sceletum.stml \
        SILVA_SCELETUM fontes/silva_tabulae_sceleti || exit 1
    "$BUILD_DIR/generator" grammatica/sceletum_imparilis.stml \
        SILVA_IMPARILIS fontes/silva_tabulae_imparilis || exit 1
    # c89 SOLA canonem gignit: sola documenta <parsura> producit
    # (sceletum/imparilis grammaticae probationis sunt)
    "$BUILD_DIR/generator" grammatica/c89.stml \
        SILVA_C89 fontes/silva_tabulae_c89 \
        amalgama/silva.h instrumenta/principalia/hospes.c \
        c89.canon c89 || exit 1
fi
