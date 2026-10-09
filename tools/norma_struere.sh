#!/bin/bash
# tools/norma_struere.sh - bin/norma per aedilis (regula R8 norma-plan-3)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
[ -x bin/aedilis ] || { echo "norma_struere: bin/aedilis deest" >&2; exit 2; }
./bin/aedilis tools/norma.c > /dev/null || exit 2
bash build/aedilis/norma/struere.sh > /dev/null || exit 2
cp build/aedilis/norma/norma bin/norma || exit 2
