#!/bin/bash
# tools/plastica_struere.sh - fabrica bin/plastica (plastica-plan T3;
# exemplar tools/negotia_struere.sh): clausura per aedilem,
# provenientia, structura, installatio per rm + cp (signatura codicis).
#
# Usus: ./tools/plastica_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/plastica.c >/dev/null || {
    echo "plastica: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): digestum ingressuum in binarium scribitur;
# 'bin/fabrica iudicare' bin/plastica stalum nominat cum fons mutatur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "plastica: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh plastica bin/plastica || {
    echo "plastica: provenientia fracta"; exit 1; }
sh build/aedilis/plastica/struere.sh >/dev/null || {
    echo "plastica: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam (macOS signaturam codicis
# per vnode condit; 'cp' super binarium quod modo cucurrit SIGKILL
# intermittentem parit - vide tools/negotia_struere.sh).
rm -f bin/plastica
cp build/aedilis/plastica/plastica bin/plastica || exit 1
echo "PLASTICA STRUCTUM: bin/plastica"
