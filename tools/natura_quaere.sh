#!/bin/sh
# natura_quaere.sh - VETUS SUPERFICIES, nunc ad bin/natura mittens
#
# Munus huius scripti in bin/natura MIGRAVIT (2026-08-08):
#   ./tools/natura_quaere.sh <terminus>   ->  bin/natura quaere <terminus>
#   ./tools/natura_quaere.sh -a mod.genus ->  bin/natura apparatus mod.genus
#
# CUR MIGRATIO: duo instrumenta uni quaestioni respondentia domus
# duplex sunt, et responsum hic FORMATUM erat (tag '[species:...]'
# post lineam), quod truncari poterat sine responso amisso -
# exacte quod 2026-08-08 accidit ('mensuratio' liber visus est,
# apud fundamentum sedebat). bin/natura SENTENTIAM reddit ultima
# linea, et umbras quoque inspicit (tria: OCCUPATUM / UMBRATUM /
# LIBERUM).
#
# Hoc velum manet ne memoria manuum frangatur; nihil proprium agit.

set -u
cd "$(dirname "$0")/.." || exit 1

if [ ! -x bin/natura ]; then
    echo "natura_quaere: bin/natura abest - strue ./tools/natura_struere.sh" >&2
    exit 2
fi

echo "natura_quaere.sh: munus ad 'bin/natura' migravit - te mitto." >&2

if [ "${1:-}" = "-a" ]; then
    [ $# -lt 2 ] && {
        echo "usus: $0 -a [modulus.]genus" >&2
        exit 2
    }
    shift
    exec bin/natura apparatus "$@"
fi

[ $# -lt 1 ] && {
    echo "usus: $0 <terminus> | $0 -a [modulus.]genus" >&2
    exit 2
}

exec bin/natura quaere "$@"
