#!/bin/bash
# apps/pictor/pictor_terminalis.sh - pictor in terminali (modulus 013
# A4): aedilis + struere + exec, ut pictor.sh
#
# Usus: ./apps/pictor/pictor_terminalis.sh [-fumus] [-volumen <via>]
#       AEDIFICARE_SOLUM=1 ./apps/pictor/pictor_terminalis.sh  (struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/pictor/pictor_terminalis.c >&2 || exit 1
bash build/aedilis/pictor_terminalis/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/pictor_terminalis \
    && cp build/aedilis/pictor_terminalis/pictor_terminalis bin/pictor_terminalis
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/pictor_terminalis "$@"
