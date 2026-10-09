#!/bin/bash
# tools/norma_c_regenerare.sh <x.norma> <praefixum> <caput.h> <corpus.c>
# bin/norma c; sub FABRICA_SCRIPTURA exitus in scripturam iudicis
# (viae eaedem), arbor intacta.
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
[ $# -eq 4 ] || { echo "usus: norma_c_regenerare.sh <x.norma> <praefixum> <caput.h> <corpus.c>" >&2; exit 2; }
./tools/norma_struere.sh || exit 2
R="${FABRICA_SCRIPTURA:-.}"
mkdir -p "$R/$(dirname "$3")" "$R/$(dirname "$4")" || exit 2
exec bin/norma c "$1" -praefixum "$2" -caput "$R/$3" -corpus "$R/$4"
