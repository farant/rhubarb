#!/bin/bash

# silva/quaestio.sh - quaestio (bibliotheca selectorum nominatorum)
#
# Usage:
#   ./silva/quaestio.sh                          # index bibliothecae
#   ./silva/quaestio.sh <plagula> <nomen> [param=valor ...]
#   ./silva/quaestio.sh -invariantia [radix]     # percursus (oculi)
#   ./silva/quaestio.sh -omnibus <nomen> [param=valor ...] [radix]
#                                    # quaestio super arborem totam
#   ./silva/quaestio.sh -clausura <fons.c> <nomen> [param=valor ...]
#                                    # super clausuram aedilis (fons
#                                    # + ordines O de --partes)
#   ./silva/quaestio.sh -proba                   # porta nativa
# Flags: -omnia (tectum mensurae sublatum)
# Exit:  0 congruentia/TENENT | 1 nulla/VIOLATUR | 2 fractura
#
# Bibliotheca: silva/quaestiones.stml (semper ex radice repositorii
# currit - cd infra).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/quaestio_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
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

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

QUAESTIO_SRC="$SILVA_DIR/instrumenta/principalia/quaestio.c"
QUAESTIO_BIN="$BUILD_DIR/quaestio"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$QUAESTIO_SRC" $obj_files \
    -o "$QUAESTIO_BIN" || exit 1

cd "$RADIX_DIR"

# -clausura: clausuram aedilis computare, per -lista alimentare
if [ "${1:-}" = "-clausura" ]; then
    FONS="${2:?quaestio -clausura: fons.c deest}"
    shift 2
    if [ ! -x bin/aedilis ]; then
        ./tools/aedilis_struere.sh || exit 2
    fi
    { echo "$FONS"; bin/aedilis "$FONS" --partes \
        | awk -F'\t' '$1=="O"{print $2}'; } | sort -u \
        | "$QUAESTIO_BIN" -lista "$@"
    exit $?
fi

exec "$QUAESTIO_BIN" "$@"
