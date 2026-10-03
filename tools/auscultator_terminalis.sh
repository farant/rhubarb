#!/bin/bash
# tools/auscultator_terminalis.sh - eventus terminalis ut STML (eventus B3b-iii)
# Usus: ./tools/auscultator_terminalis.sh   (AEDIFICARE_SOLUM=1: struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis tools/auscultator_terminalis.c >&2 || exit 1
bash build/aedilis/auscultator_terminalis/struere.sh >&2 || exit 1
[ -n "${AEDIFICARE_SOLUM:-}" ] && exit 0
exec build/aedilis/auscultator_terminalis/auscultator_terminalis "$@"
