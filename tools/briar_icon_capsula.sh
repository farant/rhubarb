#!/bin/bash
# tools/briar_icon_capsula.sh - icon ordinarius '-app' -> capsula infixa
#
# build/capsula_icon_briar.{c,h} e briar/icon/app-icon-transparent.png
# (MXXIV px, RGBA pellucidum; olim default-thistle.jpg).
# Bloccus COMMUNIS cum briar_struere.sh (exemplar:
# tools/briar_facies_capsula.sh). Separata a capsula faciei: spectator
# iconem numquam ostendit, ergo eam non vehat (spec par. 4.8).
#
# Non compressa: PNG iam compressum est (1,9 MB).
#
# Manu vocatum: regeneratur solum cum fons recentior; sub iudice
# (FABRICA_SCRIPTURA, radix umbra) et executore (FABRICA_AGIT) semper
# (fabrica 1b T6, tools/capsula_radicis.sh).

# shellcheck source=/dev/null
source "$(dirname "${BASH_SOURCE[0]}")/capsula_radicis.sh"

briar_icon_capsula_regenerare () {
    : "${CAPSULA_RADIX:=$(pwd)}"
    local TABULA="build/capsula_icon_briar.c"

    mkdir -p build
    local ICON="briar/icon/app-icon-transparent.png"
    # recens = tabula exsistit, icon non recentior, ET tabula viam
    # ICONIS HUIUS nominat
    if [ "${CAPSULA_SEMPER:-0}" != 1 ] && [ -f "$TABULA" ] \
       && ! [ "$ICON" -nt "$TABULA" ] && grep -qF "$ICON" "$TABULA"; then
        return 0
    fi
    cat > icon_briar.toml <<'TOML'
# GENERATUM a tools/briar_icon_capsula.sh - NE MANU EDITES (gitignoratum)
icon_briar_files = ["briar/icon/app-icon-transparent.png"]
icon_briar_compress = false
TOML
    capsula_radicis_generare icon_briar || return 1
    rm -f icon_briar.toml
    return 0
}

# actio fabricae 'capsula_icon_briar' (plan 1b T6): scriptum directe vocatum
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    capsula_radicis_parare briar || exit 1
    briar_icon_capsula_regenerare || exit 1
fi
