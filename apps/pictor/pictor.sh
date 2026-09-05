#!/bin/bash
# apps/pictor/pictor.sh - involucrum tenue: aedilis + struere + exec
# (conventio apps/: constructio tota per aedilem derivata; capsula
# nulla in P3 - fons in codice, canones e disco)
#
# Usus: ./apps/pictor/pictor.sh [-fumus] [-volumen <via>]
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/pictor/pictor.c >&2 || exit 1
bash build/aedilis/pictor/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/pictor && cp build/aedilis/pictor/pictor bin/pictor
exec bin/pictor "$@"
