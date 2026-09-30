#!/bin/bash
# tools/fabrica_struere.sh - bin/fabrica struit (bootstrap; plan 1a T3)
#
# Mos make.bash Go: scriptum SEQUENTIAM tenet, numquam scientiam -
# aedilis clausuram tools/fabrica.c derivat, scriptum ab eo genitum
# compilat. Ergo bin/fabrica ante omnia struendum est quod eo utitur.
#
# Usus: ./tools/fabrica_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >/dev/null || {
    echo "fabrica: aedilis non structus"; exit 1; }
bin/aedilis tools/fabrica.c >/dev/null || {
    echo "fabrica: aedilis fracta"; exit 1; }
sh build/aedilis/fabrica/struere.sh >/dev/null || {
    echo "fabrica: structura fracta"; exit 1; }
# SIGNATURA: rm ante cp (macOS signaturam per vnode condit - vide
# tools/manus_struere.sh, mensuratum 2026-08-13)
rm -f bin/fabrica
cp build/aedilis/fabrica/fabrica bin/fabrica || exit 1
echo "FABRICA STRUCTA: bin/fabrica"
