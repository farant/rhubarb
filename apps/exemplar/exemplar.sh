#!/bin/bash

# apps/exemplar/exemplar.sh - involucrum tenue: aedilis + struere +
# exec (conventio apps/). Data localia: build/exemplar_data.
#
# Usus: ./apps/exemplar/exemplar.sh [-portus N] [-invitatio C] ...
#       (sine -data: build/exemplar_data)

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1

[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1

./bin/aedilis apps/exemplar/exemplar.c >&2 || exit 1
bash build/aedilis/exemplar/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/exemplar \
    && cp build/aedilis/exemplar/exemplar bin/exemplar

case " $* " in
    *" -data "*) exec bin/exemplar "$@" ;;
    *) exec bin/exemplar -data build/exemplar_data "$@" ;;
esac
