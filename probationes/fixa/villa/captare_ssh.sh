#!/bin/bash

# probationes/fixa/villa/captare_ssh.sh - formas DEFECTUS ipsius ssh
# capere (erratum + codex exitus).
#
# NIHIL MUTAT, nec localiter nec remote. Omnes casus aut ante
# conexionem cadunt (nomen insolubile, portus clausus, mora) aut in
# ipso salutationis gradu (clavis hospitis, permissio) - nulla
# sessio umquam aperitur. Casus 'imperium_absens' solus capsam
# remotam attingit, et ibi imperium quod NON EXSISTIT currere
# temptat, quod nihil agit per definitionem.
#
# CUR OMNINO: sine his villa unum 'ssh defecit' pro omnibus
# ostenderet. Sed clavis ignota, portus clausus, permissio negata,
# et imperium REMOTUM absens remedia prorsus diversa postulant -
# et nuntii ssh per versiones mutant, ergo formae FICTAE praecise
# ea fallerent quae discernere debemus.
#
# Usus: ./probationes/fixa/villa/captare_ssh.sh [alias_ssh]

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../../.." || exit 1

ALIAS="${1:-smaragda}"
DIR="probationes/fixa/villa/ssh"
CRUDA="probationes/fixa/villa/cruda/ssh"
mkdir -p "$CRUDA"

# BIS capere; PIN solum si erratum ET codex octetim idem sunt
capere () {
    local nomen="$1"; shift
    local r

    for r in a b; do
        "$@" > "$CRUDA/$nomen.$r.out" 2> "$CRUDA/$nomen.$r.err"
        echo $? > "$CRUDA/$nomen.$r.code"
    done
    if cmp -s "$CRUDA/$nomen.a.err" "$CRUDA/$nomen.b.err" \
        && cmp -s "$CRUDA/$nomen.a.code" "$CRUDA/$nomen.b.code"
    then
        mv "$CRUDA/$nomen.a.err"  "$CRUDA/$nomen.err"
        mv "$CRUDA/$nomen.a.code" "$CRUDA/$nomen.code"
        rm -f "$CRUDA/$nomen".b.* "$CRUDA/$nomen.a.out"
        printf "  PIN      %-18s exitus=%s\n" \
            "$nomen" "$(cat "$CRUDA/$nomen.code")"
    else
        printf "  VARIANS  %-18s a=%s b=%s\n" "$nomen" \
            "$(cat "$CRUDA/$nomen.a.code")" \
            "$(cat "$CRUDA/$nomen.b.code")"
    fi
}

echo "capiens formas defectus ssh (nihil mutatur)..."

# I. nomen resolvi non potest (.invalid per RFC MMDCVI numquam
#    resolvitur - nulla petitio DNS vera exit)
capere hospes_ignotus ssh -o BatchMode=yes -o ConnectTimeout=5 \
    nulla-tale-hospitium.invalid uptime

# II. portus clausus in ansa locali
capere recusata ssh -o BatchMode=yes -o ConnectTimeout=5 \
    -p 1 127.0.0.1 uptime

# III. mora conexionis (10.255.255.1 = non itinerabile)
capere tempus ssh -o BatchMode=yes -o ConnectTimeout=3 \
    10.255.255.1 uptime

# IV. clavis prava -> 'Permission denied (publickey)'.
#     CAUTIO: hoc fixum lineam FALLACEM PRIMAM fert ('Load key ...
#     invalid format') et decisivam SECUNDAM. Id est fixum
#     pretiosissimum huius copiae - parsator lineae primae hic
#     solo falleretur.
capere clavis_negata ssh -o BatchMode=yes -o ConnectTimeout=5 \
    -o IdentitiesOnly=yes -i /dev/null "$ALIAS" uptime

# V. known_hosts vacuum + strictum -> 'Host key verification failed'
capere clavis_hospitis ssh -o BatchMode=yes -o ConnectTimeout=5 \
    -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=yes \
    "$ALIAS" uptime

# VI. imperium REMOTUM absens -> CXXVII (ssh transmittit).
#     Distinctio quam villa postulat: ssh cucurrit, capsa
#     respondit, imperium defuit.
capere imperium_absens ssh -o BatchMode=yes -o ConnectTimeout=5 \
    "$ALIAS" nulla-tale-imperium-est

# VII. successus (erratum VACUUM - forma legitima, non defectus)
capere successus ssh -o BatchMode=yes -o ConnectTimeout=5 \
    "$ALIAS" 'echo salve'

echo
echo "cruda in $CRUDA/"
echo "PROXIMUM: expurgare manu -> $DIR/*.err + *.code"
echo "  (nuntii IP verum et nomen usoris ferunt)"
echo "CAUTIO: 'sed' BSD '\\b' NON fert - pro expurgatione"
echo "  's/fran@/usor@/g' scribe, non 's/\\bfran@/usor@/g'"
