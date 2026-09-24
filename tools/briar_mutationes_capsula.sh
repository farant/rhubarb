#!/bin/bash
# tools/briar_mutationes_capsula.sh - charta mutationum -> capsula infixa
#
# build/capsula_mutationes_briar.{c,h} e briar/MUTATIONES.md. Versio
# binarii EX charta legitur (briar -versio, caput supremum '## vN'),
# ergo capsula stala = versio mendax: regeneratur cum charta recentior
# est aut tabula viam eius non nominat (exemplar:
# tools/briar_icon_capsula.sh). Bloccus COMMUNIS cum briar_struere.sh.
briar_mutationes_capsula_regenerare () {
    local TABULA="build/capsula_mutationes_briar.c"
    local CHARTA="briar/MUTATIONES.md"

    mkdir -p build
    if [ -f "$TABULA" ] && ! [ "$CHARTA" -nt "$TABULA" ] \
       && grep -qF "$CHARTA" "$TABULA"; then
        return 0
    fi
    cat > mutationes_briar.toml <<'TOML'
# GENERATUM a tools/briar_mutationes_capsula.sh - NE MANU EDITES (gitignoratum)
mutationes_briar_files = ["briar/MUTATIONES.md"]
mutationes_briar_compress = false
TOML
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || return 1
    fi
    ./bin/capsula_generare mutationes_briar.toml || return 1
    mv capsula_mutationes_briar.h capsula_mutationes_briar.c build/ || return 1
    rm -f mutationes_briar.toml
    return 0
}
