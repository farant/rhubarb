#!/bin/bash
# tools/scaena_struere.sh - fabrica bin/scaena (graphis-plan T7;
# exemplar tools/negotia_struere.sh): clausura per aedilem,
# provenientia, structura, installatio per rm + cp (signatura codicis).
#
# Usus: ./tools/scaena_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/scaena.c >/dev/null || {
    echo "scaena: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): digestum ingressuum in binarium scribitur;
# 'bin/fabrica iudicare' bin/scaena stalum nominat cum fons mutatur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "scaena: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh scaena bin/scaena || {
    echo "scaena: provenientia fracta"; exit 1; }
sh build/aedilis/scaena/struere.sh >/dev/null || {
    echo "scaena: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam (macOS signaturam codicis
# per vnode condit; 'cp' super binarium quod modo cucurrit SIGKILL
# intermittentem parit - vide tools/negotia_struere.sh).
rm -f bin/scaena
cp build/aedilis/scaena/scaena bin/scaena || exit 1
echo "SCAENA STRUCTUM: bin/scaena"
