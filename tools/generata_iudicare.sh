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
# plagula commissa SIGNUM GENERATI ferens iudicata esse debet - aliter
# 'GENERATUM sine iudicio' (declaratio deest aut deleta). Classes
# exclusae, causa nominata (EXCLUSA infra).
#
# SIGNUM (unitates-codicis U4, 2026-10-15; Franus: via A): directio
# INVERSA quoque - omne artificium commissum signum ferre debet, aliter
# 'SINE SIGNO' (formator, iudices unitatum eam ut manu scriptam
# tractarent; registra cocta signum in linea III ferebant). Signum =
# 'GENERATUM' in linea I, aut II post declarationem quam forma poscit
# (<?xml, <!doctype) - responsum UNUM: bin/fabrica quid (lib/unitates.c).
# Regiones SPLICITAE (artificium simul ingressus actionis suae: latina.h
# numeri, silva.h superficies) signum capitis non ferunt - campus V
# 'splicita' lineae IUDICIUM eas nominat.
#
# CUSTODIA MEMORABILIUM (2026-10-23): actio memorabilis cuius
# regeneratio legit quod ingressus declarati non tegunt -> verdictum
# RECENS cum causa 'MEMORIA NEGATA ...' (lib/fabrica.c): declaratio
# fracta, hic FRACTA nominata. GENERATA_AUDITUS=1 -> iudicare -audit:
# omnis memorabilis regeneratur et custoditur (lentius - auditus plenus).
#
# Usus: ./tools/generata_iudicare.sh
# Exitus: 0 omnia commissa recentia · 1 aliquod non recens (nominatur)
#         · 2 iudicare nequit (bin/fabrica deest, declarationes fractae,
#         sera tenta).
# <porta titulus="generata" genus="praeiudicium" tegit="*/aedificatio.stml aedificatio.canon aedificatio.stml fabrica.canon fabrica.stml" currit="tools/fabrica.c"/>
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

AUDITUS=()
[ "${GENERATA_AUDITUS:-0}" = 1 ] && AUDITUS=(-audit)
bin/fabrica iudicare -plenus -omnia -machina ${AUDITUS[@]+"${AUDITUS[@]}"} > "$T/fabrica" 2>&1
rc=$?
if [ "$rc" -eq 2 ]; then
    echo "  FRACTUM bin/fabrica iudicare nequit (exitus 2):"
    tail -5 "$T/fabrica" | sed 's/^/      /'
    echo "generata: FRACTA"
    exit 2
fi

# lineae machinae (fabrica-6 H1): 'IUDICIUM<TAB>status<TAB>via<TAB>causa
# [<TAB>splicita]' -> 'via<TAB>status<TAB>causa' (viae cum spatiis quoque)
awk -F'\t' '$1 == "IUDICIUM" { print $3 "\t" $2 "\t" $4 }' "$T/fabrica" \
    | sort -u > "$T/verdicta.tsv"
awk -F'\t' '$1 == "IUDICIUM" && $5 == "splicita" { print $3 }' "$T/fabrica" \
    | sort -u > "$T/splicita"
cut -f1 "$T/verdicta.tsv" > "$T/viae"
# commissa: quae git sequitur
git ls-files -- $(cat "$T/viae") 2>/dev/null | sort -u > "$T/commissa"

# GENERATUM sine iudicio: classes exclusae (exemplar<TAB>causa)
EXCLUSA='*/probationes/fixa/*	fixum: copia congelata (caput fontis sui fert)
probationes/fixa/*	fixum: copia congelata (caput fontis sui fert)
knotapel/*/demo-snapshot.c	demonstratio congelata (knotapel/archive.sh)
docs/*.html	generator Pythonicus extra fabricam (docs/instrumenta/regenerare.sh) - parcum …PT44JPJA, signum post <!doctype visum 2026-10-15'
_exclusa () {
    local via="$1" exemplar
    while IFS='	' read -r exemplar _; do
        # shellcheck disable=SC2254
        case "$via" in $exemplar) return 0 ;; esac
    done <<< "$EXCLUSA"
    return 1
}
# provenientia omnium candidatorum (verbum GENERATUM alicubi) et
# artificiorum commissorum, vocatione UNA
{ git grep -l -I GENERATUM 2>/dev/null; cat "$T/commissa"; } | sort -u > "$T/rogandae"
# shellcheck disable=SC2046
if ! bin/fabrica quid -machina $(cat "$T/rogandae") > "$T/quid" 2> "$T/quid_err"; then
    echo "  FRACTUM bin/fabrica quid (signum generati) nequit:"
    sed 's/^/      /' "$T/quid_err" | head -5
    echo "generata: FRACTA"
    exit 2
fi
awk -F'\t' '$2 == "provenientia" && $3 == "generatum" { print $1 }' "$T/quid" \
    | sort -u > "$T/signatae"
sine_iudicio=0
while IFS= read -r via; do
    _exclusa "$via" && continue
    grep -qxF "$via" "$T/viae" && continue
    echo "  GENERATUM sine iudicio: $via (declaratio deest?)"
    sine_iudicio=$((sine_iudicio + 1))
done < "$T/signatae"
sine_signo=0
while IFS= read -r via; do
    grep -qxF "$via" "$T/signatae" && continue
    grep -qxF "$via" "$T/splicita" && continue
    _exclusa "$via" && continue
    echo "  SINE SIGNO: $via - artificium fabricae sine signo generati (generator 'GENERATUM' in linea I scribat, aut II post <?xml/<!doctype)"
    sine_signo=$((sine_signo + 1))
done < "$T/commissa"

iudicata=0
negatae=0
malae=$((sine_iudicio + sine_signo))
while IFS='	' read -r via verdictum causa; do
    if [[ "$causa" == "MEMORIA NEGATA"* ]]; then
        echo "  DECLARATIO FRACTA $via - $causa"
        negatae=$((negatae + 1))
    fi
    grep -qxF "$via" "$T/commissa" || continue
    iudicata=$((iudicata + 1))
    if [ "$verdictum" != "RECENS" ]; then
        echo "  $verdictum $via - $causa"
        malae=$((malae + 1))
    fi
done < "$T/verdicta.tsv"
malae=$((malae + negatae))
non_commissa=$(( $(wc -l < "$T/verdicta.tsv") - iudicata ))

if [ "$iudicata" -eq 0 ]; then
    # porta quae nihil iudicat mortua est, non sana
    echo "  FRACTUM nulla artificia commissa iudicata"
    echo "generata: FRACTA"
    exit 2
fi
echo "  artificia commissa: $iudicata iudicata, $((malae - negatae - sine_iudicio - sine_signo))" \
    "non recentia, $sine_iudicio GENERATUM sine iudicio, $sine_signo sine" \
    "signo ($(wc -l < "$T/splicita" | tr -d ' ') splicita; non commissa:" \
    "$non_commissa, numero solo); memoria negata: $negatae"
if [ "$malae" -eq 0 ]; then
    echo "generata: sana"
    exit 0
fi
echo "generata: FRACTA"
exit 1
