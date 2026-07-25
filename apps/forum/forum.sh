#!/bin/bash

# apps/forum/forum.sh - involucrum tenue: capsula + aedilis + struere
# + exec (conventio apps/: constructio tota per aedilem derivata)
#
# Usus: ./apps/forum/forum.sh [-fumus] [-portus N]

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1

# bin/aedilis a compile_tests.sh SOLO aedificatur - custos ne
# lanceator in arbore recenti tacite cadat
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
[ -x bin/capsula_generare ] || {
    echo "forum: bin/capsula_generare abest (curre ./compile_tests.sh)" >&2
    exit 1
}

# CAPSULA ANTE OMNIA. Hic gradus DEERAT (usque 2026-07-25, 01KYCMPY96):
# index.html editum et lanceator relatus faciem STALAM monstrabant, et
# defectus SILENS erat - nihil dicit capsulam vetustam esse, ergo
# symptoma 'editio mea nihil fecit' est et suspicio ad JS it, non ad
# constructionem. villa.sh hunc gradum ab initio habuit.
./bin/capsula_generare apps/forum/assets/forum.toml >&2 || exit 1

./bin/aedilis apps/forum/forum.c >&2 || exit 1
# speculum: fontes proprii embedati (manifestum aedilis lectum)
tools/speculum_generare.sh forum apps/forum/forum.c apps/forum/forum.sh >&2 || exit 1
bash build/aedilis/forum/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
# (signatura ad-hoc invalidata) - inodus recens obligatorius
mkdir -p bin && rm -f bin/forum && cp build/aedilis/forum/forum bin/forum

exec bin/forum "$@"
