#!/bin/bash

# tools/silva_fontes_generare.sh - snippets RADIX_FONTES launcher-orum
# silvae OMNIUM (via media; exemplar compile_tests_fontes_generare)
#
# Cur non fontes_generare.sh directe: scopus derivationis rectus =
# principalia + silva/fontes/*.c GLOB. Clausurae principalium modulos
# novos non semper vident (lectio silva_annotationes 2026-07-21:
# modulum quod nemo adhuc includit launcher-i tamen per globum
# compilant), et expansio globi in mandato insculpto CONGELAT (arbor/
# auspex/generare/lustrum listas expansas sine modulis novis ferebant).
# Hic scriptor globum tempore currendi expandit; principalia in linea
# "# principalia:" cuiusque snippet perstant.
#
# Usus: ./tools/silva_fontes_generare.sh    (omnia regenerat)

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

if [ ! -x bin/aedilis ]; then
    ./tools/aedilis_struere.sh || exit 1
fi

mkdir -p silva/build

# clausurae (fabrica T6): manifestum unum per radicem DISTINCTAM ->
# build/fabrica/clausurae/silva/ (ingressus 'manifesta' actionis
# memorabilis). Derivatio quoque semel per radicem: globus
# silva/fontes/*.c omnibus fragmentis communis est (XXII x XXV
# derivationes eaedem olim, ~CCXXXVIII s).
clausurae="build/fabrica/clausurae/silva"
rm -rf "$clausurae"
mkdir -p "$clausurae"
promptuarium="silva/build/fontes_promptuarium.$$"
mkdir -p "$promptuarium"
trap 'rm -rf "$promptuarium"' EXIT

# exitus aedilis radicis (obiecta) - semel derivatus, deinde e
# promptuario; manifestum simul scriptum
derivare() {
    local clavis
    clavis="$(echo "$1" | sed 's|/|__|g')"
    if [ ! -f "$promptuarium/$clavis" ]; then
        if ! ./bin/aedilis "$1" --enumerare --thesaurus build/aedilis/obiecta \
                --manifestum "$clausurae/$clavis.stml" \
                > "$promptuarium/$clavis.nov"; then
            return 1
        fi
        mv "$promptuarium/$clavis.nov" "$promptuarium/$clavis"
    fi
    cat "$promptuarium/$clavis"
}

for snip in silva/*_fontes_generata.sh; do
    principalia="$(sed -n 's/^# principalia: //p' "$snip")"
    if [ -z "$principalia" ]; then
        # migratio: argumenta ex mandato vetere, fontes/* exuta
        principalia="$(sed -n '2s/^# regeneratio: [^ ]* [^ ]* //p' "$snip" \
            | tr ' ' '\n' | grep -v '^silva/fontes/' | grep -v '^$' \
            | tr '\n' ' ')"
    fi
    if [ -z "${principalia// /}" ]; then
        echo "silva_fontes_generare: principalia non inventa in $snip - recusatum" >&2
        exit 1
    fi

    # derivatio EXTRA fistulas - exitus aedilis numquam absorbetur
    # (lectio: $() fistulatum truncare tacite potest)
    acc="silva/build/fontes_acc.$$"
    : > "$acc"
    for scopus in $principalia silva/fontes/*.c; do
        if ! derivare "$scopus" >> "$acc"; then
            echo "silva_fontes_generare: derivatio fracta in $scopus ($snip)" >&2
            rm -f "$acc"
            exit 1
        fi
    done
    lista="$(awk '/^lib\/.*\.c$/ { sub(/^lib\//,""); sub(/\.c$/,""); print }' \
        "$acc" | sort -u)"
    rm -f "$acc"
    if [ -z "$lista" ]; then
        echo "silva_fontes_generare: derivatio vacua ($snip) - recusatum" >&2
        exit 1
    fi

    # FABRICA_SCRIPTURA (iudex): fragmentum in scripturam, arbor intacta
    destinatio="$snip"
    if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
        destinatio="$FABRICA_SCRIPTURA/$snip"
        mkdir -p "$(dirname "$destinatio")"
    fi
    {
        echo "# $(basename "$snip") - GENERATUM AB AEDILE - NE MANU EDITES"
        echo "# regeneratio: ./tools/silva_fontes_generare.sh"
        echo "# principalia: $principalia"
        echo "declare -a RADIX_FONTES=("
        echo "$lista" | while IFS= read -r f; do
            echo "    \"$f\""
        done
        echo ")"
    } > "$destinatio"
    echo "$snip scriptum ($(echo "$lista" | wc -l | tr -d ' ') fontes)"
done
