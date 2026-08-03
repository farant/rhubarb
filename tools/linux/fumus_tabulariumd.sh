#!/bin/bash
# tools/linux/fumus_tabulariumd.sh - fumus daemonis INTRA vas glibc
# (porta_linux.sh -fumus hoc vocat)
#
# Aedificat tabulariumd sub gcc (clausura ex tabulariumd_fontes_
# generata.sh + gesta/fontes + principale + sqlite venditum laxatum),
# daemonem mundo effimero incipit, petitionem JSON-RPC unam per
# /dev/tcp mittit (ansa per-lineam - tabellarius), responsum legit.
# FUMUS PLENUS = responsum jsonrpc venit (daemon vivit, parsat,
# respondet sub Linux).
set -u
cd /rhubarb || exit 2

FLAGS="-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -Wno-type-limits -Iinclude -Igesta/fontes"
VFLAGS="-O2 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0 -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"
OB=/tmp/ob
mkdir -p "$OB"

source gesta/tabulariumd_fontes_generata.sh

obj=""
for f in "${RADIX_FONTES[@]}"; do
    gcc $FLAGS -c "lib/$f.c" -o "$OB/$f.o" || { echo "FUMUS FRACTUS: lib/$f.c" >&2; exit 1; }
    obj="$obj $OB/$f.o"
done
for src in gesta/fontes/*.c; do
    b=$(basename "$src" .c)
    gcc $FLAGS -c "$src" -o "$OB/g_$b.o" || { echo "FUMUS FRACTUS: $src" >&2; exit 1; }
    obj="$obj $OB/g_$b.o"
done
gcc $FLAGS -c gesta/instrumenta/tabulariumd_principale.c -o "$OB/princ.o" || exit 1
gcc $VFLAGS -c vendor/sqlite3.c -o "$OB/sqlite3.o" || exit 1
gcc "$OB/princ.o" $obj "$OB/sqlite3.o" -lm -o /tmp/tabulariumd || { echo "FUMUS FRACTUS: nexus" >&2; exit 1; }
echo "aedificatum: tabulariumd sub gcc/glibc" >&2

mkdir -p /tmp/mundus
/tmp/tabulariumd -portus 0 -scrinium /tmp/mundus/f.db \
    -annales /tmp/mundus/f.jsonl > /tmp/portus.txt 2>/tmp/daemon.log &
DPID=$!
PORT=""
for i in $(seq 1 25); do
    PORT=$(awk '{print $2}' /tmp/portus.txt 2>/dev/null)
    [ -n "$PORT" ] && break
    sleep 0.2
done
if [ -z "$PORT" ]; then
    echo "FUMUS FRACTUS: daemon non ortus" >&2
    cat /tmp/daemon.log >&2
    kill "$DPID" 2>/dev/null
    exit 1
fi
echo "daemon vivit: portus $PORT" >&2

exec 3<>"/dev/tcp/127.0.0.1/$PORT" || { echo "FUMUS FRACTUS: connexio" >&2; kill "$DPID"; exit 1; }
printf '{"jsonrpc":"2.0","id":1,"method":"tools/call","params":{"name":"census","arguments":{}}}\n' >&3
RESPONSUM=""
IFS= read -r -t 10 RESPONSUM <&3
exec 3<&- 3>&-
kill "$DPID" 2>/dev/null

if printf '%s' "$RESPONSUM" | grep -q '"jsonrpc"'; then
    echo "FUMUS PLENUS: tabulariumd sub Linux respondet"
    printf '%s\n' "$RESPONSUM" | head -c 300
    echo ""
    exit 0
fi
echo "FUMUS FRACTUS: responsum vacuum aut alienum" >&2
printf '%s\n' "$RESPONSUM" >&2
exit 1
