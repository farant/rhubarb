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
#   XIV  custos (colloquia T8b): incipere -> mensura 'cursus.custos' cum
#        pid vocantis; currere -> pid involucri; una per cursum
#   XV   factum (colloquia T8c): 'cursus factum -titulus -valor
#        [-unitas]' mensuram cursui aperto adhaeret; sine cursu 2
#   XVI  currentes (colloquia T8d): custos vivus ostenditur, mortuus
#        celatur (numeratus), -machina columna vitae
#   XVII colloquium (colloquia T9): cursus summus 'colloquium:<id>'
#        (CLAUDE_CODE_SESSION_ID); currentes colloquium=<id> filtrat
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
#   XVIII graves (credo memoria T3): facta credo.* sectionis -> 'quaestio
#        graves' (-machina octeti, homini MiB), titulus membri
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
# fumus sub Claude Code currens sessionem eius hereditaret (T9) - XVII
# eam expresse ponit
unset CLAUDE_CODE_SESSION_ID
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

# XIV custos (colloquia T8b): mensura 'cursus.custos' parenti adhaerens -
# incipere: pid VOCANTIS (getppid; silva.py = processus qui finem
# scribet); currere: pid INVOLUCRI ipsius (exspectat prolem). Diarium
# legitur ANTE legere (quod condit)
"$RADIX/bin/mensor" cursus incipere -titulus fumus/custos > "$T/amb14"
id14=$(sed -n 's/^MENSOR_PARENS=//p' "$T/amb14")
c14=$(awk -F'\t' -v p="$id14" '$3 == p && $4 == "cursus.custos" { print $5 + 0 }' "$MENSOR_DIARIUM")
pp14=$("$RADIX/bin/mensor" cursus currere -titulus fumus/custos_currere -- /bin/sh -c 'echo $PPID')
idc14=$(awk -F'\t' '$1 == "cursus" && $2 == "initium" && $6 == "fumus/custos_currere" { print $3 }' "$MENSOR_DIARIUM")
cc14=$(awk -F'\t' -v p="$idc14" '$3 == p && $4 == "cursus.custos" { print $5 + 0 }' "$MENSOR_DIARIUM")
[ -n "$id14" ] && [ "$c14" = "$$" ] && [ -n "$idc14" ] && [ -n "$pp14" ] && [ "$cc14" = "$pp14" ] \
  && [ "$(awk -F'\t' -v p="$id14" '$3 == p && $4 == "cursus.custos"' "$MENSOR_DIARIUM" | wc -l | tr -d ' ')" = "1" ]
credo $? "XIV  custos: incipere = pid vocantis, currere = pid involucri (mensura parenti adhaerens)"

# XV factum (colloquia T8c): 'cursus factum' = mensura cursui aperto
# (MENSOR_PARENS) adhaerens in diario CURSUUM; sine cursu aut sine
# -valor exitus 2, nihil scriptum
"$RADIX/bin/mensor" cursus incipere -titulus fumus/factum > "$T/amb15"
id15=$(sed -n 's/^MENSOR_PARENS=//p' "$T/amb15")
env $(tr '\n' ' ' < "$T/amb15") "$RADIX/bin/mensor" cursus factum -titulus consilium.portae -valor 5 -unitas portae; r15=$?
f15=$(awk -F'\t' -v p="$id15" '$3 == p && $4 == "consilium.portae" { print ($5 + 0) "|" $6 }' "$MENSOR_DIARIUM")
n15a=$(wc -l < "$MENSOR_DIARIUM")
"$RADIX/bin/mensor" cursus factum -titulus consilium.portae -valor 7 2>/dev/null; r15b=$?
env $(tr '\n' ' ' < "$T/amb15") "$RADIX/bin/mensor" cursus factum -titulus consilium.portae 2>/dev/null; r15c=$?
n15b=$(wc -l < "$MENSOR_DIARIUM")
# diarium ORDINARIUM (MENSOR_DIARIUM non positum): diarium CURSUUM
# ($HOME/.rhubarb), non /tmp/mensurae.diarium 'addere' veteris - aliter
# factum in plagulam quam nemo condit caderet
mkdir -p "$T/domus15/.rhubarb"
env -u MENSOR_DIARIUM HOME="$T/domus15" MENSOR_PARENS="$id15" "$RADIX/bin/mensor" cursus factum \
    -titulus consilium.portae -valor 3 -unitas portae; r15d=$?
f15d=$(awk -F'\t' -v p="$id15" '$3 == p && $4 == "consilium.portae" { print $5 + 0 }' "$T/domus15/.rhubarb/mensurae.diarium" 2>/dev/null)
[ -n "$id15" ] && [ "$r15" -eq 0 ] && [ "$f15" = "5|portae" ] && [ "$r15b" -eq 2 ] && [ "$r15c" -eq 2 ] \
  && [ "$n15a" = "$n15b" ] && [ "$r15d" -eq 0 ] && [ "$f15d" = "3" ]
credo $? "XV   factum: mensura cursui aperto in diario cursuum; sine cursu aut valore 2, nihil scriptum"

# XVI currentes (colloquia T8d): cursus cuius custos VIVIT (concha
# dormiens) ostenditur cum custode; cuius custos ABIIT celatur et in
# -machina 'mortuus' fert. 'bash -c ...; true' furcam cogit (concha
# ultimum mandatum aliter exsequitur et custos = fumus ipse)
bash -c '"$0" cursus incipere -titulus fumus/mortuus > "$1"; true' "$RADIX/bin/mensor" "$T/amb16m"
bash -c '"$0" cursus incipere -titulus fumus/vivus > "$1"; sleep 30' "$RADIX/bin/mensor" "$T/amb16v" &
pv16=$!
n=0; until [ -s "$T/amb16v" ] || [ $n -ge 50 ]; do sleep 0.1; n=$((n + 1)); done
# pid ITERUM DATUS: custos (factum ultimum vincit) = processus vivus
# sed POST cursum natus (III s > laxitas II s) - mortuus iudicandus
"$RADIX/bin/mensor" cursus incipere -titulus fumus/iteratus > "$T/amb16i"
sleep 3
sleep 30 &
pn16=$!
MENSOR_PARENS=$(sed -n 's/^MENSOR_PARENS=//p' "$T/amb16i") "$RADIX/bin/mensor" cursus factum \
    -titulus cursus.custos -valor "$pn16" -unitas pid
"$RADIX/bin/mensor" quaestio currentes > "$T/c16" 2>&1; rc16=$?
"$RADIX/bin/mensor" quaestio currentes -machina > "$T/c16m" 2>&1
kill "$pv16" "$pn16" 2>/dev/null; wait "$pv16" "$pn16" 2>/dev/null
vi16=$(awk -F'\t' '$1 == "CURRENS" && $5 == "fumus/iteratus" { print $9 "|" $10 }' "$T/c16m")
vm16=$(awk -F'\t' '$1 == "CURRENS" && $5 == "fumus/mortuus" { print $10 }' "$T/c16m")
vv16=$(awk -F'\t' '$1 == "CURRENS" && $5 == "fumus/vivus" { print $9 "|" $10 }' "$T/c16m")
[ "$rc16" -eq 0 ] && grep -q "^fumus/vivus .*(custos $pv16)" "$T/c16" && ! grep -q 'fumus/mortuus' "$T/c16" \
  && grep -q '^celati: [1-9][0-9]* mortui' "$T/c16" && [ "$vm16" = "mortuus" ] && [ "$vv16" = "$pv16|vivus" ] \
  && [ "$vi16" = "$pn16|mortuus" ] && ! grep -q 'fumus/iteratus' "$T/c16"
credo $? "XVI  currentes: custos vivus ostenditur, mortuus et pid iterum datus celantur, -machina vitam fert"

# XVII colloquium (colloquia T9): cursus SUMMUS sub sessione Claude
# mensuram 'colloquium:<id>' fert; filius (MENSOR_PARENS) et cursus sine
# variabili nullam; currentes colloquium=<id> arborem eius solam
# ostendit, columna XVIII -machina id fert (XIX: fracta, T10a)
CLAUDE_CODE_SESSION_ID=sess-xvii "$RADIX/bin/mensor" cursus incipere -titulus fumus/colloquium > "$T/amb17"
id17=$(sed -n 's/^MENSOR_PARENS=//p' "$T/amb17")
env $(tr '\n' ' ' < "$T/amb17") CLAUDE_CODE_SESSION_ID=sess-xvii "$RADIX/bin/mensor" cursus incipere \
    -titulus fumus/colloquium_filius > "$T/amb17f"
idf17=$(sed -n 's/^MENSOR_PARENS=//p' "$T/amb17f")
"$RADIX/bin/mensor" cursus incipere -titulus fumus/sine_colloquio > "$T/amb17s"
ids17=$(sed -n 's/^MENSOR_PARENS=//p' "$T/amb17s")
c17=$(awk -F'\t' -v p="$id17" '$3 == p && $4 == "colloquium:sess-xvii"' "$MENSOR_DIARIUM" | wc -l | tr -d ' ')
cf17=$(awk -F'\t' -v p="$idf17" '$3 == p && $4 ~ /^colloquium:/' "$MENSOR_DIARIUM" | wc -l | tr -d ' ')
cs17=$(awk -F'\t' -v p="$ids17" '$3 == p && $4 ~ /^colloquium:/' "$MENSOR_DIARIUM" | wc -l | tr -d ' ')
"$RADIX/bin/mensor" quaestio currentes colloquium=sess-xvii -machina > "$T/c17m" 2>&1
"$RADIX/bin/mensor" quaestio currentes colloquium=sess-xvii > "$T/c17" 2>&1
r17=$(awk -F'\t' '$1 == "CURRENS" && $2 == 0 { print $5 "|" $18 }' "$T/c17m" | tr '\n' ' ')
f17=$(awk -F'\t' '$1 == "CURRENS" && $2 == 1 { print $5 "|" $18 }' "$T/c17m" | tr '\n' ' ')
[ "$c17" = "1" ] && [ "$cf17" = "0" ] && [ "$cs17" = "0" ] && [ "$r17" = "fumus/colloquium|sess-xvii " ] \
  && [ "$f17" = "fumus/colloquium_filius|sess-xvii " ] && grep -q '^fumus/colloquium .*\[colloquium sess-xvi\]' "$T/c17" \
  && ! grep -q 'fumus/sine_colloquio' "$T/c17"
credo $? "XVII colloquium: cursus summus id sessionis fert, filius et sine variabili nullum; currentes colloquium= filtrat"

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

# XVIII graves (credo memoria T3): sectio fumus/m#s cum factis credo.*
# (arena 48 MiB, auctus 1 MiB, vestigium 50 MiB) -> 'quaestio graves
# -machina' octetos reddit; titulus= membri sine '#' sectiones eius
# quaerit; homini MiB cum decima; membrum alienum nihil
"$RADIX/bin/mensor" cursus incipere -titulus 'fumus/m#s' > "$T/amb18"
id18=$(sed -n 's/^MENSOR_PARENS=//p' "$T/amb18")
for f18 in credo.arena_apex:50331648 credo.apex_auctus:1048576 credo.vestigium:52428800; do
    env $(tr '\n' ' ' < "$T/amb18") "$RADIX/bin/mensor" cursus factum -titulus "${f18%%:*}" -valor "${f18#*:}" -unitas octeti
done
"$RADIX/bin/mensor" cursus finire -id "$id18" -exitus 0 -duratio_ms 5
g18=$("$RADIX/bin/mensor" quaestio graves titulus=fumus/m -machina 2>&1)
h18=$("$RADIX/bin/mensor" quaestio graves praefixum=fumus/ 2>&1)
v18=$("$RADIX/bin/mensor" quaestio graves titulus=fumus/alius -machina 2>&1)
[ -n "$id18" ] && [ "$g18" = "$(printf 'GRAVIS\tfumus/m#s\t1\t50331648\t1048576\t52428800')" ] \
  && echo "$h18" | grep -qE '^ +48\.0 MiB  arena +48\.0  auctus +1\.0  vestigium +50\.0  n=1 +fumus/m#s$' \
  && [ -z "$v18" ]
credo $? "XVIII graves: facta credo.* per sectionem, octeti machinae, MiB homini, titulus membri sectiones eius"
[ "$fracta" -eq 0 ] || { echo "      g: $g18"; echo "      h: $h18"; echo "      v: $v18"; }

# XIX fusio (budget B0): commissio fusionis (filii II) deinde fusio
# (portae II) sub arbore ficta - 'quaestio fusio' commissionem ANTE
# fusionem et filios ordine durationis reddit; fusio altera sine fine
# (custos = hoc scriptum, vivum) 'currit'. Arbor radicis ex cwd computatur
# (MENSOR_ARBOR solum filiis hereditatur) - quaestio arborem quam
# commissio notavit accipit; diarium fumi proprium est
incipe_xix () { env $(cat "$1" 2>/dev/null | tr '\n' ' ') "$RADIX/bin/mensor" cursus incipere -titulus "$2"; }
id_de () { sed -n 's/^MENSOR_PARENS=//p' "$1"; }
incipe_xix /dev/null commissio > "$T/x19c"
A19=$(sed -n 's/^MENSOR_ARBOR=//p' "$T/x19c")
incipe_xix "$T/x19c" phasis:forma > "$T/x19c1"; incipe_xix "$T/x19c" porta:a > "$T/x19c2"
"$RADIX/bin/mensor" cursus finire -id "$(id_de "$T/x19c1")" -exitus 0 -duratio_ms 10 -nota ""
"$RADIX/bin/mensor" cursus finire -id "$(id_de "$T/x19c2")" -exitus 0 -duratio_ms 50 -nota ""
"$RADIX/bin/mensor" cursus finire -id "$(id_de "$T/x19c")" -exitus 0 -duratio_ms 60 -nota "commissum fumus"
incipe_xix /dev/null fusio > "$T/x19f"
incipe_xix "$T/x19f" porta:c > "$T/x19f1"; incipe_xix "$T/x19f" porta:b > "$T/x19f2"
"$RADIX/bin/mensor" cursus finire -id "$(id_de "$T/x19f1")" -exitus 0 -duratio_ms 80 -nota ""
"$RADIX/bin/mensor" cursus finire -id "$(id_de "$T/x19f2")" -exitus 0 -duratio_ms 200 -nota ""
"$RADIX/bin/mensor" cursus finire -id "$(id_de "$T/x19f")" -exitus 0 -duratio_ms 300 -nota "fusio viridis: 2 portae"
q19=$("$RADIX/bin/mensor" quaestio fusio arbor="$A19" -machina 2>&1)
f19=$(echo "$q19" | awk -F'\t' '$1 == "FUSIO_FILIUS" { printf "%s:%s:%s ", $2, $3, $5 }')
s19=$(echo "$q19" | awk -F'\t' '$1 == "FUSIO" { print $3 "|" $5 "|" $6 } $1 == "FUSIO_COMMISSIO" { print "C|" $5 "|" $6 }' | tr '\n' ' ')
incipe_xix /dev/null fusio > "$T/x19g"
q19b=$("$RADIX/bin/mensor" quaestio fusio arbor="$A19" -machina 2>&1)
[ "$f19" = "commissio:porta:a:50 commissio:phasis:forma:10 fusio:porta:b:200 fusio:porta:c:80 " ] \
  && [ "$s19" = "C|60|commissum fumus transiit|300|fusio viridis: 2 portae " ] \
  && echo "$q19b" | awk -F'\t' '$1 == "FUSIO" { exit ($3 == "currit") ? 0 : 1 }' \
  && ! "$RADIX/bin/mensor" quaestio fusio arbor=/fumus/nulla > /dev/null 2>&1
credo $? "XIX  fusio: commissio ante fusionem, filii ordine durationis, fusio sine fine currit, arbor sine fusione exitus 1"
[ "$fracta" -eq 0 ] || { echo "      q: $q19"; echo "      b: $q19b"; }

# XX interventio notare (budget B9 2b): repositorium parvum (commissa III;
# notatum HEAD~1: a.c mutatum, b.c additum - non caput, ut commissum
# notatum a contextu discernatur); fabrica legentes et portae debitae FICTAE
# (titulus per viam) - cursus 'interventio' commissum PLENUM, nota
# 'titulus [budget]', corpus = tituli affecti ordinati; -scopus angustat
R20="$T/repo20"; mkdir -p "$R20"
( cd "$R20" && git init -q . && printf 'a\n' > a.c && git add a.c \
  && git -c user.name=f -c user.email=f@f commit -qm unus \
  && printf 'a2\n' > a.c && printf 'b\n' > b.c && git add a.c b.c \
  && git -c user.name=f -c user.email=f@f commit -qm duo \
  && printf 'c\n' > c.c && git add c.c \
  && git -c user.name=f -c user.email=f@f commit -qm tres ) > /dev/null 2>&1
printf '#!/bin/sh\nshift; shift\nfor v in "$@"; do printf "LEGENS\\tprobationes_t/probatio_%%s\\tingressus:fasciculus\\t%%s\\n" "${v%%.c}" "$v"; done\n' > "$T/ff20.sh"
printf '#!/bin/sh\nshift\nprintf "DEBITUM\\tradix\\t0\\t%%s\\n" "$1"\n' > "$T/fp20.sh"
chmod +x "$T/ff20.sh" "$T/fp20.sh"
H20=$(cd "$R20" && git rev-parse HEAD~1)   # NON caput: commissum notatum, non contextus
o20=$(cd "$R20" && "$RADIX/bin/mensor" interventio notare -commissum HEAD~1 -titulus "B5 velocior" -budget B5 -fabrica "$T/ff20.sh" -portae "$T/fp20.sh" 2>&1); rc20=$?
o20s=$(cd "$R20" && "$RADIX/bin/mensor" interventio notare -commissum HEAD~1 -titulus "solum a" -scopus probationes_t/probatio_a -fabrica "$T/ff20.sh" -portae "$T/fp20.sh" 2>&1)
r20=$(legere)
i20=$(echo "$r20" | awk -F'\t' -v h="$H20" '$4 == "interventio" && $8 == h && $15 == "B5 velocior [B5]" { print $2 }' | head -1)
i20s=$(echo "$r20" | awk -F'\t' '$4 == "interventio" && $15 == "solum a" { print $2 }' | head -1)
a20=$("$RADIX/bin/mensor" cursus acta "$i20" 2>/dev/null | tr '\n' ' ')
a20s=$("$RADIX/bin/mensor" cursus acta "$i20s" 2>/dev/null | tr '\n' ' ')
[ "$rc20" -eq 0 ] && [ -n "$i20" ] \
  && [ "$a20" = "porta:radix probationes_t/probatio_a probationes_t/probatio_b " ] \
  && [ "$a20s" = "probationes_t/probatio_a " ] \
  && ! (cd "$R20" && "$RADIX/bin/mensor" interventio notare -commissum nullum -titulus x -fabrica "$T/ff20.sh" -portae "$T/fp20.sh" > /dev/null 2>&1)
credo $? "XX   interventio notare: commissum plenum, nota cum budget, tituli affecti ex legentibus et portis, scopus angustat, commissum ignotum exitus non 0"
[ "$fracta" -eq 0 ] || { echo "      o: $o20"; echo "      a: $a20 | s: $a20s"; }

echo
if [ "$fracta" -eq 0 ]; then echo "fumus mensor_cursus: sanum (XX/XX)"; exit 0; fi
echo "fumus mensor_cursus: FRACTUM ($fracta)"; exit 1
