#!/bin/bash
# tools/mensor_cursus_fumus.sh - porta natalis chassis cursuum (analytica
# T2): tools/cursus.sh + 'bin/mensor cursus currere|legere'.
#
# Diarium, volumen, acta TEMPORARIA (MENSOR_DIARIUM, MENSOR_VOLUMEN,
# MENSOR_DIURNA - taberna actorum, analytica T4) - nihil verum
# tangitur. Probat:
#   I    exitus, duratio, titulus ex annotatione <cursus>, argv
#   II   effusio et erratum TRANSMISSA (terminalis) ET capta (corpus in
#        diurna: columna acta = sigillum LXIV hex, 'cursus acta <id>'
#        corpus reddit)
#   III  nidificatio: scriptum chassis scriptum alterum vocat -> parens,
#        contextus HEREDITATUS (commissum idem; contextus in ambitu
#        praepositus vincit git - hereditas, non recomputatio)
#   IV   contextus in repositorio temporario: commissum = HEAD, ramus,
#        arbor = radix, sordidum 0 mundum / 1 mutatum
#   V    involucrum necatum (SIGKILL) -> INTERRUPTUS, non absens
#   VI   CURSUS_NULLUS=1 -> scriptum sine involucro currit, nihil notatum
#   VII  scriptum se ipsum iterum exsequens (exemplar ianuarum) -> cursus
#        UNUS, non duo
#   IX   'cursus incipere' lineas ambitus pro prolibus reddit (MENSOR_PARENS
#        = id novum, contextus); 'finire -exitus -nota' cursum claudit;
#        nidificatio per incipere; nota in columna XV (analytica T3:
#        producentes sine involucro - silva.porta, commissio); '-acta -'
#        corpus ex stdin in diurna (T4), cursus sine corpore: 'acta' exitus 1
#   X    purgare (T4 2c): regula ex STML (tools/analytica_retentio.stml
#        legitur), -siccum nihil mutat, purgatio vera corpus delet
#        ('cursus acta' -> deletum), regula absens aut deformis -> 2
#   XI   quaestiones (T5): commissio fixa cum porta fracta (corpus) ->
#        'quaestio ultimum' (status), 'quaestio acta' (corpus fracti
#        ultimi, quaestio VI), 'quaestio commissiones' (culpa nominata),
#        'quaestio numquam' (radix cum scripto non curso), quaestio
#        ignota / parametrum ignotum -> 2
#   XII  censura (T6): adoptor sine annotatione / versione alia /
#        annotatio sine prologo / titulus duplex -> exitus 1 singula
#        nominans; radix munda -> 0; ARBOR VIVA conformis (invariatio)
#   XIII curare (T6c): 'mensor curare -vis' condit, purgat regula,
#        notam ultimam scribit, se ut cursum 'mensor:curare' notat;
#        iteratum intra diem non purgat; cursus summus cum limite
#        minimo curationem SEPARATAM excitat (apparet intra X s)
#   VIII extra omne repositorium (contextus vacuus) -> cursus notatus,
#        commissum et ramus vacui, nulla ruina (planta D T2: setenv cum
#        NIHIL ruebat)
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="mensor-cursus" currit="bin/mensor" tegit="tools/cursus.sh tools/mensor.c lib/mensura.c include/mensura.h *.sh tools/unci-git/*"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
if [ ! -x bin/mensor ]; then
    echo "fumus mensor_cursus: bin/mensor absens (./tools/mensor_struere.sh) - nihil actum"
    exit 2
fi
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
export MENSOR_DIARIUM="$T/diarium.tsv" MENSOR_VOLUMEN="$T/v.volumen" MENSOR_DIURNA="$T/diurna"
# curatio separata (T6c) in fumo NON sponte: proles detracta diarium
# temporarium dum probationes legunt conderet - gradus XIII eam solus
# solvit
export MENSOR_CURARE_NULLUS=1
unset MENSOR_SESSIO MENSOR_PARENS MENSOR_COMMISSUM MENSOR_RAMUS MENSOR_ARBOR MENSOR_SORDIDUM MENSOR_MACHINA MENSOR_VOCATOR CURSUS_INTUS CURSUS_NULLUS
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
legere () { "$RADIX/bin/mensor" cursus legere -machina 2>/dev/null; }
campus () { awk -F'\t' -v t="$1" -v k="$2" '$1 == "CURSUS" && $4 == t { print $k }'; }

scriptum () {   # <via> <titulus> <corpus>
    printf '#!/bin/bash\n# <cursus titulus="%s" versio="I"/>\nsource "%s/tools/cursus.sh"\n%s\n' "$2" "$RADIX" "$3" > "$1"
    chmod +x "$1"
}

# I
scriptum "$T/primum.sh" "fumus/primum" 'sleep 0.2; exit 3'
"$T/primum.sh" alpha beta > /dev/null 2>&1; rc=$?
r=$(legere)
[ "$rc" -eq 3 ] && [ "$(echo "$r" | campus fumus/primum 5)" = "fractus" ] \
  && [ "$(echo "$r" | campus fumus/primum 6)" = "3" ] \
  && [ "$(echo "$r" | campus fumus/primum 7)" -ge 150 ] \
  && echo "$r" | campus fumus/primum 14 | grep -q "alpha beta"
credo $? "I    exitus 3 (fractus), duratio >= 150 ms, titulus ex annotatione, argv"

# II
scriptum "$T/secundum.sh" "fumus/secundum" 'echo "ad effusionem"; echo "ad erratum" >&2; exit 0'
"$T/secundum.sh" > "$T/o2" 2> "$T/e2"
acta=$(legere | campus fumus/secundum 13)
id2=$(legere | campus fumus/secundum 2)
"$RADIX/bin/mensor" cursus acta "$id2" > "$T/a2" 2>/dev/null
grep -qx "ad effusionem" "$T/o2" && grep -qx "ad erratum" "$T/e2" \
  && echo "$acta" | grep -qE '^[0-9a-f]{64}$' \
  && grep -q "ad effusionem" "$T/a2" && grep -q "ad erratum" "$T/a2"
credo $? "II   effusio et erratum transmissa et in actis capta"

# III
scriptum "$T/filius.sh" "fumus/filius" 'exit 0'
scriptum "$T/pater.sh" "fumus/pater" "\"$T/filius.sh\"; exit 0"
"$T/pater.sh" > /dev/null 2>&1
r=$(legere)
id_patris=$(echo "$r" | campus fumus/pater 2)
[ -n "$id_patris" ] && [ "$(echo "$r" | campus fumus/filius 3)" = "$id_patris" ] \
  && [ "$(echo "$r" | campus fumus/filius 8)" = "$(echo "$r" | campus fumus/pater 8)" ]
iii=$?
# hereditas, non recomputatio: contextus in ambitu praepositus vincit git
scriptum "$T/heres.sh" "fumus/heres" 'exit 0'
MENSOR_COMMISSUM=commissum-hereditatum MENSOR_RAMUS=ramus-hereditatus \
    "$T/heres.sh" > /dev/null 2>&1
r=$(legere)
[ $iii -eq 0 ] && [ "$(echo "$r" | campus fumus/heres 8)" = "commissum-hereditatum" ] \
  && [ "$(echo "$r" | campus fumus/heres 10)" = "ramus-hereditatus" ]
credo $? "III  nidificatio: parens filii = id patris, contextus hereditatus"

# IV
R="$T/repo"; mkdir -p "$R"
( cd "$R" && git init -q -b ramus-fumi . && echo a > a.txt && git add a.txt \
    && git -c user.email=f@f -c user.name=f commit -q -m i ) > /dev/null 2>&1
scriptum "$T/contextus.sh" "fumus/contextus" 'exit 0'
( cd "$R" && "$T/contextus.sh" ) > /dev/null 2>&1
echo b >> "$R/a.txt"
( cd "$R" && "$T/contextus.sh" ) > /dev/null 2>&1
r=$(legere | awk -F'\t' '$4 == "fumus/contextus"')
sha=$(git -C "$R" rev-parse HEAD)
radix_veri=$(cd "$R" && pwd -P)
[ "$(echo "$r" | head -1 | cut -f8)" = "$sha" ] && [ "$(echo "$r" | head -1 | cut -f10)" = "ramus-fumi" ] \
  && [ "$(echo "$r" | head -1 | cut -f11)" = "$radix_veri" ] \
  && [ "$(echo "$r" | head -1 | cut -f9)" = "0" ] && [ "$(echo "$r" | tail -1 | cut -f9)" = "1" ]
credo $? "IV   contextus: commissum HEAD, ramus, arbor, sordidum 0 -> 1"

# V
scriptum "$T/longum.sh" "fumus/longum" 'sleep 5; exit 0'
"$T/longum.sh" > /dev/null 2>&1 &
p=$!
# initium exspectatur (contextus git in arbore magna > 0.5 s sub porta)
n=0
until grep -q 'fumus/longum' "$MENSOR_DIARIUM" 2>/dev/null || [ $n -ge 100 ]; do
    sleep 0.1; n=$((n + 1))
done
sleep 0.2
{ kill -9 "$p"; pkill -9 -f "$T/longum.sh"; wait "$p"; } 2>/dev/null
[ "$(legere | campus fumus/longum 5)" = "interruptus" ]
credo $? "V    involucrum necatum -> interruptus"

# VI
scriptum "$T/nullus.sh" "fumus/nullus" 'echo vivus; exit 0'
o=$(CURSUS_NULLUS=1 "$T/nullus.sh" 2>&1)
[ "$o" = "vivus" ] && [ -z "$(legere | campus fumus/nullus 2)" ]
credo $? "VI   CURSUS_NULLUS: scriptum currit, nihil notatum"

# VII
scriptum "$T/iterans.sh" "fumus/iterans" 'if [ -z "${ITERANS:-}" ]; then export ITERANS=1; exec "$0" "$@"; fi; exit 0'
"$T/iterans.sh" > /dev/null 2>&1
[ "$(legere | campus fumus/iterans 2 | grep -c .)" -eq 1 ]
credo $? "VII  scriptum se iterum exsequens -> cursus unus"

# VIII
E="$T/extra"; mkdir -p "$E"
scriptum "$T/extra.sh" "fumus/extra" 'echo extra; exit 0'
o=$( cd "$E" && "$T/extra.sh" 2>&1 ); rc=$?
r=$(legere)
[ "$rc" -eq 0 ] && [ "$o" = "extra" ] && [ "$(echo "$r" | campus fumus/extra 5)" = "transiit" ] \
  && [ -z "$(echo "$r" | campus fumus/extra 8)" ] && [ -z "$(echo "$r" | campus fumus/extra 10)" ]
credo $? "VIII extra repositorium: contextus vacuus, cursus notatus, nulla ruina"

# IX
amb=$("$RADIX/bin/mensor" cursus incipere -titulus fumus/incipere)
id_p=$(echo "$amb" | sed -n 's/^MENSOR_PARENS=//p')
amb_f=$(env $(echo "$amb" | tr '\n' ' ') "$RADIX/bin/mensor" cursus incipere -titulus fumus/incipere_filius)
id_f=$(echo "$amb_f" | sed -n 's/^MENSOR_PARENS=//p')
printf 'corpus ex stdin\nlinea secunda\n' | "$RADIX/bin/mensor" cursus finire -id "$id_f" -exitus 2 \
    -duratio_ms 40 -nota "filius: 1/2 FRACTA" -titulus fumus/incipere_filius -acta -
"$RADIX/bin/mensor" cursus finire -id "$id_p" -exitus 0 -duratio_ms 90 -nota "pater sanus"
r=$(legere)
[ -n "$id_p" ] && echo "$amb" | grep -q '^MENSOR_COMMISSUM=' && echo "$amb" | grep -q '^MENSOR_SESSIO=' \
  && [ "$(echo "$r" | campus fumus/incipere_filius 3)" = "$id_p" ] \
  && [ "$(echo "$r" | campus fumus/incipere_filius 5)" = "fractus" ] \
  && [ "$(echo "$r" | campus fumus/incipere_filius 15)" = "filius: 1/2 FRACTA" ] \
  && [ "$(echo "$r" | campus fumus/incipere 5)" = "transiit" ] \
  && [ "$(echo "$r" | campus fumus/incipere 7)" = "90" ] \
  && [ "$(echo "$r" | campus fumus/incipere 15)" = "pater sanus" ] \
  && [ "$("$RADIX/bin/mensor" cursus acta "$id_f" 2>/dev/null)" = "$(printf 'corpus ex stdin\nlinea secunda')" ] \
  && ! "$RADIX/bin/mensor" cursus acta "$id_p" > /dev/null 2>&1
credo $? "IX   incipere/finire: ambitus prolis, nidificatio, exitus, nota, corpus ex stdin"

# X
cat > "$T/regula_nulla.stml" <<'R'
<retentio>
  <sana cruda="0" compressa="0"/>
  <fracta cruda="0" compressa="0"/>
  <ultimum servare="falsum"/>
  <oblivio dies="0"/>
</retentio>
R
printf '<retentio>\n  <sana cruda="7"/>\n</retentio>\n' > "$T/regula_mala.stml"
"$RADIX/bin/mensor" purgare -siccum -regula "$RADIX/tools/analytica_retentio.stml" > "$T/p0" 2>&1; rc0=$?
"$RADIX/bin/mensor" purgare -siccum -regula "$T/regula_nulla.stml" > "$T/p1" 2>&1; rc1=$?
"$RADIX/bin/mensor" cursus acta "$id2" > /dev/null 2>&1; rcs=$?
"$RADIX/bin/mensor" purgare -regula "$T/regula_nulla.stml" > "$T/p2" 2>&1; rc2=$?
"$RADIX/bin/mensor" cursus acta "$id2" > /dev/null 2> "$T/p3"; rc3=$?
"$RADIX/bin/mensor" purgare -regula "$T/nulla_talis.stml" > "$T/p4" 2>&1; rc4=$?
"$RADIX/bin/mensor" purgare -regula "$T/regula_mala.stml" > "$T/p5" 2>&1; rc5=$?
[ "$rc0" -eq 0 ] && [ "$rc1" -eq 0 ] && grep -q "siccum" "$T/p1" && grep -qE "delenda [1-9]" "$T/p1" \
  && [ "$rcs" -eq 0 ] && [ "$rc2" -eq 0 ] && [ "$rc3" -eq 1 ] && grep -q "deletum" "$T/p3" \
  && [ "$rc4" -eq 2 ] && grep -q "nulla_talis.stml" "$T/p4" && [ "$rc5" -eq 2 ] && grep -q "'sana compressa' deest" "$T/p5"
credo $? "X    purgare: regula domus legitur, siccum nihil mutat, vera delet, regula absens/deformis -> 2"

# XI
M="$RADIX/bin/mensor"
amb_c=$("$M" cursus incipere -titulus commissio)
id_c=$(echo "$amb_c" | sed -n 's/^MENSOR_PARENS=//p')
amb_g=$(env $(echo "$amb_c" | tr '\n' ' ') "$M" cursus incipere -titulus porta:xi)
id_g=$(echo "$amb_g" | sed -n 's/^MENSOR_PARENS=//p')
printf 'probatio_xi FRACTA\nlinea secunda\n' | "$M" cursus finire -id "$id_g" -exitus 1 -nota "xi: 0/1" -titulus porta:xi -acta -
"$M" cursus finire -id "$id_c" -exitus 1 -nota "SilvaError: porta xi non sana"
mkdir -p "$T/rx/tools"
printf '#!/bin/bash\n# <cursus titulus="fumus/annotatum" versio="I"/>\n' > "$T/rx/tools/annotatum.sh"
printf '#!/bin/bash\necho\n' > "$T/rx/tools/nudum.sh"
scriptum "$T/rx/tools/cursum.sh" "fumus/cursum" 'exit 0'
"$T/rx/tools/cursum.sh" > /dev/null 2>&1
u=$("$M" quaestio ultimum titulus=porta:xi -machina 2>&1)
a=$("$M" quaestio acta titulus=porta:xi status=fracta 2>&1)
cm=$("$M" quaestio commissiones n=1 -machina 2>&1)
nq=$("$M" quaestio numquam radix="$T/rx" 2>&1)
# limes decimalis: fumus/lentum 100,100 -> 300,300 (ratio III.0):
# limes 2.5 nominat, 3.5 non (parsatio decimalis vere legitur)
for dur in 100 100 300 300; do
    a_l=$("$M" cursus incipere -titulus fumus/lentum)
    "$M" cursus finire -id "$(echo "$a_l" | sed -n 's/^MENSOR_PARENS=//p')" -exitus 0 -duratio_ms $dur
done
l25=$("$M" quaestio lentescentes fenestra=2 limes=2.5 titulus=fumus/lentum -machina 2>&1 | grep -c '^LENTESCENS')
l35=$("$M" quaestio lentescentes fenestra=2 limes=3.5 titulus=fumus/lentum -machina 2>&1 | grep -c '^LENTESCENS')
sx=$("$M" quaestio sectiones titulus=nemo -machina 2>&1; echo "rc$?")
# homini: quaestio vacua causam nominat (stderr; titulus nudus -> similis)
hx=$("$M" quaestio historia titulus=xi 2>&1 >/dev/null)
lx=$("$M" quaestio lentescentes fenestra=2 limes=1.5 -machina 2>&1; echo "rc$?")
"$M" quaestio nulla_talis > /dev/null 2>&1; rq=$?
"$M" quaestio historia frobnica=1 > /dev/null 2>&1; rp=$?
echo "$u" | grep -q "	porta:xi	fractus	" \
  && [ "$a" = "$(printf 'probatio_xi FRACTA\nlinea secunda')" ] \
  && echo "$cm" | awk -F'\t' '$1 == "COMMISSIO" && $9 == "porta:xi"' | grep -q . \
  && echo "$nq" | grep -qx "tools/annotatum.sh" && echo "$nq" | grep -qx "tools/nudum.sh" \
  && ! echo "$nq" | grep -q "cursum.sh" && [ "$rq" -eq 2 ] && [ "$rp" -eq 2 ] \
  && [ "$sx" = "rc0" ] && [ "${lx##*rc}" = "0" ] && [ "$l25" = "1" ] && [ "$l35" = "0" ] \
  && echo "$hx" | grep -q "nihil congruit - titulus 'xi' nullum cursum habet; simillimi: porta:xi (n=1)"
credo $? "XI   quaestiones: ultimum, acta fracti, commissiones cum culpa, numquam; ignota -> 2; vacua homini nominat"

# XII
C="$T/cens"; mkdir -p "$C/a" "$C/m"
bonus() { printf '#!/bin/bash\n# <cursus titulus="%s" versio="I"/>\nsource "$(dirname "$0")/../tools/cursus.sh"\n' "$2" > "$1"; }
bonus "$C/a/bonus.sh" "cens/bonus"
printf '#!/bin/bash\nsource "$(dirname "$0")/../tools/cursus.sh"\n' > "$C/a/sine_annotatione.sh"
printf '#!/bin/bash\n# <cursus titulus="cens/vetus" versio="0"/>\nsource "$(dirname "$0")/../tools/cursus.sh"\n' > "$C/a/vetus.sh"
printf '#!/bin/bash\n# <cursus titulus="cens/mortuus" versio="I"/>\necho\n' > "$C/a/mortuus.sh"
bonus "$C/a/geminus1.sh" "cens/geminus"
bonus "$C/a/geminus2.sh" "cens/geminus"
bonus "$C/m/solus.sh" "cens/solus"
# uncus sine suffixo cum prologo sine annotatione -> vitium (universum
# scripta sine '.sh' quoque videt); via relativa 'cursus.sh' = prologus
printf '#!/bin/bash\nsource "$(git rev-parse --show-toplevel)/tools/cursus.sh"\n' > "$C/a/pre-commit"
printf '#!/bin/bash\n# <cursus titulus="cens/vicinus" versio="I"/>\nsource "$(dirname "$0")/cursus.sh"\n' > "$C/m/vicinus.sh"
"$M" cursus censura -radix "$C/a" > "$T/c1" 2>&1; rc1=$?
"$M" cursus censura -radix "$C/m" > "$T/c2" 2>&1; rc2=$?
"$M" cursus censura -radix "$RADIX" > "$T/c3" 2>&1; rc3=$?
[ "$rc1" -eq 1 ] && grep -q "sine_annotatione.sh" "$T/c1" && grep -q "vetus.sh" "$T/c1" \
  && grep -q "mortuus.sh" "$T/c1" && grep -q "cens/geminus" "$T/c1" && ! grep -q "bonus.sh" "$T/c1" \
  && grep -q "	pre-commit	" "$T/c1" && grep -q "adoptores 2" "$T/c2" \
  && [ "$rc2" -eq 0 ] && [ "$rc3" -eq 0 ]
credo $? "XII  censura: sine annotatione, versio alia, mortuus, geminus nominati; mundus 0; arbor viva conformis"
[ "$rc3" -eq 0 ] || sed 's/^/      /' "$T/c3" | head -8

# XIII
# corpus RECENS (purgatio gradus X priora iam delevit)
scriptum "$T/corpus13.sh" "fumus/corpus13" 'echo corpus decimum tertium; exit 0'
"$T/corpus13.sh" > /dev/null 2>&1
id13=$(legere | campus fumus/corpus13 2)
"$M" cursus acta "$id13" > /dev/null 2>&1; ante=$?
# cursus post legere (quod iam condit) - eius lineae in diario manent
# donec curare condat: probatio conditionis non vacua
scriptum "$T/plicandum.sh" "fumus/plicandum" 'exit 0'
"$T/plicandum.sh" > /dev/null 2>&1
d0=$(grep -c 'fumus/plicandum' "$MENSOR_DIARIUM" 2>/dev/null)
MENSOR_RETENTIO="$T/regula_nulla.stml" "$M" curare -vis > "$T/k1" 2>&1; rk1=$?
d1=$(grep -c 'fumus/plicandum' "$MENSOR_DIARIUM" 2>/dev/null)
"$M" cursus acta "$id13" > /dev/null 2> "$T/k1a"; post=$?
MENSOR_RETENTIO="$T/regula_nulla.stml" "$M" curare > "$T/k2" 2>&1; rk2=$?
k=$("$M" quaestio historia titulus=mensor:curare -machina 2>/dev/null)
nk=$(echo "$k" | grep -c '^HISTORIA')
ultima=$(echo "$k" | head -1 | cut -f11)
scriptum "$T/excitans.sh" "fumus/excitans" 'exit 0'
( unset MENSOR_CURARE_NULLUS; MENSOR_CURARE_LIMES=1 MENSOR_RETENTIO="$T/regula_nulla.stml" "$T/excitans.sh" ) > /dev/null 2>&1
n=0; nk3=0
until [ "$nk3" -ge 3 ] || [ $n -ge 100 ]; do
    sleep 0.1; n=$((n + 1))
    nk3=$("$M" quaestio historia titulus=mensor:curare -machina 2>/dev/null | grep -c '^HISTORIA')
done
[ "$ante" -eq 0 ] && [ "${d0:-0}" -ge 1 ] && [ "$rk1" -eq 0 ] && [ "${d1:-0}" = "0" ] && [ "$post" -eq 1 ] && grep -q "deletum" "$T/k1a" \
  && [ -f "$MENSOR_DIURNA/curatio.ultima" ] && [ "$rk2" -eq 0 ] && [ "$nk" -eq 2 ] \
  && echo "$ultima" | grep -q "purgatio non debita" && [ "$nk3" -ge 3 ]
credo $? "XIII curare: condit, purgat, notatur; intra diem non purgat; cursus summus curationem separatam excitat"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus mensor_cursus: sanum (XIII/XIII)"; exit 0; fi
echo "fumus mensor_cursus: FRACTUM ($fracta)"; exit 1
