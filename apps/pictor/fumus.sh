#!/bin/bash
# apps/pictor/fumus.sh - fumus pictoris: fenestra vera, XXX quadra,
# volumen temporarium, exitus 0, linea mensurarum. Nullus cursor hoc
# verrit (apps/ extra probationes/); vocatur manu et a T11.
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
echo "=== FUMUS PICTORIS ==="
EXITUS=$(./apps/pictor/pictor.sh -fumus 2>&1)
CODEX=$?
echo "$EXITUS"
[ $CODEX -eq 0 ] || { echo "FUMUS FRACTUS (codex $CODEX)"; exit 1; }
echo "$EXITUS" | grep -q "ludus: quadra=30" || { echo "FRACTUM: quadra"; exit 1; }
echo "FUMUS PLENUS"
