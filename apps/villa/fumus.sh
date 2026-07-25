#!/bin/bash

# apps/villa/fumus.sh - fumus villae: catena tota sine oculis.
#
# CAUTIO: hoc scriptum a NULLO cursore verritur (probationes/ apps/
# non videt). Dimidium quod verri DEBET in probationes/
# probatio_villa_agens.c habitat - hic sola catena APPLICATIONIS
# manet (configuratio -> agens -> probatio -> effusio), quae
# binarium verum postulat.
#
# ssh VERUS numquam vocatur: VILLA_SSH ad stipitem ponitur.

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1

export VILLA_SSH="probationes/fixa/villa/ssh_stipes.sh"
export VILLA_STIPES_MODUS="${VILLA_STIPES_MODUS:-bene}"

echo "=== FUMUS VILLAE (stipes: $VILLA_STIPES_MODUS) ==="
EXITUS=$(./apps/villa/villa.sh -fumus 2>&1)
CODEX=$?
echo "$EXITUS"

if [ $CODEX -ne 0 ]; then
    echo "FUMUS FRACTUS (codex $CODEX)"
    exit 1
fi

# adsertiones formae: numeri ex fixis veris venientes
echo "$EXITUS" | grep -q "felix=1"      || { echo "FRACTUM: non felix"; exit 1; }
echo "$EXITUS" | grep -q "unitates=25"  || { echo "FRACTUM: unitates"; exit 1; }
echo "$EXITUS" | grep -q "servitia=3"   || { echo "FRACTUM: servitia"; exit 1; }
echo "$EXITUS" | grep -q "situs=4"      || { echo "FRACTUM: situs"; exit 1; }
echo "$EXITUS" | grep -q "querelae=''" || echo "$EXITUS" | grep -q "querelae=''" \
    || echo "  (nota: querelae praesentes - inspice supra)"

echo "FUMUS PLENUS"
