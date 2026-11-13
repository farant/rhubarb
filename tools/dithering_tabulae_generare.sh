#!/bin/bash
# tools/dithering_tabulae_generare.sh - tabulae exemplariorum GENERATAE
#
# lib/dithering_exemplaria_tabulae.c ex fontibus communibus
# (lib/dithering_exemplaria_fontes.c) per
# tools/dithering_tabulae_generare.c (aedilis struit: color.h ->
# thema). Pondera eodem modo ac classicus computata (exemplaria-velox
# T2): velox ex tabulis == classicus octetim.
#
# Usus: ./tools/dithering_tabulae_generare.sh            # scribit
#       ./tools/dithering_tabulae_generare.sh -probare   # recens? 0/1 (2 fractum)
#
# -probare EXACTA (cmp): plagula commissa = effusio generatoris octetim
# (GENERATUM in linea prima - uncus pre-commit eam non format).
set -u
export LC_ALL=C
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"

EXITUS="lib/dithering_exemplaria_tabulae.c"
PROBARE=0
for arg in "$@"; do
    case "$arg" in
        -probare) PROBARE=1 ;;
        *) echo "usus: dithering_tabulae_generare.sh [-probare]" >&2
           exit 2 ;;
    esac
done

if [ ! -x bin/aedilis ]; then
    echo "dithering_tabulae_generare: bin/aedilis abest - strue: ./tools/aedilis_struere.sh" >&2
    exit 2
fi
# Obiecta fontium quos tabula reddit SEMPER recompilantur: struere.sh
# per '-nt' (secundae integrae) iudicat, ergo fons mutatus et restitutus
# intra secundam unam obiectum vetus servaret (memoria plant-restore-
# mtime) - tabula de fonte alio iudicaretur.
rm -f build/aedilis/obiecta/tools__dithering_tabulae_generare.o \
    build/aedilis/obiecta/lib__dithering_exemplaria_fontes.o
bin/aedilis tools/dithering_tabulae_generare.c >/dev/null \
    || { echo "dithering_tabulae_generare: aedilis fracta" >&2; exit 2; }
sh build/aedilis/dithering_tabulae_generare/struere.sh >/dev/null \
    || { echo "dithering_tabulae_generare: structura fracta" >&2; exit 2; }
BIN="build/aedilis/dithering_tabulae_generare/dithering_tabulae_generare"

if [ "$PROBARE" = 1 ]; then
    TMP="$(mktemp)"
    trap 'rm -f "$TMP"' EXIT
    "$BIN" "$TMP" || exit 2
    if cmp -s "$TMP" "$EXITUS"; then
        echo "dithering_tabulae_generare: $EXITUS recens"
        exit 0
    fi
    echo "dithering_tabulae_generare: $EXITUS RANCIDA (regenera: ./tools/dithering_tabulae_generare.sh)"
    exit 1
fi
# FABRICA_SCRIPTURA (bin/fabrica -plenus)
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
    mkdir -p "$(dirname "$EXITUS")" || exit 2
fi
"$BIN" "$EXITUS"
