#!/bin/bash
# apps/scriba/scriba.sh - scriba in fenestra: aedilis + struere + exec
#
# Usus: ./apps/scriba/scriba.sh [-fumus] [-volumen <via>]
#       AEDIFICARE_SOLUM=1 ./apps/scriba/scriba.sh  (struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/scriba/scriba.c >&2 || exit 1
bash build/aedilis/scriba/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/scriba \
    && cp build/aedilis/scriba/scriba bin/scriba
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/scriba "$@"
