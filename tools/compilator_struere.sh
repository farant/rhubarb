#!/bin/bash
# tools/compilator_struere.sh - fabrica bin/compilator (fabrica plan 2
# T4): 'clang -c' per thesaurum contentorum. Mos manus_struere.sh:
# clausura ab aedile, provenientia, signatura per rm ante cp.
#
# Usus: ./tools/compilator_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/compilator.c >/dev/null || {
    echo "compilator: aedilis fracta"; exit 1; }
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "compilator: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh compilator bin/compilator || {
    echo "compilator: provenientia fracta"; exit 1; }
sh build/aedilis/compilator/struere.sh >/dev/null || {
    echo "compilator: structura fracta"; exit 1; }
# signatura macOS per vnode: rm (inodus novus) ante cp - vide
# manus_struere.sh
rm -f bin/compilator
cp build/aedilis/compilator/compilator bin/compilator || exit 1
echo "COMPILATOR STRUCTUS: bin/compilator"
