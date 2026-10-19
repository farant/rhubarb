#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/linux.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ante 'cd' infra stat, ne "$0" relativum pereat.
# Ratio plena: compile_tests.sh radicis.
if [ -z "${LINUX_PROBATIO_EFFUSIO:-}" ]; then
    export LINUX_PROBATIO_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../../build/test_logs/linux.log"
    exit "${PIPESTATUS[0]}"
fi

# tools/linux/probatio.sh - probationes sub Linux (glibc aut musl)
#
# Usus: ./tools/linux/probatio.sh [-musl|-glibc] <nomina...>
#       e.g. ./tools/linux/probatio.sh -musl piscina chorda xar
#
# Clausuras HOSPES computat (bin/aedilis --varians posix --partes -
# vasi aedilis deest), scriptum vasis generat, vas UNUM compilat et
# CURRIT. Radices inclusionum ex bin/aedilis --radices (apps/ quoque);
# fontes venditi (V: sqlite, bearssl) semel cum vexillis laxis
# compile_tests (VENDOR_FLAGS). Probationes purae C solae - quae
# .m/fenestram trahunt hic non pertinent (vide porta_linux.sh).
# Exitus: 0 omnes praeteritae | 1 fractae | 2 apparatus
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

IMAGO=glibc
case "${1:-}" in
    -musl) IMAGO=musl; shift ;;
    -glibc) shift ;;
esac
[ $# -ge 1 ] || { echo "usus: $0 [-musl|-glibc] <nomina probationum...>" >&2; exit 2; }
RADICES="$(bin/aedilis --radices 2>/dev/null | sed 's/^/-I/' | tr '\n' ' ')"
[ -n "$RADICES" ] || { echo "probatio.sh: radices (bin/aedilis --radices)" >&2; exit 2; }

SCRATCH="build/porta_linux/probationes"
mkdir -p "$SCRATCH"
SCRIPTUM="$SCRATCH/curre.sh"

{
    echo '#!/bin/sh'
    echo 'set -u'
    echo "FLAGS=\"-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -Wno-type-limits -O2 $RADICES\""
    echo 'VFLAGS="-O2 -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0 -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"'
    echo 'fractae=0'
} > "$SCRIPTUM"

for titulus in "$@"; do
    PROBATIO="probationes/probatio_${titulus}.c"
    if [ ! -f "$PROBATIO" ]; then
        echo "probatio.sh: $PROBATIO deest" >&2
        exit 2
    fi
    PARTES=$(bin/aedilis "$PROBATIO" --varians posix --partes 2>/dev/null \
        | awk -F'\t' '$1=="O" {printf "%s ", $2}')
    VENDITI=$(bin/aedilis "$PROBATIO" --varians posix --partes 2>/dev/null \
        | awk -F'\t' '$1=="V" {printf "%s ", $2}')
    VOBJ=""
    for v in $VENDITI; do
        b=$(basename "$v" .c)
        VOBJ="$VOBJ /tmp/v_$b.o"
        echo "[ -f /tmp/v_$b.o ] || gcc \$VFLAGS -c $v -o /tmp/v_$b.o || { echo 'VENDITUM FRACTUM $v'; fractae=\$((fractae+1)); }" >> "$SCRIPTUM"
    done
    if [ -z "$PARTES" ]; then
        echo "probatio.sh: clausura $titulus vacua (aedilis)" >&2
        exit 2
    fi
    {
        echo "echo '== $titulus =='"
        echo "gcc \$FLAGS $PROBATIO $PARTES $VOBJ -lm -o /tmp/p_$titulus || { echo 'COMPILATIO FRACTA'; fractae=\$((fractae+1)); }"
        echo "[ -x /tmp/p_$titulus ] && { /tmp/p_$titulus > /tmp/e_$titulus 2>&1 || fractae=\$((fractae+1)); tail -6 /tmp/e_$titulus; }"
    } >> "$SCRIPTUM"
done
echo 'exit $([ "$fractae" = "0" ] && echo 0 || echo 1)' >> "$SCRIPTUM"

docker run --rm -v "$PWD":/rhubarb:ro -w /rhubarb \
    "rhubarb-proba-$IMAGO" sh "/rhubarb/$SCRIPTUM"
EXITUS=$?
if [ "$EXITUS" = "0" ]; then
    echo "PROBATIONES LINUX ($IMAGO): PRAETERITAE ($#)"
else
    echo "PROBATIONES LINUX ($IMAGO): FRACTAE" >&2
fi
exit "$EXITUS"
