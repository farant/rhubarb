#!/bin/sh
# js_syntaxis.sh - plagulas JS syntaxi probare
#
#   ./tools/js_syntaxis.sh <plagula.js> [...]
#   ./tools/js_syntaxis.sh -tacite <plagula.js> [...]   (solum pravae)
#
# Exitus (mos domus):
#   0  omnes validae
#   1  aliqua prava
#   2  NIHIL PROBATUM (nulla plagula data aut inventa)
#
# Exitus II cardo est: globus qui nihil capit et porta quae omnia
# probat idem exitum ZEPHYRUM darent, et porta mortua a porta viva
# distingui non posset. Vide doctrinam instrumentorum.

set -e
HIC="$(cd "$(dirname "$0")" && pwd)"

TACITE=0
if [ "$1" = "-tacite" ]; then
    TACITE=1
    shift
fi

if [ "$#" -eq 0 ]; then
    echo "js_syntaxis: NULLA plagula data (exitus II = nihil probatum)" >&2
    exit 2
fi

EFFUSIO=$(osascript -l JavaScript "$HIC/js_syntaxis.js" "$@" 2>&1) || {
    echo "js_syntaxis: oraculum ipsum cecidit:" >&2
    echo "$EFFUSIO" >&2
    exit 2
}

SUMMA=$(printf '%s\n' "$EFFUSIO" | grep '^SUMMA ' | tail -1)
if [ -z "$SUMMA" ]; then
    echo "js_syntaxis: oraculum SUMMAM non reddidit (exitus II)" >&2
    printf '%s\n' "$EFFUSIO" >&2
    exit 2
fi

OMNES=$(printf '%s' "$SUMMA" | awk '{print $2}')
PRAVAE=$(printf '%s' "$SUMMA" | awk '{print $3}')

if [ "$TACITE" -eq 1 ]; then
    printf '%s\n' "$EFFUSIO" | grep -E '^(PRAVUM|ILLEGIBILIS|VACUA) ' || true
else
    printf '%s\n' "$EFFUSIO"
fi

if [ "$OMNES" -eq 0 ]; then
    echo "js_syntaxis: nulla plagula probata (exitus II)" >&2
    exit 2
fi
if [ "$PRAVAE" -ne 0 ]; then
    echo "js_syntaxis: $PRAVAE ex $OMNES PRAVAE" >&2
    exit 1
fi
echo "js_syntaxis: $OMNES validae"
exit 0
