#!/bin/bash
# tools/linux/probatio.sh - probationes sub Linux (glibc) currere
#
# Usus: ./tools/linux/probatio.sh <nomina...>     # e.g. piscina chorda xar
#
# Clausuras HOSPES computat (bin/aedilis --partes - vasi aedilis
# deest), scriptum vasis generat, vas UNUM compilat et CURRIT.
# Probationes purae C solae - quae .m/fenestram trahunt hic non
# pertinent (vide porta_linux.sh pro sceleto).
# Exitus: 0 omnes praeteritae | 1 fractae | 2 apparatus
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

[ $# -ge 1 ] || { echo "usus: $0 <nomina probationum...>" >&2; exit 2; }

SCRATCH="build/porta_linux/probationes"
mkdir -p "$SCRATCH"
SCRIPTUM="$SCRATCH/curre.sh"

{
    echo '#!/bin/sh'
    echo 'set -u'
    echo 'FLAGS="-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -Wno-type-limits -Iinclude -Iprobationes"'
    echo 'fractae=0'
} > "$SCRIPTUM"

for titulus in "$@"; do
    PROBATIO="probationes/probatio_${titulus}.c"
    if [ ! -f "$PROBATIO" ]; then
        echo "probatio.sh: $PROBATIO deest" >&2
        exit 2
    fi
    PARTES=$(bin/aedilis "$PROBATIO" --partes 2>/dev/null \
        | awk -F'\t' '$1=="O" {printf "%s ", $2}')
    if [ -z "$PARTES" ]; then
        echo "probatio.sh: clausura $titulus vacua (aedilis)" >&2
        exit 2
    fi
    {
        echo "echo '== $titulus =='"
        echo "gcc \$FLAGS $PROBATIO $PARTES -lm -o /tmp/p_$titulus || { echo 'COMPILATIO FRACTA'; fractae=\$((fractae+1)); }"
        echo "[ -x /tmp/p_$titulus ] && { /tmp/p_$titulus > /tmp/e_$titulus 2>&1 || fractae=\$((fractae+1)); tail -6 /tmp/e_$titulus; }"
    } >> "$SCRIPTUM"
done
echo 'exit $([ "$fractae" = "0" ] && echo 0 || echo 1)' >> "$SCRIPTUM"

docker run --rm -v "$PWD":/rhubarb:ro -w /rhubarb \
    rhubarb-proba-glibc sh "/rhubarb/$SCRIPTUM"
EXITUS=$?
if [ "$EXITUS" = "0" ]; then
    echo "PROBATIONES LINUX: PRAETERITAE ($#)"
else
    echo "PROBATIONES LINUX: FRACTAE" >&2
fi
exit "$EXITUS"
