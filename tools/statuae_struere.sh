#!/bin/bash
# tools/statuae_struere.sh - fabrica bin/statuae (statuae-plan T6;
# exemplar tools/negotia_struere.sh): clausura per aedilem,
# provenientia, structura, installatio per rm + cp (signatura codicis).
#
# Usus: ./tools/statuae_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/statuae.c >/dev/null || {
    echo "statuae: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): digestum ingressuum in binarium scribitur;
# 'bin/fabrica iudicare' bin/statuae stalum nominat cum fons mutatur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "statuae: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh statuae bin/statuae || {
    echo "statuae: provenientia fracta"; exit 1; }
sh build/aedilis/statuae/struere.sh >/dev/null || {
    echo "statuae: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam (macOS signaturam codicis
# per vnode condit; 'cp' super binarium quod modo cucurrit SIGKILL
# intermittentem parit - vide tools/negotia_struere.sh).
rm -f bin/statuae
cp build/aedilis/statuae/statuae bin/statuae || exit 1
echo "STATUAE STRUCTUM: bin/statuae"
