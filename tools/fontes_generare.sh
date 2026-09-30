#!/bin/bash

# tools/fontes_generare.sh - snippet RADIX_FONTES ex aedile
# (generice - pro scriptis instrumentorum quae listas manuales
# bibliothecarum ferunt; exemplar primum: aedilis_struere.sh ipse,
# se-hospitans)
#
# Usus: ./tools/fontes_generare.sh <exitus.sh> <fons.c> [...]
#
# Clausurae scoporum uniuntur, obiecta lib/*.c ad bases nudantur,
# snippet fons-abile emittitur. Snippet COMMITTITUR - consumptor
# eum fontat sine aedile praesente (ovum bootstrap fractum);
# regeneratio bin/aedilis requirit.

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

if [ $# -lt 2 ]; then
    echo "usus: fontes_generare.sh <exitus.sh> <fons.c> [...]" >&2
    exit 1
fi
exitus="$1"
shift

if [ ! -x bin/aedilis ]; then
    ./tools/aedilis_struere.sh || exit 1
fi

# clausurae (fabrica T6): manifestum unum per radicem, ut iudex
# ingressus fragmenti VEROS sigillet (actio memorabilis). Directorium
# ante cursum vacuatur - radix ablata manifestum suum non relinquit.
clausurae="build/fabrica/clausurae/$(echo "${exitus%.sh}" | sed 's|/|__|g')"
rm -rf "$clausurae"
mkdir -p "$clausurae"

lista="$(
    for scopus in "$@"; do
        ./bin/aedilis "$scopus" --enumerare \
            --manifestum "$clausurae/$(echo "$scopus" | sed 's|/|__|g').stml" \
            || exit 1
    done | awk '/^lib\/.*\.c$/ { sub(/^lib\//,""); sub(/\.c$/,""); print }' \
         | sort -u
)"
if [ -z "$lista" ]; then
    echo "fontes_generare: derivatio vacua - recusatum" >&2
    exit 1
fi

# FABRICA_SCRIPTURA (iudex fabricae): fragmentum in scripturam, arbor
# intacta; clausurae tamen in build/ (status aedificationis, non arbor)
destinatio="$exitus"
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    destinatio="$FABRICA_SCRIPTURA/$exitus"
    mkdir -p "$(dirname "$destinatio")"
fi

{
    echo "# $(basename "$exitus") - GENERATUM AB AEDILE - NE MANU EDITES"
    echo "# regeneratio: ./tools/fontes_generare.sh $exitus $*"
    echo "declare -a RADIX_FONTES=("
    echo "$lista" | while IFS= read -r f; do
        echo "    \"$f\""
    done
    echo ")"
} > "$destinatio"

echo "$exitus scriptum ($(echo "$lista" | wc -l | tr -d ' ') fontes)"
