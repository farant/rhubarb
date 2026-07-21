#!/bin/bash

# tools/amalgama_fontes_generare.sh - listae amalgamatoris ex aedile
#
# AMALGAMA Phasis C: quattuor ordines plagularum manifesti
# (CAPITA_VENDICATA / CORPORA_VENDICATA / CAPITA_PROPRIA /
# CORPORA_PROPRIA) ut snippet generatum emittere, quem amalgamator.c
# manualis #includet. LISTAE derivatae (aedilis), POLITICA manualis
# (fontes_politica.sh iuxta manifestum: radices, exclusa,
# servanda/excludenda per basem). Ordo capitum = topologia stabilis
# (praelatio = snippet prior aut --enumerare binarii praesentis -
# migratio byte-identica); ordo corporum = praelatio (corpora ordine
# libera - probatio permutationis Phasis 0).
#
# Usus: ./tools/amalgama_fontes_generare.sh <proiectum>
#       (proiectum: silva | tessera | officina)

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

si_fracta() { echo "amalgama_fontes_generare: $1" >&2; exit 1; }

[ $# -eq 1 ] || si_fracta "usus: amalgama_fontes_generare.sh <proiectum>"
PROIECTUM="$1"
SEDES="$PROIECTUM/instrumenta/principalia"
POLITICA="$SEDES/fontes_politica.sh"
EXITUS="$SEDES/fontes_generata.h"
[ -f "$POLITICA" ] || si_fracta "politica deest: $POLITICA"
# shellcheck source=/dev/null
. "$POLITICA"

./tools/aedilis_struere.sh || exit 1

STATIO="build/aedilis/amalgama_fontes/$PROIECTUM"
rm -rf "$STATIO"
mkdir -p "$STATIO"

# ------------------------------------------------------------------
# derivatio: unio partium + aristarum super radices politicae
# ------------------------------------------------------------------
radices > "$STATIO/radices.txt"
[ -s "$STATIO/radices.txt" ] || si_fracta "radices vacuae"
: > "$STATIO/partes.tsv"
: > "$STATIO/aristae.tsv"
while IFS= read -r radix; do
    ./bin/aedilis "$radix" --partes >> "$STATIO/partes.tsv" \
        || si_fracta "derivatio fracta: $radix"
    ./bin/aedilis "$radix" --aristae >> "$STATIO/aristae.tsv" \
        || si_fracta "aristae fractae: $radix"
done < "$STATIO/radices.txt"
sort -u "$STATIO/aristae.tsv" > "$STATIO/aristae_unicae.tsv"

# copiae (exclusa politicae + latina = linea mechanismi)
_purgare() {
    while IFS= read -r v; do
        [ "$v" = "include/latina.h" ] && continue
        exclusa_est "$v" && continue
        echo "$v"
    done
}
awk -F'\t' '$1=="C" { print $2 }' "$STATIO/partes.tsv" | sort -u \
    | _purgare > "$STATIO/capita.txt"
{ awk -F'\t' '$1=="O" { print $2 }' "$STATIO/partes.tsv"
  cat "$STATIO/radices.txt"; } | sort -u \
    | _purgare > "$STATIO/corpora.txt"

# fissio vendicata/propria (propria = sub directorio proiecti)
grep    "^$PROIECTUM/" "$STATIO/capita.txt"  > "$STATIO/cp.txt" || true
grep -v "^$PROIECTUM/" "$STATIO/capita.txt"  > "$STATIO/cv.txt" || true
grep    "^$PROIECTUM/" "$STATIO/corpora.txt" > "$STATIO/op.txt" || true
grep -v "^$PROIECTUM/" "$STATIO/corpora.txt" > "$STATIO/ov.txt" || true

# ------------------------------------------------------------------
# praelatio ordinis: snippet prior si adest, alioquin --enumerare
# binarii amalgamatoris praesentis (semen migrationis)
# ------------------------------------------------------------------
if [ -f "$EXITUS" ]; then
    awk -F'"' '
        /CAPITA_VENDICATA\[\]/  { s = "CV" }
        /CORPORA_VENDICATA\[\]/ { s = "OV" }
        /CAPITA_PROPRIA\[\]/    { s = "CP" }
        /CORPORA_PROPRIA\[\]/   { s = "OP" }
        s != "" && /^    \{ "/ { print s "\t" $2 }
    ' "$EXITUS" > "$STATIO/praelatio.tsv"
else
    BINARIUM="$PROIECTUM/build/amalgamator"
    [ -x "$BINARIUM" ] \
        || si_fracta "praelatio deest: nec $EXITUS nec $BINARIUM (curre $PROIECTUM/amalgamare.sh prius)"
    "$BINARIUM" --enumerare > "$STATIO/praelatio.tsv" \
        || si_fracta "--enumerare fractum"
fi
for g in CV OV CP OP; do
    awk -F'\t' -v g="$g" '$1==g { print $2 }' \
        "$STATIO/praelatio.tsv" > "$STATIO/praelatio_$g.txt"
done

# candidati ordinati: praelatio (in copia) primum, novi sorted post
_candidati() {  # $1 = copia.txt, $2 = praelatio.txt
    awk 'NR==FNR { copia[$0]=1; next }
         ($0 in copia) && !(visa[$0]++) { print; emissa[$0]=1 }
         END { }' "$1" "$2"
    comm -23 <(sort "$1") <(sort "$2")
}

# topologia stabilis (Kahn, praelatio-prior; cyclus = fractura)
_topo() {  # $1 = candidati ordinati, $2 = aristae
    awk -F'\t' '
        NR==FNR { ordo[++n]=$0; in_copia[$0]=1; next }
        ($1 in in_copia) && ($2 in in_copia) {
            dep[$1] = dep[$1] SUBSEP $2 }
        END {
            numerus_emissorum = 0
            while (numerus_emissorum < n) {
                progressus = 0
                for (i = 1; i <= n; i++) {
                    c = ordo[i]
                    if (emissum[c]) continue
                    paratum = 1
                    m = split(dep[c], ds, SUBSEP)
                    for (j = 1; j <= m; j++) {
                        if (ds[j] != "" && !emissum[ds[j]]) {
                            paratum = 0; break } }
                    if (paratum) {
                        print c; emissum[c] = 1
                        numerus_emissorum++; progressus = 1 } }
                if (!progressus) {
                    print "CYCLUS" > "/dev/stderr"; exit 1 } } }
    ' "$1" "$2"
}

_candidati "$STATIO/cv.txt" "$STATIO/praelatio_CV.txt" > "$STATIO/cv_cand.txt"
_candidati "$STATIO/cp.txt" "$STATIO/praelatio_CP.txt" > "$STATIO/cp_cand.txt"
_topo "$STATIO/cv_cand.txt" "$STATIO/aristae_unicae.tsv" > "$STATIO/cv_ordo.txt" \
    || si_fracta "cyclus in capitibus vendicatis"
_topo "$STATIO/cp_cand.txt" "$STATIO/aristae_unicae.tsv" > "$STATIO/cp_ordo.txt" \
    || si_fracta "cyclus in capitibus propriis"
_candidati "$STATIO/ov.txt" "$STATIO/praelatio_OV.txt" > "$STATIO/ov_ordo.txt"
_candidati "$STATIO/op.txt" "$STATIO/praelatio_OP.txt" > "$STATIO/op_ordo.txt"

# validatio: ordo emissionis (CV+CP) contra aristas - defensio
cat "$STATIO/cv_ordo.txt" "$STATIO/cp_ordo.txt" > "$STATIO/capita_ordo.txt"
VIOLATIONES=$(awk -F'\t' '
    NR==FNR { ordo[$0]=FNR; next }
    ($1 in ordo) && ($2 in ordo) && ordo[$2] > ordo[$1] {
        printf "  %s ante %s\n", $1, $2 }
' "$STATIO/capita_ordo.txt" "$STATIO/aristae_unicae.tsv")
[ -z "$VIOLATIONES" ] || si_fracta "ordo emissus invalidus:
$VIOLATIONES"

# ------------------------------------------------------------------
# emissio
# ------------------------------------------------------------------
_basis() { b="${1##*/}"; echo "${b%.*}"; }

_ordines_emittere() {  # $1 = ordo.txt, $2 = est_corpus, $3 = est_vendicata
    primus=1
    while IFS= read -r v; do
        [ -n "$v" ] || continue
        [ $primus -eq 1 ] || echo ","
        primus=0
        b=$(_basis "$v")
        printf '    { "%s", %s, %s, %s, %s }' \
            "$v" "$(servanda_pro "$b")" "$(excludenda_pro "$b")" \
            "$2" "$3"
    done < "$1"
    echo ""
}

{
    echo "/* fontes_generata.h - GENERATUM AB AEDILE - NE MANU EDITES"
    echo " *"
    echo " * Listae plagularum amalgamatoris ($PROIECTUM) ex clausuris"
    echo " * derivatis; politica (servanda/excludenda/exclusa) manualis in"
    echo " * fontes_politica.sh + amalgamator.c manet. Ordo capitum ="
    echo " * topologia stabilis (praelatio = emissio prior)."
    echo " * Regeneratio: ./tools/amalgama_fontes_generare.sh $PROIECTUM"
    echo " */"
    echo ""
    echo "interior constans AmalgamaPlagula CAPITA_VENDICATA[] = {"
    _ordines_emittere "$STATIO/cv_ordo.txt" FALSUM VERUM
    echo "};"
    echo ""
    echo "interior constans AmalgamaPlagula CORPORA_VENDICATA[] = {"
    _ordines_emittere "$STATIO/ov_ordo.txt" VERUM VERUM
    echo "};"
    echo ""
    echo "interior constans AmalgamaPlagula CAPITA_PROPRIA[] = {"
    _ordines_emittere "$STATIO/cp_ordo.txt" FALSUM FALSUM
    echo "};"
    echo ""
    echo "interior constans AmalgamaPlagula CORPORA_PROPRIA[] = {"
    _ordines_emittere "$STATIO/op_ordo.txt" VERUM FALSUM
    echo "};"
} > "$STATIO/fontes_generata.h.novum"

mv "$STATIO/fontes_generata.h.novum" "$EXITUS"
echo "amalgama_fontes_generare ($PROIECTUM): $EXITUS ($(wc -l < "$STATIO/capita_ordo.txt" | tr -d ' ') capita, $(cat "$STATIO/ov_ordo.txt" "$STATIO/op_ordo.txt" | wc -l | tr -d ' ') corpora)"
