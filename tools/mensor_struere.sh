#!/bin/bash
# tools/mensor_struere.sh - fabrica bin/mensor
#
# Clausuram per aedilis computat (sqlite per volumen intrat - id
# exspectatum est). Binarium in bin/ ponitur ut ceteri.
#
# Usus: ./tools/mensor_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
mkdir -p bin

bin/aedilis tools/mensor.c >/dev/null || { echo "mensor: aedilis fracta"; exit 1; }
sh build/aedilis/mensor/struere.sh >/dev/null || { echo "mensor: structura fracta"; exit 1; }
cp build/aedilis/mensor/mensor bin/mensor || exit 1
echo "MENSOR STRUCTUS: bin/mensor"
