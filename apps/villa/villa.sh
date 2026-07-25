#!/bin/bash

# apps/villa/villa.sh - involucrum tenue: capsula + aedilis +
# struere + exec (conventio apps/)
#
# Usus: ./apps/villa/villa.sh [-fumus] [-alias N] [-portus N]

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1

# bin/aedilis a compile_tests.sh SOLO aedificatur - custos ne
# lanceator in arbore recenti tacite cadat
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
[ -x bin/capsula_generare ] || {
    echo "villa: bin/capsula_generare abest (curre ./compile_tests.sh)" >&2
    exit 1
}

# CAPSULA ANTE OMNIA: forum hunc gradum OMITTIT, ideo index.html
# editum et lanceatorem relatum faciem STALAM monstrat. Hic non.
./bin/capsula_generare apps/villa/assets/villa.toml >&2 || exit 1

./bin/aedilis apps/villa/villa.c >&2 || exit 1
tools/speculum_generare.sh villa apps/villa/villa.c \
    apps/villa/villa.sh >&2 || exit 1
bash build/aedilis/villa/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
# (signatura ad-hoc invalidata) - inodus recens obligatorius
mkdir -p bin && rm -f bin/villa && cp build/aedilis/villa/villa bin/villa

exec bin/villa "$@"
