#!/bin/bash
# tools/capsula_radicis.sh - auxilium capsularum quarum globi e RADICE
# arboris leguntur (corpus infixum, capsulae briar) - fabrica 1b T6
#
# bin/capsula_generare exitum IUXTA plagulam TOML scribit et globos ex
# directorio eius solvit, ergo TOML in radice stare debet. Sub
# FABRICA_SCRIPTURA (iudex) radix UMBRA paratur: directorium scripturae
# nexus symbolicos ad ingressus radicis nominatos accipit, TOML et
# plagulae generatae ibi scribuntur, exitus in umbra/build/ - viae
# capsulae (nomina infixa) eaedem, arbor intacta.
#
# Usus (sourced): capsula_radicis_parare ingressus...
#   ponit CAPSULA_RADIX (radix vera) et cd in radicem operis (vera aut
#   umbra); CAPSULA_SEMPER=1 si regeneratio semper (iudex aut executor
#   fabricae: staleness fabrica iudicat, non mtime)
capsula_radicis_parare () {
    CAPSULA_RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
    CAPSULA_SEMPER=0
    if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
        local ingressus
        mkdir -p "$FABRICA_SCRIPTURA/build" || return 1
        for ingressus in "$@"; do
            ln -sfn "$CAPSULA_RADIX/$ingressus" \
                "$FABRICA_SCRIPTURA/$ingressus" || return 1
        done
        cd "$FABRICA_SCRIPTURA" || return 1
        CAPSULA_SEMPER=1
    else
        cd "$CAPSULA_RADIX" || return 1
        [ -n "${FABRICA_AGIT:-}" ] && CAPSULA_SEMPER=1
    fi
    return 0
}

# capsula_radicis_generare SECTIO: SECTIO.toml in radice operis ->
# build/capsula_SECTIO.{c,h}
capsula_radicis_generare () {
    local sectio="$1"
    if [ ! -x "$CAPSULA_RADIX/bin/capsula_generare" ]; then
        ( cd "$CAPSULA_RADIX" && ./compile_tools.sh capsula_generare \
            >/dev/null ) || return 1
    fi
    "$CAPSULA_RADIX/bin/capsula_generare" "$sectio.toml" >/dev/null \
        || return 1
    mkdir -p build || return 1
    mv "capsula_$sectio.h" "capsula_$sectio.c" build/ || return 1
}
