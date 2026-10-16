#!/bin/bash
# tools/tls_effluvia.sh - effluvia memoriae TLS per 'leaks --atExit'
# (macOS): probationes TLS (probatio_tls_servus: pactio vera contra
# s_server qui post responsum CLAUDIT -> tls_recipere clausa -> via
# tls_claudere ubi contextus SSL olim peribat, R5; probatio_tls_gradus)
# sub 'leaks' currunt; omne ROOT LEAK genere NON tolerato = FRACTUM.
#
# tools/tls_effluvia.tolerata: genera Apple interna nota (cum causa) -
# non nostra, numquam sanabilia hic. Effusio 'leaks' sine summario =
# exitus II (porta muta numquam viridis).
#
# Usus: ./tools/tls_effluvia.sh [-aedificare]   (ex radice repositorii;
#   -aedificare binaria probationum prius per compile_tests.sh struit)
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX"
TOLERATA="$SCRIPT_DIR/tls_effluvia.tolerata"
PROBATIONES="probatio_tls_servus probatio_tls_gradus"
T="$(mktemp -d "${TMPDIR:-/tmp}/tls_effluvia.XXXXXX")"
trap 'rm -rf "$T"' EXIT

if [ "${1:-}" = "-aedificare" ]; then
    for p in $PROBATIONES; do
        ./compile_tests.sh "$p" > "$T/aed_$p.log" 2>&1 || {
            echo "tls_effluvia: $p non aedificatum (vide $T)"; exit 2; }
    done
fi

tolerata=$(grep -v '^#' "$TOLERATA" | awk 'NF {print $1}')
fractae=0
for p in $PROBATIONES; do
    b="build/probationes/$p"
    [ -x "$b" ] || { echo "tls_effluvia: $b abest (-aedificare)"; exit 2; }
    log="$T/$p.leaks"
    leaks --atExit -- "$b" > "$log" 2>&1 &
    lp=$!
    # leaks processum inspectum interdum suspensum relinquit: summarium
    # exspectatur (tectum CLXXX s), deinde uterque necatur
    for k in $(seq 1 180); do
        # log nondum creatum in primis iterationibus: tacite
        grep -q ' leaks for ' "$log" 2>/dev/null && break
        sleep 1
    done
    pid=$(sed -n 's/^Process \([0-9]*\): .* leaks for .*/\1/p' "$log" | head -1)
    [ -n "$pid" ] && kill -9 "$pid" 2>/dev/null
    kill "$lp" 2>/dev/null; wait "$lp" 2>/dev/null
    summa=$(grep -m1 ' leaks for ' "$log")
    if [ -z "$summa" ]; then
        echo "tls_effluvia: $p - leaks SINE summario (mutum); cauda:"
        tail -5 "$log"
        exit 2
    fi
    # summa VERA ex summario; tolerata = instantiae singulae generum
    # toleratorum ('  1 (32 bytes) ROOT LEAK: <Genus ...'). Residuum > 0
    # = FRACTUM - effluvia nostra saepe ut 'ROOT CYCLE' apparent
    # (SSLContext, R5), non ut 'ROOT LEAK': numerare tantum radices
    # ROOT LEAK plantam R5 viridem relinquebat
    summa_n=$(echo "$summa" | sed -n 's/.*: \([0-9]*\) leaks for .*/\1/p')
    toleratae_n=0
    for g in $tolerata; do
        n=$(grep -v 'STACK OF' "$log" \
            | grep -cF "bytes) ROOT LEAK: <$g " || true)
        toleratae_n=$((toleratae_n + n))
    done
    residuum=$((summa_n - toleratae_n))
    if [ "$residuum" -gt 0 ]; then
        radices=$(grep -v 'STACK OF' "$log" \
            | sed -nE 's/.*\) ROOT (LEAK|CYCLE): <([^>]*)>.*/\1:\2/p' \
            | sed 's/ 0x[0-9a-f]*$//; s/ *$//' | sort | uniq -c | grep -v -F "$(echo "$tolerata" | sed 's/^/LEAK:/')" \
            | head -6 | tr '\n' ';')
        echo "tls_effluvia: $p FRACTUM - $residuum effluvia non tolerata [$summa] radices: $radices"
        mkdir -p build/tls_effluvia && cp "$log" "build/tls_effluvia/$p.leaks"
        fractae=$((fractae + 1))
    else
        echo "tls_effluvia: $p viride [$summa; tolerata $toleratae_n]"
    fi
done
[ "$fractae" -eq 0 ] || exit 1
echo "tls_effluvia: omnia virides"
