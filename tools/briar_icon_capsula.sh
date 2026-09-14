#!/bin/bash
# tools/briar_icon_capsula.sh - icon ordinarius '-app' -> capsula infixa
#
# build/capsula_icon_briar.{c,h} e briar/icon/default-thistle.jpg.
# Bloccus COMMUNIS cum briar_struere.sh (exemplar:
# tools/briar_facies_capsula.sh). Separata a capsula faciei: spectator
# iconem numquam ostendit, ergo eam non vehat (spec par. 4.8).
#
# Non compressa: JPEG iam compressum est (386 KB).
briar_icon_capsula_regenerare () {
    local TABULA="build/capsula_icon_briar.c"
    local ICON="briar/icon/default-thistle.jpg"

    mkdir -p build
    if [ -f "$TABULA" ] && ! [ "$ICON" -nt "$TABULA" ]; then
        return 0
    fi
    cat > icon_briar.toml <<'TOML'
# GENERATUM a tools/briar_icon_capsula.sh - NE MANU EDITES (gitignoratum)
icon_briar_files = ["briar/icon/default-thistle.jpg"]
icon_briar_compress = false
TOML
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || return 1
    fi
    ./bin/capsula_generare icon_briar.toml || return 1
    mv capsula_icon_briar.h capsula_icon_briar.c build/ || return 1
    rm -f icon_briar.toml
    return 0
}
