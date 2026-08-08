#!/bin/sh
# apparatus_sh.sh - apparatum omnium generum ex proiectione SHELL emittere,
# eodem formato quo probator_naturae, ut differentia fieri possit.
# Ordo: catena a genere ipso ad radicem; intra gradum P, M, A, R
# (ordo natura_quaere.sh -a).
set -eu

CORPUS=build/natura_apparatus_umbra/corpus.txt
mkdir -p build/natura_apparatus_umbra
# corpus IPSE generat: natura_quaere.sh id non iam scribit - munus
# eius in bin/natura migravit 2026-08-08, et pendere ab effectu
# secundario alterius instrumenti fragile erat.
bin/natura_examen -corpus "$CORPUS" >/dev/null 2>&1 || true
[ -r "$CORPUS" ] || { echo "corpus generari nequit - strue ./tools/natura_struere.sh"; exit 2; }

# parens: modulus.genus -> modulus.genus
# CAMPI ut maiores() in natura_quaere.sh eos legit: $4 = modulus
# parentis, $5 = nomen parentis (NON e converso - error prior hic)
PARENS=$(mktemp)
awk -F'\t' '$1=="G"{ if($5!="") print $2"."$3"\t"$4"."$5 }' "$CORPUS" > "$PARENS"

awk -F'\t' '$1=="G"{print $2"\t"$3}' "$CORPUS" | while IFS='	' read -r m g; do
    cur="$m.$g"; i=0
    while [ -n "$cur" ] && [ "$i" -lt 64 ]; do
        cm=${cur%%.*}; cg=${cur#*.}
        awk -F'\t' -v m="$cm" -v g="$cg" -v om="$m" -v og="$g" \
            '$1=="P"&&$2==m&&$3==g{print om"\t"og"\tproprietas\t"$4"\t"m"\t"g}' "$CORPUS"
        awk -F'\t' -v m="$cm" -v g="$cg" -v om="$m" -v og="$g" \
            '$1=="A"&&$2==m&&$3==g{print om"\t"og"\tpars\t"$4"\t"m"\t"g}' "$CORPUS"
        awk -F'\t' -v m="$cm" -v g="$cg" -v om="$m" -v og="$g" \
            '$1=="M"&&$2==m&&$3==g{print om"\t"og"\tmachina_statuum\t"$4"\t"m"\t"g}' "$CORPUS"
        awk -F'\t' -v m="$cm" -v g="$cg" -v om="$m" -v og="$g" \
            '$1=="R"&&$2==m&&$3==g{print om"\t"og"\trelatio\t"$4"\t"m"\t"g}' "$CORPUS"
        cur=$(grep -m1 "^$cur	" "$PARENS" | cut -f2 || true)
        i=$((i + 1))
    done
done

rm -f "$PARENS"
