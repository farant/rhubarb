#!/bin/bash
# tools/tabula_nodorum_generare.sh - tabula nodorum GENERATA
#
# lib/tabula_nodorum_data.c ex extracto KnotInfo fixo
# (probationes/fixa/knotinfo/2026.10.5/nodi_x.tsv: nomina, symmetria,
# codices PD) per tools/tabula_nodorum_generare.c: polynomia Alexander
# et Jones per laqueus computantur.
#
# Usus: ./tools/tabula_nodorum_generare.sh             # scribit
#       ./tools/tabula_nodorum_generare.sh -probare    # recens? 0/1
#       ./tools/tabula_nodorum_generare.sh -collatio <polynomia.tsv>
#           (ab tools/tabula_nodorum_extrahere.sh: polynomia computata
#            == columnae KnotInfo? 0/1)
#
# -probare EXACTA (cmp): plagula commissa = effusio generatoris octetim
# (GENERATUM in linea prima - uncus pre-commit eam non format).
set -u
export LC_ALL=C
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"

FIXUM="probationes/fixa/knotinfo/2026.10.5/nodi_x.tsv"
EXITUS="lib/tabula_nodorum_data.c"
MODUS=scribere
POLYNOMIA=""
case "${1:-}" in
    "") ;;
    -probare) MODUS=probare ;;
    -collatio) MODUS=collatio; POLYNOMIA="${2:-}"
        [ -n "$POLYNOMIA" ] || { echo "usus: -collatio <polynomia.tsv>" >&2; exit 2; } ;;
    *) echo "usus: tabula_nodorum_generare.sh [-probare | -collatio <polynomia.tsv>]" >&2
       exit 2 ;;
esac

source "$RADIX_DIR/tools/vexilla.sh"
mkdir -p build
BIN="build/tabula_nodorum_generare"
clang "${VEXILLA_C89[@]}" -Iinclude tools/tabula_nodorum_generare.c \
    lib/laqueus.c lib/situs.c lib/polynomium.c lib/matrix.c \
    lib/congruentia.c lib/anulus.c lib/fractio.c lib/magnus.c \
    lib/chorda.c lib/chorda_aedificator.c lib/piscina.c \
    -o "$BIN" || exit 2

case "$MODUS" in
collatio)
    exec "$BIN" -collatio "$FIXUM" "$POLYNOMIA" ;;
probare)
    TMP="$(mktemp)"
    trap 'rm -f "$TMP"' EXIT
    "$BIN" "$FIXUM" "$TMP" || exit 2
    if cmp -s "$TMP" "$EXITUS"; then
        echo "tabula_nodorum_generare: $EXITUS recens"
        exit 0
    fi
    echo "tabula_nodorum_generare: $EXITUS RANCIDA (regenera: ./tools/tabula_nodorum_generare.sh)"
    exit 1 ;;
esac
# FABRICA_SCRIPTURA (bin/fabrica -plenus)
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
    mkdir -p "$(dirname "$EXITUS")" || exit 2
fi
exec "$BIN" "$FIXUM" "$EXITUS"
