#!/bin/bash
# tools/nexus_probationis.sh - nexus binarii probationis SOLUM si signum
# nexus mutatum. FONTANDUM, non exsequendum (cursores suitarum: tessera,
# gesta, officina, briar, oratio - budget B2 E3/J9).
#
# CUR. Cursores omnem probationem omni cursu nectebant (~LXXXIV
# probationes per suitas). Obiecta nunc per bin/compilator: obiectum
# identicum non rescribitur, ergo inode et mtime per ictus stabiles
# manent - signum (vexilla + stat obiectorum et binarii) idem = nexus
# idem (deterministicus), nihil novi nectendum. Exemplar aedilis E10
# (nexus_signum in struere.sh emisso).
#
# probationem_nectere <binarium> <vexilla nexus> <obiecta...>
#   vexilla nexus: verbum UNUM (vexilla spatiis iuncta, ut "${GCC_FLAGS[*]}"),
#   in signo et in vocatione clang (scissa). Signum: vexilla + 'stat -f
#   %N %i %m %z' omnium obiectorum et binarii; in <binarium>.nexus
#   servatum post nexum. Signum mutatum aut binarium absens -> clang
#   nectit. Obiectum remotum ex lista signum mutat (lista intrat).
#   Exitus 1 si clang fractus (signum deletum, binarium non servatur).

_nexus_probationis_signum () {   # <binarium> <vexilla> <obiecta...>
    local binarium="$1" vexilla="$2"
    shift 2
    echo "$vexilla"
    stat -f '%N %i %m %z' "$@" "$binarium" 2>/dev/null
}

probationem_nectere () {   # <binarium> <vexilla nexus> <obiecta...>
    local binarium="$1" vexilla="$2" signum
    shift 2
    signum="$(_nexus_probationis_signum "$binarium" "$vexilla" "$@")"
    if [ -f "$binarium" ] \
            && [ "$signum" = "$(cat "$binarium.nexus" 2>/dev/null)" ]; then
        return 0
    fi
    rm -f "$binarium.nexus"
    # shellcheck disable=SC2086 (vexilla: verba consulto scissa)
    clang $vexilla "$@" -o "$binarium" || return 1
    _nexus_probationis_signum "$binarium" "$vexilla" "$@" > "$binarium.nexus"
}
