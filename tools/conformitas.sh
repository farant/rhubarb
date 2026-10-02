#!/bin/bash
# tools/conformitas.sh - conformitas: tabula conformitatis contra fenestram (eventus A4)
# Usus: ./tools/conformitas.sh [tabula.stml]   (AEDIFICARE_SOLUM=1: struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis tools/conformitas.c >&2 || exit 1
bash build/aedilis/conformitas/struere.sh >&2 || exit 1
[ -n "${AEDIFICARE_SOLUM:-}" ] && exit 0
exec build/aedilis/conformitas/conformitas "$@"
