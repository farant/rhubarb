#!/bin/bash
# tools/instrumentum_nectere.sh - nexus binarii instrumenti SOLUM SI STALUM,
# scriptura ATOMICA (sourcetur ab involucris silva/<x>.sh, officina/<x>.sh)
#
# Olim involucra 'clang ... -o "$X_BIN"' in OMNI vocatione currebant: tempus
# (nexus totus per quaestionem) et CONCURRENTIA - vocatio altera binarium
# exsequebatur dum prima id rescribebat (ledger …WB6Q3H: census convertoris
# VI simul fumum credo X locis fregit). Nunc:
#   - stalum = binarium absens, aut ingressus quivis STRICTE recentior
#     (regula make: tempora aequalia = recens - /bin/bash '-nt' secundas
#     comparat, et nexus post obiecta eadem secunda refecta aliter in
#     omni vocatione iteraretur) (fontes,
#     obiecta, tools/vexilla.sh, involucrum vocans ipsum - vexilla aut
#     bibliothecae mutatae), aut caput recentius (newest_header vocantis,
#     si definita);
#   - nexus in '<bin>.nexus.<pid>' deinde 'mv -f' - binarium currens
#     numquam in loco rescribitur (inodus novus; exsecutio vetus integra).
# Vocationes concurrentes stalae: quaeque suum temporarium nectit, ultima
# mv vincit - omnes binarium integrum exsequuntur.
#
# Usus:  source tools/instrumentum_nectere.sh
#        instrumentum_nectere <bin> <ingressus...> -- <argumenta clang...>
#        (argumenta clang SINE '-o'; fontes et obiecta inter ea quoque)
# Exitus: 0 recens aut nexum | 1 nexus fractus (temporarium deletum)

instrumentum_nectere () {
    local bin="$1"
    local stalum=0
    local ingressus
    shift
    [ -f "$bin" ] || stalum=1
    for ingressus in "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/vexilla.sh" \
                     "${BASH_SOURCE[1]}"; do
        [ "$stalum" -eq 1 ] || ! [ "$ingressus" -nt "$bin" ] || stalum=1
    done
    while [ $# -gt 0 ] && [ "$1" != "--" ]; do
        [ "$stalum" -eq 1 ] || ! [ "$1" -nt "$bin" ] || stalum=1
        shift
    done
    [ "${1:-}" = "--" ] && shift
    # <tolera codex="lint:effectus-mandatum-ignotum" (>newest_header ab involucro vocante definitur (declare -F custodit)
    if [ "$stalum" -eq 0 ] && declare -F newest_header > /dev/null \
        && [ -n "$(newest_header "$bin")" ]; then
        stalum=1
    fi
    [ "$stalum" -eq 0 ] && return 0
    local temporarium="$bin.nexus.$$"
    if ! clang "$@" -o "$temporarium"; then
        rm -f "$temporarium"
        return 1
    fi
    mv -f "$temporarium" "$bin"
    # clang -g fontem simul compilans et nectens '<exitus>.dSYM' iuxta
    # temporarium scribit: symbola cum binario migrant (non relinquuntur)
    if [ -d "$temporarium.dSYM" ]; then
        rm -rf "$bin.dSYM"
        mv -f "$temporarium.dSYM" "$bin.dSYM" 2> /dev/null \
            || rm -rf "$temporarium.dSYM"
    fi
}
