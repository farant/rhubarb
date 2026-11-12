#!/bin/bash
# tools/mundus_struere.sh - fabrica bin/mundus (plastica-plan T9;
# exemplar tools/negotia_struere.sh): clausura per aedilem,
# provenientia, structura, installatio per rm + cp (signatura codicis).
#
# Usus: ./tools/mundus_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/mundus.c >/dev/null || {
    echo "mundus: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): digestum ingressuum in binarium scribitur;
# 'bin/fabrica iudicare' bin/mundus stalum nominat cum fons mutatur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "mundus: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh mundus bin/mundus || {
    echo "mundus: provenientia fracta"; exit 1; }
sh build/aedilis/mundus/struere.sh >/dev/null || {
    echo "mundus: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam (macOS signaturam codicis
# per vnode condit; 'cp' super binarium quod modo cucurrit SIGKILL
# intermittentem parit - vide tools/negotia_struere.sh).
rm -f bin/mundus
cp build/aedilis/mundus/mundus bin/mundus || exit 1
echo "MUNDUS STRUCTUM: bin/mundus"
