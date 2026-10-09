#!/bin/bash
# tools/agmen_vectorizatio.sh - porta VECTORIZATIONIS agminis (agmen.h)
#
# Nuclei agminis C purum sunt quos clang (-O2) ipse vectorizat; celeritas
# ergo invisibilis est: si ansa vectorizari desinit (versio compilatoris
# nova, ramus aut vocatio non inserta in ansa), probationes virides
# manent dum celeritas perit. Haec porta id videt:
#
#   I    omnis ansa nota 'AGMEN VECTORIZANDA: <nucleus>' vectorizata
#        (-Rpass=loop-vectorize, vexilla domus)
#   II   nota una per nucleum publicum (agmen_*_modulo) - nucleus novus
#        sine nota non tacet
#   III  NULLA ansa alia vectorizata: via relata (agminis_*_scalaris)
#        scalaris manet, aliter oraculum et mensura NEON contra NEON
#
# In defectu remarcae 'missed'/'analysis' ansae nominantur (causa).
# Exitus 0 sana | 1 FRACTA | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
source tools/vexilla.sh
FONS=lib/agmen.c
T="$(mktemp -d "${TMPDIR:-/tmp}/agmen_vectorizatio.XXXXXX")" || exit 2
trap 'rm -rf "${T:?}"' EXIT
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }

if ! clang "${VEXILLA_C89[@]}" -Iinclude -c "$FONS" -o "$T/agmen.o" \
        -Rpass=loop-vectorize > "$T/remarcae" 2>&1; then
    echo "agmen_vectorizatio: $FONS non compilat:"; head -20 "$T/remarcae"
    echo "vectorizatio agminis: nihil actum"
    exit 2
fi

# lineae ansarum vectorizatarum
grep -E "^$FONS:[0-9]+:[0-9]+: remark: vectorized loop" "$T/remarcae" \
    | cut -d: -f2 | sort -n > "$T/vectorizatae"
# notae: linea notae + 1 = linea ansae
grep -n 'AGMEN VECTORIZANDA:' "$FONS" | sed -E 's/^([0-9]+):.*AGMEN VECTORIZANDA: ([a-z_]+).*/\1 \2/' \
    > "$T/notae"
[ -s "$T/notae" ] || { echo "agmen_vectorizatio: nullae notae in $FONS"; echo "vectorizatio agminis: FRACTA"; exit 1; }

# I
: > "$T/notatae"
while read -r linea nucleus; do
    ansa=$((linea + 1))
    echo "$ansa" >> "$T/notatae"
    if ! sed -n "${ansa}p" "$FONS" | grep -q '^ *per ('; then
        credo 1 "I    $nucleus: linea post notam ($ansa) non est ansa 'per'"
    elif grep -qx "$ansa" "$T/vectorizatae"; then
        latitudo=$(grep -E "^$FONS:$ansa:" "$T/remarcae" | sed -E 's/.*vectorization width: ([0-9]+).*/\1/')
        credo 0 "I    $nucleus vectorizatus (linea $ansa, latitudo $latitudo)"
    else
        credo 1 "I    $nucleus NON vectorizatus (linea $ansa)"
        clang "${VEXILLA_C89[@]}" -Iinclude -c "$FONS" -o /dev/null \
            -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize 2>&1 \
            | grep -E "^$FONS:[0-9]+:[0-9]+: remark" | grep -E ":($linea|$ansa|$((ansa + 1))|$((ansa + 2))|$((ansa + 3))):" \
            | sed 's/^/         /' | head -4
    fi
done < "$T/notae"

# II
grep -E '^agmen_[a-z_]+_modulo \($' "$FONS" | sed -E 's/^agmen_([a-z_]+)_modulo \($/\1/' | sort > "$T/nuclei"
cut -d' ' -f2 "$T/notae" | sort > "$T/notae_nomina"
if cmp -s "$T/nuclei" "$T/notae_nomina"; then
    credo 0 "II   nota una per nucleum ($(wc -l < "$T/nuclei" | tr -d ' ') nuclei)"
else
    credo 1 "II   nuclei et notae dispares: nuclei [$(tr '\n' ' ' < "$T/nuclei")] notae [$(tr '\n' ' ' < "$T/notae_nomina")]"
fi

# III
sort -n "$T/notatae" > "$T/notatae_s"
aliae=$(comm -23 "$T/vectorizatae" "$T/notatae_s" | tr '\n' ' ')
if [ -z "$aliae" ]; then
    credo 0 "III  nulla ansa alia vectorizata (via relata scalaris)"
else
    credo 1 "III  ansae non notatae vectorizatae (lineae $aliae) - via relata?"
fi

if [ "$fracta" -eq 0 ]; then
    echo "vectorizatio agminis: sana"
    exit 0
fi
echo "vectorizatio agminis: FRACTA"
exit 1
