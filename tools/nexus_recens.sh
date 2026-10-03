#!/bin/bash
# tools/nexus_recens.sh - nexus binariorum: SOLUM si stalum, ATOMICE.
# Fontandum (non exsecutum) a structoribus manu scriptis
# (aedilis_struere.sh, natura_struere.sh).
#
# CUR (2026-10-02): aedilis_struere.sh bin/aedilis OMNI vocatione
# relinkabat. Nexus macOS octetis non reproducibilis (LC_UUID), et
# fabrica catenam amalgamatorum (et fragmentum compile_tests) per
# binarium bin/aedilis clavat - ergo omnis cursus generata (qui
# aedilis_struere vocat) memorias fabricae ~C s irritas faciebat, et
# porta 'fabrica' (generata + iudex plenus) id SIBI ipsi faciebat
# (mensuratum: -plenus post generata 117 s, statim iterum 16 s). Item
# scriptura binarii IN LOCO processum eum currentem necat (macOS).
# natura_struere.sh idem eodem die (frigida_probare) - nunc sedes una.
#
# binarium_recens <binarium> <ingressus...>
#   0 si binarium exstat et STRICTE recentius omni ingressu exstante
#   (aequalitas secundi = stalum; argumenta quae plagulae non sunt -
#   vexilla clang - praetereuntur). Sub FABRICA_AGIT semper 1: fabrica
#   stalitatem iudicat, executor semper agit.
binarium_recens () {
    local binarium="$1" f
    shift
    [ -z "${FABRICA_AGIT:-}" ] || return 1
    [ -f "$binarium" ] || return 1
    for f in "$@"; do
        [ -e "$f" ] || continue
        ! [ "$binarium" -nt "$f" ] && return 1
    done
    return 0
}

# nectere_atomice <exitus> <argumenta clang...>
#   clang ... -o <exitus>.novum.<pid>, deinde mv (renominatio atomica:
#   processus binarium vetus currens non turbatur); fasciculus dSYM
#   (vexillum -g) cum binario transfertur. Exitus 1 si clang fractus.
nectere_atomice () {
    local exitus="$1"
    shift
    clang "$@" -o "$exitus.novum.$$" || {
        rm -f "$exitus.novum.$$"
        rm -rf "$exitus.novum.$$.dSYM"
        return 1
    }
    mv -f "$exitus.novum.$$" "$exitus" || return 1
    if [ -d "$exitus.novum.$$.dSYM" ]; then
        rm -rf "$exitus.dSYM"
        mv "$exitus.novum.$$.dSYM" "$exitus.dSYM" || return 1
    fi
    return 0
}
