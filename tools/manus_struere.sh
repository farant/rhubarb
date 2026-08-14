#!/bin/bash
# tools/manus_struere.sh - fabrica bin/manus
#
# Plagula 'manus_instrumentum.c' vocatur (non 'manus.c') quia
# lib/manus.c iam manus.o parit et acervus obiectorum planus est;
# binarium tamen 'manus' est, quod nomen usor scribit.
#
# Usus: ./tools/manus_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/manus_instrumentum.c >/dev/null || {
    echo "manus: aedilis fracta"; exit 1; }
sh build/aedilis/manus_instrumentum/struere.sh >/dev/null || {
    echo "manus: structura fracta"; exit 1; }
# SIGNATURA: destinatio TOLLENDA ante copiam.
#
# MENSURATUM 2026-08-13 (X ruinae, relatio 'CODESIGNING / Taskgated
# Invalid Signature'): macOS signaturam codicis per vnode condit. 'cp'
# super binarium quod modo cucurrit contentum novum relinquit sub
# signatura VETERE conditata - nucleus discrepantiam invenit et
# SIGKILL mittit. Intermittens de natura: pendet an cache calidus sit.
# 'rm' inodum NOVUM parit, ergo nihil conditum est quod discrepet.
rm -f bin/manus
cp build/aedilis/manus_instrumentum/manus_instrumentum bin/manus || exit 1
echo "MANUS STRUCTA: bin/manus"
