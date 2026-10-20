#!/bin/bash

# tools/linux/aedificare.sh - applicatio in binarium STATICUM Linux
# (musl, amd64 ordinarie) -> build/linux/<titulus>
#
# Usus: ./tools/linux/aedificare.sh apps/exemplar/exemplar.c
#       LINUX_ARCHITECTURA=arm64 ./tools/linux/aedificare.sh ...
#
# Clausura HOSPES computatur (bin/aedilis --varians posix --partes:
# variantes _posix, non _macos), deinde in vase alpine/musl
# (--platform linux/<arch>; sub OrbStack amd64 per emulationem)
# compilatur: fontes nostri cum vexillis domus + -static -O2, venditi
# (V: sqlite, bearssl) cum vexillis laxis compile_tests. Binarium unum,
# sine bibliothecis communibus - in quamvis distributionem Linux
# copiari potest (smaragda: Ubuntu 25.04, glibc irrelevans).
#
# Exitus: 0 structum | 1 compilatio fracta | 2 apparatus (docker,
# clausura)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

[ $# -eq 1 ] || { echo "usus: $0 <applicatio.c>" >&2; exit 2; }
FONS="$1"
[ -f "$FONS" ] || { echo "aedificare.sh: $FONS deest" >&2; exit 2; }
TITULUS="$(basename "$FONS" .c)"
ARCH="${LINUX_ARCHITECTURA:-amd64}"
IMAGO="rhubarb-proba-musl-$ARCH"

command -v docker > /dev/null || { echo "aedificare.sh: docker deest (OrbStack?)" >&2; exit 2; }
docker info > /dev/null 2>&1 || { echo "aedificare.sh: docker non currit (OrbStack incipe)" >&2; exit 2; }
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 2

if ! docker image inspect "$IMAGO" > /dev/null 2>&1; then
    docker build -q --platform "linux/$ARCH" -t "$IMAGO" \
        -f tools/linux/Dockerfile.musl tools/linux > /dev/null || exit 2
fi

PARTES_TOTAE="$(bin/aedilis "$FONS" --varians posix --partes 2>/dev/null)"
PARTES="$(printf '%s\n' "$PARTES_TOTAE" | awk -F'\t' '$1=="O" {printf "%s ", $2}')"
VENDITI="$(printf '%s\n' "$PARTES_TOTAE" | awk -F'\t' '$1=="V" {printf "%s ", $2}')"
[ -n "$PARTES" ] || { echo "aedificare.sh: clausura vacua (aedilis)" >&2; exit 2; }
RADICES="$(bin/aedilis --radices 2>/dev/null | sed 's/^/-I/' | tr '\n' ' ')"

SCRATCH="build/porta_linux/aedificare"
mkdir -p "$SCRATCH" build/linux
SCRIPTUM="$SCRATCH/$TITULUS.sh"
{
    echo '#!/bin/sh'
    echo 'set -eu'
    echo "FLAGS=\"-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -Wno-type-limits -O2 $RADICES\""
    echo 'VFLAGS="-O2 -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0 -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"'
    VOBJ=""
    for v in $VENDITI; do
        b="$(basename "$v" .c)"
        VOBJ="$VOBJ /tmp/v_$b.o"
        echo "gcc \$VFLAGS -c $v -o /tmp/v_$b.o"
    done
    echo "gcc \$FLAGS -static $FONS $PARTES $VOBJ -lm -o /out/$TITULUS"
    echo "strip /out/$TITULUS"
} > "$SCRIPTUM"

rm -f "build/linux/$TITULUS"
docker run --rm --platform "linux/$ARCH" \
    -v "$PWD":/rhubarb:ro -v "$PWD/build/linux":/out -w /rhubarb \
    "$IMAGO" sh "/rhubarb/$SCRIPTUM" || {
    echo "aedificare.sh: compilatio fracta ($TITULUS, $ARCH)" >&2
    exit 1
}
file "build/linux/$TITULUS"
echo "STRUCTUM: build/linux/$TITULUS"
