#!/bin/bash
# tools/iudex_unitatum.sh - filtrum iudicis unitatum pro scriptis
# crustae (formator commissionis, lint Latinus, unci): quas viarum
# iudex TITULUS (unitates.stml <iudex>) iudicat?
#
# Usage:  ./tools/iudex_unitatum.sh [-causae] TITULUS [VIA...]   (sine viis: stdin)
#         -causae: via NON iudicata in stderr cum causa sua
# Exit:   0 responsum (viae iudicatae, linea una) | 2 usus, iudex
#         ignotus, recusatio unitatum aut aedificatio fracta
#
# INSTRUMENTUM SE STRUIT (exemplar crusta/facies.sh): clausura sua ab
# aedile, obiecta per bin/compilator, nexus solum si obiectum mutatum -
# arbor recens (bin/ non commissum) eum habet sine bin/fabrica. Causae
# (cur via NON iudicatur): bin/fabrica quid -iudex TITULUS VIA.
set -u
RADIX_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/iudex_unitatum"
BIN="$BUILD_DIR/iudex_unitatum"
SRC="$RADIX_DIR/tools/iudex_unitatum.c"
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || {
    echo "iudex_unitatum: aedificatio fracta" >&2; exit 2; }
cd "$RADIX_DIR" || exit 2
exec "$BIN" "$@"
