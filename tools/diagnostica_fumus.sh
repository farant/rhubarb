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
#      numero linearum quoque fixis; ordo pravus recusatus;
# XII.  gradus II per dispositorem (codex lintris, subtractio
#      declarata: '-nt' negatum TACET);
# XIII. excusatio declarata gradum II tegit;
# XIV. REVERSIO ordinis lintris (codex 'lint:' praefixum suum fert);
# XV.  scriptura fracta ORDO est (materia:scriptura cum sede), non
#      refutatio muta;
# XVI. instrumentum ex QUOVIS cwd regulas invenit;
# XVII. lintrum VACUUM refutatio est (exitus 2 nominatus), non exitus 0;
# XVIII. ordo SINE SEDE refutatio est, non silentium (paritas cum
#      extractore priore);
# XIX. excusatio LINTRIS mortua nominatur (EX8 clausum).
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

# XII-XIV. GRADUS II PER DISPOSITOREM. Ante hunc arcum regula lintris
# nusquam automatice currebat: instrumentum gradum I solum sciebat, et
# inventa gradus II per pythonica sola veniebant. Hae portae probant
# quod dispositor utrumque gradum fert, quod excusatio eum tegit, et
# quod subtractio declarata per instrumentum pervenit.
printf '#!/bin/bash\n[ $a -nt $b ]\n! [ $c -nt $d ]\n' > "$T/lint.sh"
./tools/diagnostica.sh "$T/lint.sh" > "$T/lint.out" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ]; credo $? "XII. gradus II ad instrumentum pervenit (rc $rc)"
grep -q 'lint:nt-aequalitas' "$T/lint.out"
credo $? "XII. codex lintris in forma humana"
# SUBTRACTIO: '-nt' NEGATUM inventum non est. Sine hac assertione
# porta duo inventa pro uno acciperet et differentiam declaratam
# nemo probaret.
[ "$(grep -c 'lint:nt-aequalitas' "$T/lint.out")" = "1" ]
credo $? "XII. subtractio declarata: negatum TACET ($(grep -c 'lint:nt-aequalitas' "$T/lint.out") inventum)"

printf '#!/bin/bash\n# <tolera codex="lint:nt-aequalitas" (>consulto\n[ $a -nt $b ]\n' \
    > "$T/lint_excusatus.sh"
./tools/diagnostica.sh "$T/lint_excusatus.sh" > "$T/lint_exc.out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] && [ ! -s "$T/lint_exc.out" ]
credo $? "XIII. excusatio declarata gradum II tegit (rc $rc)"

# XIV. REVERSIO ordinis lintris. Codex 'lint:' praefixum SUUM fert,
# ergo pictor grammaticam ei non praefigit - res quam ordo gradus I
# (qui 'crusta:' accipit) probare non potest.
./tools/diagnostica.sh -machina "$T/lint.sh" 2>/dev/null \
    | ./tools/diagnostica.sh -lege > "$T/lint.pertsv" 2>/dev/null
diff -q "$T/lint.out" "$T/lint.pertsv" >/dev/null
credo $? "XIV. lint: REVERSIO octetim"

# XV. SCRIPTURA FRACTA ORDO EST, NON SILENTIUM. Valor cuius spatium
# extremum lineam novam fert in elemento mixto a scriptore RECUSATUR
# (crusta P7). Per faciem ea recusatio ordinem 'materia:scriptura' cum
# sede reddit; refutatio muta plagulam vitiosam a plagula sana non
# distingueret, quia utraque 'nihil iudicatum' esset.
printf "a 'b \n" > "$T/scriptura.sh"
./tools/diagnostica.sh "$T/scriptura.sh" > "$T/scriptura.out" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ]; credo $? "XV. scriptura fracta: exitus 1, non 2 (rc $rc)"
grep -q 'materia:scriptura' "$T/scriptura.out"
credo $? "XV. codex materia:scriptura in exitu"
[ "$(head -1 "$T/scriptura.out")" = "$T/scriptura.sh:1:3: [erratum] materia:scriptura" ]
credo $? "XV. sede sua nominata: $(head -1 "$T/scriptura.out")"

# XVI. EX QUOVIS CWD. Via regularum ordinaria RELATIVA est
# (crusta/lintrum), ergo instrumentum ALIUNDE curritum eas invenire non
# poterat - exitus 2 pro plagula quaque .sh (regressio mensurata
# 2026-09-18, post dispositorem: ante eum instrumentum ex quovis cwd
# currebat). Involucrum ambitum CRUSTA_LINTRUM ABSOLUTE ponit; vocans
# qui eum ipse ponit vincit (modus evolutionis).
printf '#!/bin/bash\n[ $a -nt $b ]\n' > "$T/alibi.sh"
( cd "$T" && unset CRUSTA_LINTRUM && "$RADIX/tools/diagnostica.sh" "$T/alibi.sh" ) \
    > "$T/alibi.out" 2>&1
rc=$?
[ "$rc" -eq 1 ] && grep -q 'lint:nt-aequalitas' "$T/alibi.out"
credo $? "XVI. ex cwd alieno: regulae inventae (rc $rc)"

# XVII. LINTRUM VACUUM REFUTATIO EST, NON SANITAS. Directorium quod
# exstat sed regulam nullam fert gradum II tacite deponeret et exitum 0
# redderet - a plagula vere sana INDISTINGUIBILE. Id est ipsa forma
# quam Fran timuit (regulae additae quae non currunt), et causa propter
# quam numerus regularum ante iudicium ullum sciendus est.
mkdir -p "$T/lintrum_vacuum"
( cd "$RADIX" && CRUSTA_LINTRUM="$T/lintrum_vacuum" \
    ./tools/diagnostica.sh "$T/lint.sh" ) > "$T/vacuum.out" 2>&1
rc=$?
[ "$rc" -eq 2 ]; credo $? "XVII. lintrum vacuum: exitus 2, non 0 (rc $rc)"
grep -q 'regula nulla' "$T/vacuum.out"
credo $? "XVII. causa NOMINATA, non silentium"

# XVIII. ORDO SINE SEDE REFUTATIO EST. Regula quae CONGRUIT sed nodum
# non capit ('<situs/>') inventum locari non potest. Forma prior eum
# TACITE DEPONEBAT: regula bis congruens 'nihil inventum, exitus 0'
# reddebat - a plagula vere sana indistinguibile, et commentarium
# extractoris contrarium promittebat. Extractor prior (pythonica) id
# semper recusabat, ergo haec est PARITAS, non mos novus.
mkdir -p "$T/lintrum_sine"
# FORMA CLAUDENS VITATUR: elementum 'crusta-' clausum in valore huius
# plagulae ipsius sederet, et scriptor talem valorem RECUSAT (limes
# formae crudae, 01M2KPJ0HW) - id est, haec porta plagulam suam
# inproiectabilem redderet et porta I rubra fieret. Mensuratum
# 2026-09-18; forma se ipsam claudens eandem regulam dat sine valore.
cat > "$T/lintrum_sine/sine.stml" <<'STML'
<EXEMPLAR output="$c"><crusta-litteralis $t/></EXEMPLAR>
<relatum lint="sine-sede"><PER congruentia="$c"><situs/></PER></relatum>
STML
( cd "$RADIX" && CRUSTA_LINTRUM="$T/lintrum_sine" \
    ./tools/diagnostica.sh "$T/lint.sh" ) > "$T/sine.out" 2>&1
rc=$?
[ "$rc" -eq 2 ]; credo $? "XVIII. ordo sine sede: exitus 2, non 0 (rc $rc)"
grep -q 'sine sede' "$T/sine.out"
credo $? "XVIII. causa NOMINATA cum lintre"

# XIX. EXCUSATIO LINTRIS MORTUA NOMINATUR (EX8, apertum usque ad
# 2026-09-18). Antea codex 'lint:' a iudicio MORTUAE semper excipiebatur,
# quia instrumentum gradum II non currebat et excusationes verae OMNES
# mortuae videbantur (XIV domus rubram fecerunt). Facies gradum II nunc
# currit cum copia PLENA, ergo praemissa expiravit.
#
# ASYMMETRIA: eadem excusatio, sola VICTIMA differens. Porta XIII
# excusationem VIVAM tacere probat; haec MORTUAM clamare.
printf '#!/bin/bash\n# <tolera codex="lint:nt-aequalitas" (>nihil hic est\necho ok\n' \
    > "$T/lint_mortua.sh"
./tools/diagnostica.sh "$T/lint_mortua.sh" > "$T/lint_mortua.out" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ]; credo $? "XIX. excusatio lintris mortua: exitus 1 (rc $rc)"
grep -q 'materia:excusatio-mortua' "$T/lint_mortua.out"
credo $? "XIX. codex excusatio-mortua nominatus"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus diagnostica: sanum"; exit 0; fi
echo "fumus diagnostica: FRACTUM ($fracta)"; exit 1
