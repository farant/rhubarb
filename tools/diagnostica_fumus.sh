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
#      ordine UNO per diagnosticum;
# VI.  REVERSIO: 'x' == '-machina x | -lege', octetim, exitibus et
#      numero linearum quoque fixis; ordo pravus recusatus.
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
# sedes relata OCTETOS quoque fert (@B-B): pictor octetis pingit, ergo
# forma sine eis reversionem reddere non posset - hoc porta VI invenit
[ "$(awk -F'\t' '{print $11}' "$T/plantata.tsv")" = "1:1-1:9@0-8|hic coepit" ]
credo $? "V. campus XI sedem relatam cum octetis et nota fert"
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

# VI. REVERSIO: forma humana per TSV eadem est ac recta. Assertio una
# convergentiam totam figit - si viae duae divergunt, altera pictorem
# suum genuit. EXITUS ET NUMERUS LINEARUM quoque figuntur: exitus 2
# (aedificatio fracta) ambo vacua redderet et 'diff' tacite transiret.
printf 'if true; then\n  echo x\n' > "$T/duae_lineae.sh"
for f in plantata duae_lineae; do
    ./tools/diagnostica.sh "$T/$f.sh" > "$T/$f.recta" 2>/dev/null
    rc_recta=$?
    ./tools/diagnostica.sh -machina "$T/$f.sh" 2>/dev/null \
        | ./tools/diagnostica.sh -lege > "$T/$f.pertsv" 2>/dev/null
    rc_tsv=$?
    n=$(wc -l < "$T/$f.recta" | tr -d ' ')
    [ "$rc_recta" -eq 1 ] && [ "$rc_tsv" -eq 1 ]
    credo $? "VI. $f: exitus 1 utrimque (recta $rc_recta, per TSV $rc_tsv)"
    [ "$n" -ge 5 ]
    credo $? "VI. $f: exitus rectus non vacuus ($n lineae)"
    diff -q "$T/$f.recta" "$T/$f.pertsv" >/dev/null
    credo $? "VI. $f: REVERSIO octetim ($(diff "$T/$f.recta" "$T/$f.pertsv" | head -2 | tr '\n' ' '))"
done

# VI. ordo pravus RECUSATUR, non tacite acceptus
printf 'a\tb\tc\n' | ./tools/diagnostica.sh -lege > /dev/null 2> "$T/pravus.err"
rc=$?
[ "$rc" -eq 2 ] && grep -q 'ordo TSV pravus' "$T/pravus.err"
credo $? "VI. ordo camporum pravorum: exitus 2 nominatus (rc $rc)"

# VII. EXCUSATIO DECLARATA: inventum gradus I excusatum CADIT, et
# exitus id sequitur. Forma codicis PICTA ('crusta:grex/tok_clausura')
# adhibetur consulto: id est quod usor in nuntio VIDIT, et codex nudus
# in TSV solo apparet. Si excusatio post numeros curreret, plagula
# tota excusata exitum 1 nihilominus redderet.
printf '# <tolera codex="crusta:grex/tok_clausura" (>consulto\n{ echo a\n' \
    > "$T/excusata.sh"
./tools/diagnostica.sh "$T/excusata.sh" > "$T/excusata.out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ]; credo $? "VII. plagula excusata: exitus 0 (rc $rc)"
[ ! -s "$T/excusata.out" ]
credo $? "VII. plagula excusata: nihil pictum ($(wc -l < "$T/excusata.out" | tr -d ' ') lineae)"

# VIII. EXCUSATIO MORTUA: quae nihil absorbuit se ipsam nominat.
# Aliter exemptio consulta a rancida distingui non potest.
printf '# <tolera codex="crusta:grex/tok_clausura" (>nihil hic\necho ok\n' \
    > "$T/mortua.sh"
./tools/diagnostica.sh -machina "$T/mortua.sh" > "$T/mortua.tsv" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ] && grep -q 'materia:excusatio-mortua' "$T/mortua.tsv"
credo $? "VIII. excusatio mortua nominatur, exitus 1 (rc $rc)"

# IX. PROSA NON EST ANNOTATIO. Domus '# <via>' pro SIGNATURA scribit
# (excubitor.sh:21, tools/sera.sh:39). Sine hac porta signatura
# quaeque 'excusatio fracta' fieret: XIII in domo mensuratae
# 2026-09-18, et porta I tota rubuit donec regula angustata est.
printf 'f () {   # <via> -> pid\n  echo x\n}\n' > "$T/prosa.sh"
./tools/diagnostica.sh -machina "$T/prosa.sh" > "$T/prosa.tsv" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] && ! grep -q 'excusatio' "$T/prosa.tsv"
credo $? "IX. '# <via>' prosa manet, non excusatio fracta (rc $rc)"

# X. '-excusa' OPTIO EST, NON MOS. Tabula ex '-machina' iam cribrata
# venit; applicatio altera excusationem VIVAM mortuam nominaret (eius
# victima primo transitu cecidit). Ergo pictor quod datur pingit.
# Tabula lintris hic FINGITUR ex plagula sine annotatione, sedibus ad
# annotationem (LIV octeti, linea una) motis - id est quod pythonica
# regulae emittit: numquam cribratum.
printf '{ echo a\n' > "$T/nuda.sh"
./tools/diagnostica.sh -machina "$T/nuda.sh" 2>/dev/null \
    | awk -F'\t' -v v="$T/excusata.sh" 'BEGIN{OFS="\t"}
        {$1=v; $2=$2+1; $4=$4+1; $6=$6+54; $7=$7+54; print}' \
    > "$T/lintris.tsv"
[ -s "$T/lintris.tsv" ]; credo $? "X. tabula lintris ficta non vacua"
./tools/diagnostica.sh -lege -excusa < "$T/lintris.tsv" \
    > "$T/cum.out" 2>/dev/null
rc_cum=$?
./tools/diagnostica.sh -lege < "$T/lintris.tsv" \
    > "$T/sine.out" 2>/dev/null
rc_sine=$?
[ "$rc_cum" -eq 0 ] && [ ! -s "$T/cum.out" ]
credo $? "X. '-lege -excusa' cribrat (rc $rc_cum)"
[ "$rc_sine" -eq 1 ] && [ -s "$T/sine.out" ]
credo $? "X. '-lege' solum PINGIT quod datur (rc $rc_sine)"

# XI. REVERSIO cum annotatione. Excusatio mortua transitum utrumque
# fert, ergo ordines BINOS pareret si '-lege' eam iterum applicaret -
# hoc exacte accidit antequam '-excusa' optio fieret (mensuratum
# 2026-09-18: I ordo recte, II per reversionem).
for f in excusata mortua; do
    ./tools/diagnostica.sh "$T/$f.sh" > "$T/$f.recta" 2>/dev/null
    ./tools/diagnostica.sh -machina "$T/$f.sh" 2>/dev/null \
        | ./tools/diagnostica.sh -lege > "$T/$f.pertsv" 2>/dev/null
    diff -q "$T/$f.recta" "$T/$f.pertsv" >/dev/null
    credo $? "XI. $f: REVERSIO octetim cum annotatione"
done

echo
if [ "$fracta" -eq 0 ]; then echo "fumus diagnostica: sanum"; exit 0; fi
echo "fumus diagnostica: FRACTUM ($fracta)"; exit 1
