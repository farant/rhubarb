#!/bin/bash

# apps/forum/fumus.sh - fumus plenus se-nuntians (exemplar tabellae)
#
# Daemon effimerus mundo effimero (portu auto) + fenestra vera
# -fumus-plenus: IS pipat in onere, C respondet via daemonis (via
# MCP simulata), IS filium in tabula videt -> FUMUS PLENUS + exitus
# 0. Fractura quaevis = exitus non-zerus aut tempus excessum.

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1

SCRATCH="build/fumus_fori"
mkdir -p "$SCRATCH"
rm -f "$SCRATCH"/*

./gesta/tabulariumd.sh -struere || exit 1
./apps/forum/forum.sh -fumus >/dev/null 2>&1 || true  # praestruit app

gesta/build/tabulariumd -portus 0 -scrinium "$SCRATCH/f.db" \
    -annales "$SCRATCH/f.jsonl" > "$SCRATCH/portus.txt" 2>/dev/null &
DPID=$!
PORT=""
for i in $(seq 1 25); do
    PORT=$(awk '{print $2}' "$SCRATCH/portus.txt" 2>/dev/null)
    [ -n "$PORT" ] && break
    sleep 0.2
done
if [ -z "$PORT" ]; then
    echo "FUMUS FRACTUS: daemon non ortus" >&2
    kill "$DPID" 2>/dev/null
    exit 1
fi

bin/forum -fumus-plenus -portus "$PORT"
EXITUS=$?

kill "$DPID" 2>/dev/null
if [ "$EXITUS" -eq 0 ]; then
    echo "FUMUS FORI: PLENUS (exitus 0)"
else
    echo "FUMUS FORI: FRACTUS (exitus $EXITUS)" >&2
fi
exit "$EXITUS"
