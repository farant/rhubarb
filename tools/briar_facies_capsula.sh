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
briar_facies_capsula_regenerare () {
    local TABULA="build/capsula_facies_briar.c"
    local VESTIS="briar/facies/facies.html briar/facies/facies.css \
        briar/facies/facies.js briar/facies/md-html-facies.stml"

    mkdir -p build
    if [ -f "$TABULA" ]; then
        local RECENS=""
        for v in $VESTIS; do
            if [ "$v" -nt "$TABULA" ]; then RECENS="$v"; break; fi
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
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || return 1
    fi
    ./bin/capsula_generare facies_briar.toml || return 1
    mv capsula_facies_briar.h capsula_facies_briar.c build/ || return 1
    rm -f facies_briar.toml
    return 0
}
