#!/bin/bash
# tools/briar_facies_capsula.sh - vestis faciei -> capsula infixa
#
# build/capsula_facies_briar.{c,h} e briar/facies/ (involucrum, styli,
# scriptum) + proiectione md faciei. Bloccus COMMUNIS cum
# briar_struere.sh, exemplar: tools/corpus_infixum.sh.
#
# Cur capsula et non literalia C: vestis plagulae VERAE sunt - CSS ut
# CSS legitur et emendatur, scriptum ut scriptum. briar eas SPLICAT,
# numquam continet (cicatrix speculi: 'JS in literalibus C - Latina
# irrepit'). Vide spec par. 4.6 F3.
#
# Non compressa: quattuor plagulae, pauca milia octetorum.
#
# Manu vocatum: regeneratur solum cum fons recentior; sub iudice
# (FABRICA_SCRIPTURA, radix umbra) et executore (FABRICA_AGIT) semper
# (fabrica 1b T6, tools/capsula_radicis.sh).

# shellcheck source=/dev/null
source "$(dirname "${BASH_SOURCE[0]}")/capsula_radicis.sh"

briar_facies_capsula_regenerare () {
    : "${CAPSULA_RADIX:=$(pwd)}"
    local TABULA="build/capsula_facies_briar.c"

    mkdir -p build
    local VESTIS="briar/facies/facies.html briar/facies/facies.css \
        briar/facies/facies.js briar/facies/md-html-facies.stml"
    if [ "${CAPSULA_SEMPER:-0}" != 1 ] && [ -f "$TABULA" ]; then
        local RECENS=""
        for v in $VESTIS; do
            if ! [ "$TABULA" -nt "$v" ]; then RECENS="$v"; break; fi
        done
        if [ -z "$RECENS" ]; then
            return 0
        fi
    fi
    cat > facies_briar.toml <<'TOML'
# GENERATUM a tools/briar_facies_capsula.sh - NE MANU EDITES (gitignoratum)
facies_briar_files = ["briar/facies/facies.html", "briar/facies/facies.css", "briar/facies/facies.js", "briar/facies/md-html-facies.stml"]
facies_briar_compress = false
TOML
    capsula_radicis_generare facies_briar || return 1
    rm -f facies_briar.toml
    return 0
}

# actio fabricae 'capsula_facies_briar' (plan 1b T6): scriptum directe vocatum
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    capsula_radicis_parare briar || exit 1
    briar_facies_capsula_regenerare || exit 1
fi
