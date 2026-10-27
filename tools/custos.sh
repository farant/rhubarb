#!/bin/bash
# tools/custos.sh - CUSTOS: mandatum quodlibet sub limite memoriae et
# temporis currere.
#
# Memoria = VESTIGIUM (phys_footprint) processus et omnium filiorum,
# compressa et permutata inclusa, per bin/mensor memoria
# (memoria_processus - lector unus custodiarum domus, ut tools/sonda.sh).
# RSS sub compressione macOS mentitur: 2026-10-27 tools/uncinus_mensura
# in ansa infinita allocans ~120 GB tenuit dum custos per RSS (ps -o rss)
# paene nihil videbat. Mensura omni 0.2 s (ansa illa ~3 GB/s crescebat).
#
# Limites: CUSTOS_MEMORIA_MB (MMMMXCVI MB ordinarie) -> SIGKILL toti
# arbori processuum et 'memoria excessa'; CUSTOS_MORA s (MMMDC
# ordinarie) -> 'mora excessa'.
#
# Usus: [CUSTOS_MEMORIA_MB=N] [CUSTOS_MORA=S] ./tools/custos.sh -- mandatum [argumenta...]
# Effusio: mandati effusio, deinde in stderr
#   CUSTOS: exitus N · vestigium apex M MB (limes L MB, compressa inclusa) · <intra fines | memoria excessa | mora excessa>
# Exitus: exitus mandati (137 si occisum) · 2 usus aut bin/mensor absens
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MENSOR="$RADIX/bin/mensor"
LIMES_MB="${CUSTOS_MEMORIA_MB:-4096}"
MORA="${CUSTOS_MORA:-3600}"
[ "${1:-}" = "--" ] && shift
[ $# -gt 0 ] || { echo "usus: $0 -- mandatum [argumenta...]" >&2; exit 2; }
[ -x "$MENSOR" ] || { echo "CUSTOS: bin/mensor abest (memoria legi nequit) - nihil cursum" >&2; exit 2; }

"$@" &
PID=$!

arbor () {
    local p="$1" f
    echo "$p"
    for f in $(pgrep -P "$p" 2>/dev/null); do
        arbor "$f"
    done
}

apex=0
initium=$SECONDS
custodia=""
while kill -0 "$PID" 2>/dev/null; do
    summa=0
    processus=$(arbor "$PID")
    for p in $processus; do
        o=$("$MENSOR" memoria "$p" -machina 2>/dev/null | awk -F'\t' '$1 == "MEMORIA" { print $3 }')
        summa=$(( summa + ${o:-0} ))
    done
    [ "$summa" -gt "$apex" ] && apex=$summa
    if [ $(( summa / 1048576 )) -gt "$LIMES_MB" ]; then
        kill -9 $processus 2>/dev/null
        custodia="memoria excessa (vestigium $(( summa / 1048576 )) MB > $LIMES_MB MB)"
        break
    fi
    if [ $(( SECONDS - initium )) -gt "$MORA" ]; then
        kill -9 $processus 2>/dev/null
        custodia="mora excessa (> $MORA s)"
        break
    fi
    sleep 0.2
done
wait "$PID"
exitus=$?
echo "CUSTOS: exitus $exitus · vestigium apex $(( apex / 1048576 )) MB (limes $LIMES_MB MB, compressa inclusa) · ${custodia:-intra fines}" >&2
exit "$exitus"
