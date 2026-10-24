#!/bin/bash
# tools/auditio_struere.sh - fabrica bin/auditio (musica M5c; exemplar
# tools/manus_struere.sh): clausura per aedilem, provenientia, structura,
# installatio per rm + cp (signatura codicis, vide infra).
#
# Usus: ./tools/auditio_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/auditio.c >/dev/null || {
    echo "auditio: aedilis fracta"; exit 1; }
# provenientia (fabrica T7): digestum ingressuum in binarium scribitur;
# 'bin/fabrica iudicare' bin/auditio stalum nominat cum fons mutatur
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >/dev/null || {
    echo "auditio: fabrica non structa"; exit 1; }
./tools/provenientia_scribere.sh auditio bin/auditio || {
    echo "auditio: provenientia fracta"; exit 1; }
sh build/aedilis/auditio/struere.sh >/dev/null || {
    echo "auditio: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam.
#
# MENSURATUM 2026-08-13 (X ruinae, relatio 'CODESIGNING / Taskgated
# Invalid Signature'): macOS signaturam codicis per vnode condit. 'cp'
# super binarium quod modo cucurrit contentum novum relinquit sub
# signatura VETERE conditata - nucleus discrepantiam invenit et
# SIGKILL mittit. Intermittens de natura: pendet an cache calidus sit.
# 'rm' inodum NOVUM parit, ergo nihil conditum est quod discrepet.
rm -f bin/auditio
cp build/aedilis/auditio/auditio bin/auditio || exit 1
echo "AUDITIO STRUCTUM: bin/auditio"
