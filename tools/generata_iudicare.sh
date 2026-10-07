#!/bin/bash
# tools/generata_iudicare.sh - PORTA GENERATORUM per fabricam (fabrica
# plan 5 T7a): omne artificium generatum COMMISSUM == regeneratio
# hodierna, iudice bin/fabrica (iudicare -plenus -omnia: regeneratio in
# area scripturae aut verificatio memorata cum clave ingressuum et
# mandati). Succedit tools/generata_probare.sh (stadia I-VIII): oraculum
# fabricae (tools/fabrica_oraculum.sh) consensum in omni artificio
# communi XXXIV cursibus ostendit (2026-10-02 - 10-07), et fabrica
# XCVIII artificia plura iudicat quae generata non videbat.
#
# Artificia NON commissa (build/, bin/, ~/.bin, verdicta) nominantur
# numero solo: commissionem non obstant (sanare ea sanat - ut
# _fabricam_exigere in silva.commissio).
#
# Usus: ./tools/generata_iudicare.sh
# Exitus: 0 omnia commissa recentia · 1 aliquod non recens (nominatur)
#         · 2 iudicare nequit (bin/fabrica deest, declarationes fractae,
#         sera tenta).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
if [ ! -x bin/fabrica ]; then
    echo "  FRACTUM bin/fabrica deest (./tools/fabrica_struere.sh)"
    echo "generata: FRACTA"
    exit 2
fi
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT

bin/fabrica iudicare -plenus -omnia > "$T/fabrica" 2>&1
rc=$?
if [ "$rc" -eq 2 ]; then
    echo "  FRACTUM bin/fabrica iudicare nequit (exitus 2):"
    tail -5 "$T/fabrica" | sed 's/^/      /'
    echo "generata: FRACTA"
    exit 2
fi

# 'VERDICTUM via - causa' -> 'via<TAB>VERDICTUM<TAB>causa'
sed -nE 's/^(RECENS|STALUM|IGNOTUM|NON IUDICATUM) ([^ ]+)( - (.*))?$/\2	\1	\4/p' \
    "$T/fabrica" | sort -u > "$T/verdicta.tsv"
cut -f1 "$T/verdicta.tsv" > "$T/viae"
# commissa: quae git sequitur
git ls-files -- $(cat "$T/viae") 2>/dev/null | sort -u > "$T/commissa"

iudicata=0
malae=0
while IFS='	' read -r via verdictum causa; do
    grep -qxF "$via" "$T/commissa" || continue
    iudicata=$((iudicata + 1))
    if [ "$verdictum" != "RECENS" ]; then
        echo "  $verdictum $via - $causa"
        malae=$((malae + 1))
    fi
done < "$T/verdicta.tsv"
non_commissa=$(( $(wc -l < "$T/verdicta.tsv") - iudicata ))

if [ "$iudicata" -eq 0 ]; then
    # porta quae nihil iudicat mortua est, non sana
    echo "  FRACTUM nulla artificia commissa iudicata"
    echo "generata: FRACTA"
    exit 2
fi
echo "  artificia commissa: $iudicata iudicata, $malae non recentia" \
    "(non commissa: $non_commissa, numero solo)"
if [ "$malae" -eq 0 ]; then
    echo "generata: sana"
    exit 0
fi
echo "generata: FRACTA"
exit 1
