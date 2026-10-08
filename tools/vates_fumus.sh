#!/bin/bash
# tools/vates_fumus.sh - fumus vivus vatis (vates-plan-2 T7): vocatio,
# cache lectum in secunda, reditus cogitationis. NON PORTA: API vera
# (centesimae). Usus: ./tools/vates_fumus.sh [-exemplar ID]
#       (AEDIFICARE_SOLUM=1: struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis tools/vates_fumus.c >&2 || exit 1
bash build/aedilis/vates_fumus/struere.sh >&2 || exit 1
[ -n "${AEDIFICARE_SOLUM:-}" ] && exit 0
exec build/aedilis/vates_fumus/vates_fumus "$@"
