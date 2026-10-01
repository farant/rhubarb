#!/bin/bash
# tools/briar_mutationes_capsula.sh - charta mutationum -> capsula infixa
#
# build/capsula_mutationes_briar.{c,h} e briar/MUTATIONES.md. Versio
# binarii EX charta legitur (briar -versio, caput supremum '## vN'),
# ergo capsula stala = versio mendax: regeneratur cum charta recentior
# est aut tabula viam eius non nominat (exemplar:
# tools/briar_icon_capsula.sh). Bloccus COMMUNIS cum briar_struere.sh.
#
# Manu vocatum: regeneratur solum cum fons recentior; sub iudice
# (FABRICA_SCRIPTURA, radix umbra) et executore (FABRICA_AGIT) semper
# (fabrica 1b T6, tools/capsula_radicis.sh).

# shellcheck source=/dev/null
source "$(dirname "${BASH_SOURCE[0]}")/capsula_radicis.sh"

briar_mutationes_capsula_regenerare () {
    : "${CAPSULA_RADIX:=$(pwd)}"
    local TABULA="build/capsula_mutationes_briar.c"

    mkdir -p build
    local CHARTA="briar/MUTATIONES.md"
    if [ "${CAPSULA_SEMPER:-0}" != 1 ] && [ -f "$TABULA" ] \
       && ! [ "$CHARTA" -nt "$TABULA" ] && grep -qF "$CHARTA" "$TABULA"; then
        return 0
    fi
    cat > mutationes_briar.toml <<'TOML'
# GENERATUM a tools/briar_mutationes_capsula.sh - NE MANU EDITES (gitignoratum)
mutationes_briar_files = ["briar/MUTATIONES.md"]
mutationes_briar_compress = false
TOML
    capsula_radicis_generare mutationes_briar || return 1
    rm -f mutationes_briar.toml
    return 0
}

# actio fabricae 'capsula_mutationes_briar' (plan 1b T6): scriptum directe vocatum
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    capsula_radicis_parare briar || exit 1
    briar_mutationes_capsula_regenerare || exit 1
fi
