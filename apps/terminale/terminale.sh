#!/bin/bash
# apps/terminale/terminale.sh - terminale in fenestra: aedilis + struere
# + exec (exemplar apps/scriba/scriba.sh)
#
# Usus: ./apps/terminale/terminale.sh [-fumus] [-imago <via>]
#       AEDIFICARE_SOLUM=1 ./apps/terminale/terminale.sh  (sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/terminale/terminale.c >&2 || exit 1
bash build/aedilis/terminale/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/terminale \
    && cp build/aedilis/terminale/terminale bin/terminale
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/terminale "$@"
