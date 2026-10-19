#!/bin/bash
# tools/sonda.sh - SONDA: probatio temporaria (plagula C una, saepe sub
# build/) contra bibliothecas domus VERAS aedificatur et currit, sub
# sanitatoribus (ASan + UBSan) - suspicio de casu limitis ('chorda vacua
# hic fallit?') fit factum quod alius iterum currere potest, non
# argumentum. Audit 2026-10-17 (domain audit): nulla relatio cimicis sine
# sonda quae eum ostendit.
#
# Clausura per bin/aedilis (capita -> lib/), scriptum in build/sonda/<T>/
# (--exitus), obiecta in build/sonda/<T>/obiecta (--obiecta, thesaurus
# proprius PER SONDAM - sondae simul ab agentibus pluribus structae idem
# obiectum numquam simul scribunt; communis numquam tangitur). VEXILLA: '-O2 -g' ->
# '-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined
# -Wno-deprecated-declarations', '-Werror' demptum (sonda monita
# videt, non frangitur). -nudus: sine sanitatoribus (-O0 -g). -domus:
# vexilla domus INTACTA (-Werror, -O2) - structura qualis in portis, sed
# privata (migratio suitarum: ante/post conversionem idem iudicium).
# Substitutio probatur (transformatio tacita nihil probat).
#
# CUSTODIAE (2026-10-19: sondae involutionis i32 GB veros tangebant, una
# XXXIV horas pendebat; probatio_arbor_syntaxis conversa XLIX G
# COMPRESSA tenebat cum RSS CCLXI M): vestigium memoriae (MEM ex top:
# residens + compressa - macOS comprimit, RSS mentitur) > SONDA_MEMORIA_MB
# (IV M MB)
# -> SIGKILL et 'memoria excessa'; tempus > SONDA_MORA s (CXX) ->
# 'mora excessa'. Sub ASan etiam hard_rss_limit_mb/malloc_limit_mb (ASan
# ipse nominat allocationem nimiam). Sonda quae limitem tangit vitium
# ostendit SINE GB veris: mensuram involutionis demonstra (summa,
# allocatio parva), non scripturam totam.
#
# Usus: ./tools/sonda.sh [-nudus | -domus] VIA.c [argumenta...]
# Effusio: sondae effusio, deinde linea
#   SONDA: exitus N · ASan: <genus @ functio plagula:linea | nulla> ·
#          UBSan: K sedes
# Exitus: exitus sondae (ASan: non 0) · 2 usus aut structura fracta
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
NUDUS=0; DOMUS=0
[ "${1:-}" = "-nudus" ] && { NUDUS=1; shift; }
[ "${1:-}" = "-domus" ] && { DOMUS=1; shift; }
VIA="${1:-}"
[ -n "$VIA" ] && [ -f "$VIA" ] || { echo "usus: $0 [-nudus | -domus] VIA.c [argumenta...]" >&2; exit 2; }
shift
[ -x bin/aedilis ] || { echo "sonda: bin/aedilis deest (./tools/aedilis_struere.sh)" >&2; exit 2; }
T=$(basename "$VIA" .c)
D="$RADIX/build/sonda/$T"
O="$D/obiecta$([ "$NUDUS" -eq 1 ] && echo _nuda)$([ "$DOMUS" -eq 1 ] && echo _domus)"
mkdir -p "$D" "$O"
if ! bin/aedilis "$VIA" --exitus "$D" --obiecta "$O" > "$D/aedilis.log" 2>&1; then
    sed 's/^/  /' "$D/aedilis.log" | tail -5 >&2
    echo "sonda: aedilis fracta" >&2; exit 2
fi
if [ "$NUDUS" -eq 1 ]; then
    NOVA="-O0 -g"
else
    NOVA="-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined -Wno-deprecated-declarations"
fi
if [ "$DOMUS" -eq 1 ]; then
    NOVA="-O2 -g"
    cp "$D/struere.sh" "$D/struere_sonda.sh"
else
    sed -e "s/^VEXILLA=\"\\(.*\\) -O2 -g\"\$/VEXILLA=\"\\1 $NOVA\"/" -e '/^VEXILLA=/s/ -Werror//' \
        "$D/struere.sh" > "$D/struere_sonda.sh"
fi
if ! grep -qF -- " $NOVA\"" "$D/struere_sonda.sh"; then
    echo "sonda: substitutio vexillorum defecit ($D/struere.sh) - nihil aedificatum" >&2; exit 2
fi
if ! sh "$D/struere_sonda.sh" > "$D/struere.log" 2>&1; then
    grep -E 'error|Undefined' "$D/struere.log" | head -8 >&2
    echo "sonda: structura fracta ($D/struere.log)" >&2; exit 2
fi
grep -E 'warning:' "$D/struere.log" | grep -F "$VIA" | head -5 >&2
EXE="$D/$T"
[ -x "$EXE" ] || { echo "sonda: binarium absens ($EXE)" >&2; exit 2; }
LIMES_MB="${SONDA_MEMORIA_MB:-4096}"
MORA="${SONDA_MORA:-120}"
ASAN_OPTIONS="detect_leaks=0:abort_on_error=0:hard_rss_limit_mb=$LIMES_MB:malloc_limit_mb=$LIMES_MB" \
UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=0" \
    "$EXE" "$@" > "$D/effusio.log" 2>&1 &
PID=$!
custodia=""
initium=$(date +%s)
while kill -0 "$PID" 2>/dev/null; do
    mem=$(top -l 1 -pid "$PID" -stats mem 2>/dev/null | tail -1 | tr -d ' +-')
    mb=$(echo "$mem" | awk '/[0-9]+K$/ {printf "%d", $0/1024} /[0-9]+M$/ {printf "%d", $0+0} /[0-9]+G$/ {printf "%d", ($0+0)*1024} /[0-9]+T$/ {printf "%d", ($0+0)*1048576}')
    if [ -n "$mb" ] && [ "$mb" -gt "$LIMES_MB" ]; then
        kill -9 "$PID" 2>/dev/null; custodia="memoria excessa ($mem > $LIMES_MB MB, residens + compressa)"; break
    fi
    if [ $(( $(date +%s) - initium )) -gt "$MORA" ]; then
        kill -9 "$PID" 2>/dev/null; custodia="mora excessa (> $MORA s)"; break
    fi
    sleep 0.5
done
wait "$PID" 2>/dev/null
rc=$?
[ -n "$custodia" ] && echo "SONDA CUSTODIA: $custodia - sonda occisa" >> "$D/effusio.log"
cat "$D/effusio.log"
asan=$(grep -m1 -oE 'ERROR: AddressSanitizer: [a-z-]+' "$D/effusio.log" | sed 's/ERROR: AddressSanitizer: //')
locus=$(grep -A40 'ERROR: AddressSanitizer' "$D/effusio.log" \
    | grep -m1 -oE 'in [A-Za-z_][A-Za-z0-9_]* [A-Za-z0-9_]+\.[cm]:[0-9]+' | sed 's/^in //')
ub=$(grep -cE 'runtime error:' "$D/effusio.log")
[ -n "$asan" ] && asan="$asan @ $locus" || asan=nulla
echo "SONDA: exitus $rc · ASan: $asan · UBSan: $ub sedes${custodia:+ · CUSTODIA: $custodia}"
exit "$rc"
