#!/bin/bash
# tools/generata_iudicare.sh - PORTA GENERATORUM per fabricam (fabrica
# plan 5 T7a): omne artificium generatum COMMISSUM == regeneratio
# hodierna, iudice bin/fabrica (iudicare -plenus -omnia: regeneratio in
# area scripturae aut verificatio memorata cum clave ingressuum et
# mandati). Succedit tools/generata_probare.sh (stadia I-VIII; deleta
# T7c, in historia git): oraculum fabricae (fabrica_oraculum.sh, item
# deletum) consensum in omni artificio communi XXXIV cursibus ostendit
# (2026-10-02 - 10-07), et fabrica XCVIII artificia plura iudicat quae
# generata non videbat.
#
# Artificia NON commissa (build/, bin/, ~/.bin, verdicta) nominantur
# numero solo: commissionem non obstant (sanare ea sanat - ut
# _fabricam_exigere in silva.commissio).
#
# TEGMEN (T7b, succedit oraculo fabricae 'declaratio omissa'): omnis
# plagula commissa cuius linea prima GENERATUM fert iudicata esse debet
# - aliter 'GENERATUM sine iudicio' (declaratio deest aut deleta).
# Classes exclusae, causa nominata (EXCLUSA infra).
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

bin/fabrica iudicare -plenus -omnia -machina > "$T/fabrica" 2>&1
rc=$?
if [ "$rc" -eq 2 ]; then
    echo "  FRACTUM bin/fabrica iudicare nequit (exitus 2):"
    tail -5 "$T/fabrica" | sed 's/^/      /'
    echo "generata: FRACTA"
    exit 2
fi

# lineae machinae (fabrica-6 H1): 'IUDICIUM<TAB>status<TAB>via<TAB>causa'
# -> 'via<TAB>status<TAB>causa' (viae cum spatiis quoque)
awk -F'\t' '$1 == "IUDICIUM" { print $3 "\t" $2 "\t" $4 }' "$T/fabrica" \
    | sort -u > "$T/verdicta.tsv"
cut -f1 "$T/verdicta.tsv" > "$T/viae"
# commissa: quae git sequitur
git ls-files -- $(cat "$T/viae") 2>/dev/null | sort -u > "$T/commissa"

# GENERATUM sine iudicio: classes exclusae (exemplar<TAB>causa)
EXCLUSA='*/probationes/fixa/*	fixum: copia congelata (caput fontis sui fert)
probationes/fixa/*	fixum: copia congelata (caput fontis sui fert)
knotapel/*/demo-snapshot.c	demonstratio congelata (knotapel/archive.sh)
gesta/annales/tabula.md	proiectio tabularii (residens scribit, non generator)'
_exclusa () {
    local via="$1" exemplar
    while IFS='	' read -r exemplar _; do
        # shellcheck disable=SC2254
        case "$via" in $exemplar) return 0 ;; esac
    done <<< "$EXCLUSA"
    return 1
}
sine_iudicio=0
while IFS= read -r via; do
    head -1 "$via" 2>/dev/null | grep -q GENERATUM || continue
    _exclusa "$via" && continue
    grep -qxF "$via" "$T/viae" && continue
    echo "  GENERATUM sine iudicio: $via (declaratio deest?)"
    sine_iudicio=$((sine_iudicio + 1))
done < <(git grep -l -I GENERATUM 2>/dev/null)

iudicata=0
malae=$sine_iudicio
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
echo "  artificia commissa: $iudicata iudicata, $((malae - sine_iudicio))" \
    "non recentia, $sine_iudicio GENERATUM sine iudicio (non commissa:" \
    "$non_commissa, numero solo)"
if [ "$malae" -eq 0 ]; then
    echo "generata: sana"
    exit 0
fi
echo "generata: FRACTA"
exit 1
