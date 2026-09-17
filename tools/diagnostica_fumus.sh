#!/bin/bash
# tools/diagnostica_fumus.sh - porta natalis tools/diagnostica.sh
#
# I.   domus: plagulae .sh tractae (fixa crustae pathologica exclusa)
#      sine erratis, exitus 0;
# II.  plagula plantata '{ echo a': exitus 1, linea prima exacta,
#      excerptum cum sedibus AMBABUS in linea fontis una;
# III. suffixum sine cliente solum: exitus 2 nominatus;
# IV.  css plantata (B4): exitus 1, codex css;
# V.   TSV XII campos fert (XI sedes relatae, XII nota primariae),
#      ordine UNO per diagnosticum.
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT

git ls-files '*.sh' | grep -v '^probationes/fixa/crusta/' > "$T/domus.lst"
./tools/diagnostica.sh -machina $(cat "$T/domus.lst") > "$T/domus.tsv" 2> "$T/domus.err"
rc=$?
[ "$rc" -eq 0 ]; credo $? "I. domus exitus 0 ($(wc -l < "$T/domus.lst" | tr -d ' ') plagulae, rc $rc)"
! grep -q $'\terratum\t' "$T/domus.tsv"; credo $? "I. domus sine erratis"

printf '{ echo a' > "$T/plantata.sh"
./tools/diagnostica.sh "$T/plantata.sh" > "$T/plantata.out" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ]; credo $? "II. plantata exitus 1 (rc $rc)"
[ "$(head -1 "$T/plantata.out")" = "$T/plantata.sh:1:9: [erratum] crusta:grex/tok_clausura" ]
credo $? "II. linea prima: $(head -1 "$T/plantata.out")"
# excerptum: sedes AMBAE, et ambae in linea I, ergo linea fontis SEMEL
# et signa BINA sub ea. Ante sedes multiplices linea IV signum primarium
# sola ferebat; nunc apertura eam tenet et primaria in lineam V cadit -
# mutatio CONSULTA, hic mensurata.
[ "$(sed -n '3p' "$T/plantata.out")" = "   1 | { echo a" ]
credo $? "II. linea fontis SEMEL (sedes ambae in linea I)"
[ "$(sed -n '4p' "$T/plantata.out")" = "     | ^~~~~~~~ hic coepit" ]
credo $? "II. signum aperturae derivatae"
[ "$(sed -n '5p' "$T/plantata.out")" = "     |         ^ hic exspectatur" ]
credo $? "II. signum primariae sub columna IX"
[ "$(wc -l < "$T/plantata.out" | tr -d ' ')" = "5" ]
credo $? "II. lineae quinque, non plures (nulla linea fontis iterata)"

# V. TSV: XII campi, sedes relatae in XI, nota primaria in XII
./tools/diagnostica.sh -machina "$T/plantata.sh" > "$T/plantata.tsv" 2>/dev/null
[ "$(awk -F'\t' '{print NF}' "$T/plantata.tsv")" = "12" ]
credo $? "V. TSV XII campos fert ($(awk -F'\t' '{print NF}' "$T/plantata.tsv"))"
[ "$(awk -F'\t' '{print $11}' "$T/plantata.tsv")" = "1:1-1:9|hic coepit" ]
credo $? "V. campus XI sedem relatam cum nota fert"
[ "$(awk -F'\t' '{print $12}' "$T/plantata.tsv")" = "hic exspectatur" ]
credo $? "V. campus XII notam primariae fert"
# ordo UNUS diagnosticum unum manet: numerus qui TSV quaerit non crescit
[ "$(wc -l < "$T/plantata.tsv" | tr -d ' ')" = "1" ]
credo $? "V. ordo unus, non ordines per sedem"

printf 'x\n' > "$T/x.txt"
./tools/diagnostica.sh "$T/x.txt" > /dev/null 2> "$T/x.err"
rc=$?
[ "$rc" -eq 2 ] && grep -q 'suffixum sine cliente' "$T/x.err"; credo $? "III. suffixum sine cliente: exitus 2 nominatus (rc $rc)"

printf 'a { color: red; } }' > "$T/plantata.css"
./tools/diagnostica.sh -machina "$T/plantata.css" > "$T/css.tsv" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ] && grep -q $'\terratum\tregula-mala\t' "$T/css.tsv"; credo $? "IV. css plantata exitus 1, codex regula-mala (rc $rc)"
# TSV codicem CRUDUM fert (sine praefixo grammaticae), linea humana non
! grep -q 'css:regula-mala' "$T/css.tsv"; credo $? "IV. TSV codicem crudum fert"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus diagnostica: sanum"; exit 0; fi
echo "fumus diagnostica: FRACTUM ($fracta)"; exit 1
