#!/bin/bash

# apps/forum/forum.sh - involucrum tenue: aedilis + struere + exec
# (conventio apps/: constructio tota per aedilem derivata)
#
# Usus: ./apps/forum/forum.sh [-fumus] [-portus N]

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1

./bin/aedilis apps/forum/forum.c >&2 || exit 1
# speculum: fontes proprii embedati (manifestum aedilis lectum)
tools/speculum_generare.sh forum apps/forum/forum.c apps/forum/forum.sh >&2 || exit 1
bash build/aedilis/forum/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
# (signatura ad-hoc invalidata) - inodus recens obligatorius
mkdir -p bin && rm -f bin/forum && cp build/aedilis/forum/forum bin/forum

exec bin/forum "$@"
