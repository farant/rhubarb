#!/bin/bash
# tools/clonare_struere.sh - fabrica bin/clonare (pythonica S3; exemplar
# tools/manus_struere.sh): clausura per aedilem, provenientia, structura,
# installatio per rm + cp (signatura codicis, vide infra).
#
# Usus: ./tools/clonare_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/clonare.c >/dev/null || {
    echo "clonare: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): digestum ingressuum in binarium scribitur;
# 'bin/fabrica iudicare' bin/clonare stalum nominat cum fons mutatur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "clonare: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh clonare bin/clonare || {
    echo "clonare: provenientia fracta"; exit 1; }
sh build/aedilis/clonare/struere.sh >/dev/null || {
    echo "clonare: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam.
#
# MENSURATUM 2026-08-13 (X ruinae, relatio 'CODESIGNING / Taskgated
# Invalid Signature'): macOS signaturam codicis per vnode condit. 'cp'
# super binarium quod modo cucurrit contentum novum relinquit sub
# signatura VETERE conditata - nucleus discrepantiam invenit et
# SIGKILL mittit. Intermittens de natura: pendet an cache calidus sit.
# 'rm' inodum NOVUM parit, ergo nihil conditum est quod discrepet.
rm -f bin/clonare
cp build/aedilis/clonare/clonare bin/clonare || exit 1
echo "CLONARE STRUCTUM: bin/clonare"
