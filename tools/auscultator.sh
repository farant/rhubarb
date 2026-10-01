#!/bin/bash
# tools/auscultator.sh - auscultator: eventus fenestrae ut STML (eventus A3)
# Usus: ./tools/auscultator.sh   (AEDIFICARE_SOLUM=1: struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis tools/auscultator.c >&2 || exit 1
bash build/aedilis/auscultator/struere.sh >&2 || exit 1
[ -n "${AEDIFICARE_SOLUM:-}" ] && exit 0
exec build/aedilis/auscultator/auscultator
