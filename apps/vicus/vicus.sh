#!/bin/bash
# apps/vicus/vicus.sh - vicus in fenestra: aedilis + struere + exec
#
# Usus: ./apps/vicus/vicus.sh [-fumus] [-volumen <via>]
#       AEDIFICARE_SOLUM=1 ./apps/vicus/vicus.sh  (struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/vicus/vicus.c >&2 || exit 1
bash build/aedilis/vicus/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/vicus \
    && cp build/aedilis/vicus/vicus bin/vicus
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/vicus "$@"
