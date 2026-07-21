#!/bin/bash

# gesta/fontes_generare.sh - RADIX_FONTES ex aedile regenerare
#
# Lista manualis prima retiratur (aedilis Phasis C): clausurae
# probationum gestarum derivantur, obiecta lib/ uniuntur, snippet
# fons-abile gesta/fontes_generata.sh emittitur (COMMITTITUR -
# compile_probationes.sh eum fontat). Post mutationes inclusionum
# gestarum: hoc scriptum currere et snippet novum committere.

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

if [ ! -x bin/aedilis ]; then
    ./tools/aedilis_struere.sh || exit 1
fi

lista="$(
    for p in gesta/probationes/probatio_*.c; do
        ./bin/aedilis "$p" --enumerare || exit 1
    done | awk '/^lib\/.*\.c$/ { sub(/^lib\//,""); sub(/\.c$/,""); print }' \
         | sort -u
)"
if [ -z "$lista" ]; then
    echo "fontes_generare: derivatio vacua - recusatum" >&2
    exit 1
fi

{
    echo "# gesta/fontes_generata.sh - GENERATUM AB AEDILE - NE MANU EDITES"
    echo "# regeneratio: ./gesta/fontes_generare.sh"
    echo "# fons veritatis: clausurae probationum gestarum (bin/aedilis --enumerare)"
    echo "declare -a RADIX_FONTES=("
    echo "$lista" | while IFS= read -r f; do
        echo "    \"$f\""
    done
    echo ")"
} > gesta/fontes_generata.sh

echo "gesta/fontes_generata.sh scriptum ($(echo "$lista" | wc -l | tr -d ' ') fontes)"
