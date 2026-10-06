#!/bin/bash
# apps/vicus/vicus_terminalis.sh - vicus in terminali: aedilis +
# struere + exec, ut vicus.sh
#
# Usus: ./apps/vicus/vicus_terminalis.sh [-fumus] [-volumen <via>]
#       AEDIFICARE_SOLUM=1 ./apps/vicus/vicus_terminalis.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/vicus/vicus_terminalis.c >&2 || exit 1
bash build/aedilis/vicus_terminalis/struere.sh >&2 || exit 1
mkdir -p bin && rm -f bin/vicus_terminalis \
    && cp build/aedilis/vicus_terminalis/vicus_terminalis \
    bin/vicus_terminalis
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/vicus_terminalis "$@"
