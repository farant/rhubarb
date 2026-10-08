#!/bin/bash
# apps/terminale/terminale.sh - terminale in fenestra: struere per
# tools/terminale_struere.sh (actio fabricae 'terminale': aedilis,
# provenientia, struere, bin/terminale) + exec
#
# Usus: ./apps/terminale/terminale.sh [-fumus] [-imago <via>]
#       AEDIFICARE_SOLUM=1 ./apps/terminale/terminale.sh  (sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./tools/terminale_struere.sh >&2 || exit 1
if [ -n "${AEDIFICARE_SOLUM:-}" ]; then
    exit 0
fi
exec bin/terminale "$@"
