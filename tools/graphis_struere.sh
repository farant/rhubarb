#!/bin/bash
# tools/graphis_struere.sh - fabrica bin/graphis (graphis-plan T4;
# exemplar tools/negotia_struere.sh): clausura per aedilem,
# provenientia, structura, installatio per rm + cp (signatura codicis).
#
# Usus: ./tools/graphis_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/graphis.c >/dev/null || {
    echo "graphis: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): digestum ingressuum in binarium scribitur;
# 'bin/fabrica iudicare' bin/graphis stalum nominat cum fons mutatur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "graphis: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh graphis bin/graphis || {
    echo "graphis: provenientia fracta"; exit 1; }
sh build/aedilis/graphis/struere.sh >/dev/null || {
    echo "graphis: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam (macOS signaturam codicis
# per vnode condit; 'cp' super binarium quod modo cucurrit SIGKILL
# intermittentem parit - vide tools/negotia_struere.sh).
rm -f bin/graphis
cp build/aedilis/graphis/graphis bin/graphis || exit 1
echo "GRAPHIS STRUCTUM: bin/graphis"
