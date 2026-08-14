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
cp build/aedilis/manus_instrumentum/manus_instrumentum bin/manus || exit 1
echo "MANUS STRUCTA: bin/manus"
