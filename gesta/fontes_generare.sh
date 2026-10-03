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

# clausurae (fabrica T6): manifestum unum per probationem ->
# build/fabrica/clausurae/gesta/ (ingressus 'manifesta' actionis
# memorabilis); vacuatum ante cursum
clausurae="build/fabrica/clausurae/gesta"
rm -rf "$clausurae"
mkdir -p "$clausurae"

lista="$(
    for p in gesta/probationes/probatio_*.c; do
        ./bin/aedilis "$p" --enumerare --thesaurus build/aedilis/obiecta \
            --manifestum "$clausurae/$(echo "$p" | sed 's|/|__|g').stml" \
            || exit 1
    done | awk '/^lib\/.*\.c$/ { sub(/^lib\//,""); sub(/\.c$/,""); print }' \
         | sort -u
)"
if [ -z "$lista" ]; then
    echo "fontes_generare: derivatio vacua - recusatum" >&2
    exit 1
fi

# FABRICA_SCRIPTURA (iudex fabricae): fragmentum in scripturam, arbor
# intacta
destinatio="gesta/fontes_generata.sh"
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    destinatio="$FABRICA_SCRIPTURA/gesta/fontes_generata.sh"
    mkdir -p "$(dirname "$destinatio")"
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
} > "$destinatio"

echo "gesta/fontes_generata.sh scriptum ($(echo "$lista" | wc -l | tr -d ' ') fontes)"
