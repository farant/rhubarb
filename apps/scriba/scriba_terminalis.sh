#!/bin/bash
# apps/scriba/scriba_terminalis.sh - scriba in terminali: aedilis +
# struere + exec, ut scriba.sh
#
# Usus: ./apps/scriba/scriba_terminalis.sh [-fumus] [-volumen <via>]
#       AEDIFICARE_SOLUM=1 ./apps/scriba/scriba_terminalis.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/scriba/scriba_terminalis.c >&2 || exit 1
bash build/aedilis/scriba_terminalis/struere.sh >&2 || exit 1
mkdir -p bin && rm -f bin/scriba_terminalis \
    && cp build/aedilis/scriba_terminalis/scriba_terminalis \
    bin/scriba_terminalis
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/scriba_terminalis "$@"
