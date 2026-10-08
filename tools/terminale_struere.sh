#!/bin/bash
# tools/terminale_struere.sh - fabrica bin/terminale (fenestra
# terminalis, aemulator-plan D8; exemplar tools/mensor_ui_struere.sh)
#
# Actio fabricae 'terminale' (aedificatio.stml); 'institutio_terminale'
# deinde bin/terminale in ~/.bin copiat (tools/instituere.sh, ex main).
# apps/terminale/terminale.sh hoc vocat ante exec.
#
# Usus: ./tools/terminale_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis apps/terminale/terminale.c >/dev/null \
    || { echo "terminale: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): '-provenientia' respondetur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "terminale: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh terminale bin/terminale || {
    echo "terminale: provenientia fracta"; exit 1; }
sh build/aedilis/terminale/struere.sh >/dev/null \
    || { echo "terminale: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam (macOS signaturam per
# vnode condit: 'cp' super binarium currentem -> SIGKILL; 'rm' inodum
# novum parit - vide tools/mensor_ui_struere.sh, MENSURATUM 2026-08-13)
rm -f bin/terminale
cp build/aedilis/terminale/terminale bin/terminale || exit 1
echo "TERMINALE STRUCTUM: bin/terminale"
