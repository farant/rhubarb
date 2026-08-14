#!/bin/bash
# tools/mensor_ui_struere.sh - fabrica bin/mensor_ui (explorator vitreae)
#
# Capsulam REGENERAT primum: facies in binario vehitur, ergo
# index.html mutatum sine hoc gradu INVISIBILE manet - et binarium
# vetus recte structum videtur.
#
# Usus: ./tools/mensor_ui_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/capsula_generare apps/mensor/assets/mensor.toml >/dev/null \
    || { echo "mensor_ui: capsula fracta"; exit 1; }
bin/aedilis apps/mensor/mensor_ui.c >/dev/null \
    || { echo "mensor_ui: aedilis fracta"; exit 1; }
sh build/aedilis/mensor_ui/struere.sh >/dev/null \
    || { echo "mensor_ui: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam.
#
# MENSURATUM 2026-08-13 (X ruinae, relatio 'CODESIGNING / Taskgated
# Invalid Signature'): macOS signaturam codicis per vnode condit. 'cp'
# super binarium quod modo cucurrit contentum novum relinquit sub
# signatura VETERE conditata - nucleus discrepantiam invenit et
# SIGKILL mittit. Intermittens de natura: pendet an cache calidus sit.
# 'rm' inodum NOVUM parit, ergo nihil conditum est quod discrepet.
rm -f bin/mensor_ui
cp build/aedilis/mensor_ui/mensor_ui bin/mensor_ui || exit 1
echo "MENSOR_UI STRUCTUS: bin/mensor_ui"
