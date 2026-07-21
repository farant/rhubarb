#!/bin/bash

# silva/nexus.sh - nexus (instrumenta prima #4)
#
# Usage:
#   ./silva/nexus.sh <symbolum> [-omnia]    sedes + usus symboli
#   ./silva/nexus.sh -similis <quaestio>    nomina similia
#   ./silva/nexus.sh -renovare              tabulam regenerare
#                                           (sweep corporis, ~min)
# Exit:  0 inventum | 1 non inventum | 2 tabula deest / usus
#
# Tabula build/nexus.tsv DISPONIBILIS: numquam committitur;
# vetustas contra fontes .c/.h monetur (CAUTIO), non impeditur.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
# MUNDUS AMALGAMATIS (LEGATUS v0.1b): sweep contra amalgama
# aedificatur (fontes numquam); logica ordinum in
# instrumenta/nexus_ordines.{h,c} communis cum legato. Barra
# migrationis: paritas octetim tsv (vide legatus.worklog).
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/amalgama"
    "-I$SILVA_DIR/instrumenta"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/nexus_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
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

# silva.h mutatio formae sine recompilo = corruptio ABI (inventum
# v0.2 in officina; TERTIUM exemplar manu volutum - excubitor chunk 1)
SILVA_H="$SILVA_DIR/amalgama/silva.h"

src="$SILVA_DIR/amalgama/silva.c"
obj="$BUILD_DIR/nexus_amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$SILVA_H" -nt "$obj" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi

src="$SILVA_DIR/instrumenta/nexus_ordines.c"
obj="$BUILD_DIR/nexus_ordines.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ "$SILVA_DIR/instrumenta/nexus_ordines.h" -nt "$obj" ] \
    || [ "$SILVA_H" -nt "$obj" ]; then
    echo "  [ordines] nexus_ordines.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
sweep_objs="$obj_files $BUILD_DIR/nexus_amalgama_silva.o $BUILD_DIR/nexus_ordines.o"

# CLI (tabulam legit - silva non tangit; obiecta bibliothecae sola)
CLI_SRC="$SILVA_DIR/instrumenta/principalia/nexus.c"
CLI_BIN="$BUILD_DIR/nexus"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$CLI_SRC" $obj_files \
    -o "$CLI_BIN" || exit 1

cd "$RADIX_DIR"

# -renovare: sweep corporis (nexus_percursus) - VARIANTE CELERI
# (-O2 -flto, exemplar cursor -celer; 143s -> ~55s mensuratum
# 2026-07-14, PARITAS OCTETIM contra -O0 probata in ambabus
# tabulis). Obiecta celeria in build/celer/ - NUMQUAM mixta cum
# -O0. RUNG 5: modus INCREMENTALIS ordinarius (copia laboris per
# stampam GENERATUM + clausuram reversam graphi; commissio typica
# 1-2s); -plenus vim facit. INSTRUMENTUM NOVUM -> plenus coactum
# (tabula mixta duorum parsatorum numquam nascitur).
if [ "${1:-}" = "-renovare" ]; then
    shift   # args reliqua (-plenus, -omnia) ad sweep transeunt
    CELER_DIR="$BUILD_DIR/celer"
    mkdir -p "$CELER_DIR"
    CELER_FLAGS=("${GCC_FLAGS[@]}" "-O2" "-flto")
    celer_objs=""
    celer_novum=0   # obiectum quodvis recompilatum -> religa
    for f in "${RADIX_FONTES[@]}"; do
        src="$RADIX_DIR/lib/$f.c"
        obj="$CELER_DIR/$f.o"
        if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
            || [ -n "$(newest_header "$obj")" ]; then
            echo "  [celer dep] $f.c" >&2
            clang "${CELER_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
                -c "$src" -o "$obj" || exit 1
            celer_novum=1
        fi
        celer_objs="$celer_objs $obj"
    done
    src="$SILVA_DIR/amalgama/silva.c"
    obj="$CELER_DIR/amalgama_silva.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
        || [ "$SILVA_H" -nt "$obj" ]; then
        echo "  [celer amalgama] silva.c" >&2
        clang "${CELER_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
        celer_novum=1
    fi
    celer_objs="$celer_objs $obj"
    src="$SILVA_DIR/instrumenta/nexus_ordines.c"
    obj="$CELER_DIR/nexus_ordines.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
        || [ "$SILVA_DIR/instrumenta/nexus_ordines.h" -nt "$obj" ] \
        || [ "$SILVA_H" -nt "$obj" ]; then
        echo "  [celer ordines] nexus_ordines.c" >&2
        clang "${CELER_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
            -c "$src" -o "$obj" || exit 1
        celer_novum=1
    fi
    celer_objs="$celer_objs $obj"
    SWEEP_SRC="$SILVA_DIR/instrumenta/principalia/nexus_percursus.c"
    SWEEP_BIN="$CELER_DIR/nexus_percursus"
    plenus_vis=""
    if [ "$celer_novum" = "1" ] || [ ! -f "$SWEEP_BIN" ] \
        || [ "$SWEEP_SRC" -nt "$SWEEP_BIN" ] \
        || [ -n "$(newest_header "$SWEEP_BIN")" ] \
        || [ "$SILVA_H" -nt "$SWEEP_BIN" ]; then
        echo "  [celer percursus] nexus_percursus.c" >&2
        clang "${CELER_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SWEEP_SRC" \
            $celer_objs -o "$SWEEP_BIN" || exit 1
        # instrumentum novum: iudicia mutari potuerunt -> plenus
        # (numquam tacite: nota infra)
        plenus_vis="-plenus"
        echo "  [instrumentum novum -> plenus]" >&2
    fi
    mkdir -p "$RADIX_DIR/build"
    # $plenus_vis SINE virgulis consulto (vacuum evanescit - bash
    # scissio verborum; tabula vacua sub set -u in bash 3.2 fallit)
    exec "$SWEEP_BIN" $plenus_vis "$@"
fi

# cautio vetustatis: tabula senior fonte aliquo recentiore
TSV="$RADIX_DIR/build/nexus.tsv"
if [ -f "$TSV" ]; then
    RECENTIOR=$(find "$RADIX_DIR" -name '*.c' -newer "$TSV" \
        -not -path '*/build/*' -not -path '*/.git/*' 2>/dev/null | head -1)
    if [ -n "$RECENTIOR" ]; then
        echo "CAUTIO: nexus.tsv vetustior quam $RECENTIOR" >&2
        echo "        (renovare: ./silva/nexus.sh -renovare)" >&2
    fi
fi

exec "$CLI_BIN" "$@"
