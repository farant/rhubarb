#!/bin/bash
# apps/terminale/terminale_terminalis.sh - terminale in terminali:
# aedilis + struere + exec (exemplar apps/scriba/scriba_terminalis.sh)
#
# Usus: ./apps/terminale/terminale_terminalis.sh [-fumus]
#       AEDIFICARE_SOLUM=1 ./apps/terminale/terminale_terminalis.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/terminale/terminale_terminalis.c >&2 || exit 1
bash build/aedilis/terminale_terminalis/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/terminale_terminalis \
    && cp build/aedilis/terminale_terminalis/terminale_terminalis \
    bin/terminale_terminalis
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/terminale_terminalis "$@"
