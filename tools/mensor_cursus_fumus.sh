#!/bin/bash
# tools/mensor_cursus_fumus.sh - porta natalis chassis cursuum (analytica
# T2): tools/cursus.sh + 'bin/mensor cursus currere|legere'.
#
# Diarium, volumen, acta TEMPORARIA (MENSOR_DIARIUM, MENSOR_VOLUMEN,
# MENSOR_ACTA) - nihil verum tangitur. Probat:
#   I    exitus, duratio, titulus ex annotatione <cursus>, argv
#   II   effusio et erratum TRANSMISSA (terminalis) ET capta (acta)
#   III  nidificatio: scriptum chassis scriptum alterum vocat -> parens,
#        contextus HEREDITATUS (commissum idem; contextus in ambitu
#        praepositus vincit git - hereditas, non recomputatio)
#   IV   contextus in repositorio temporario: commissum = HEAD, ramus,
#        arbor = radix, sordidum 0 mundum / 1 mutatum
#   V    involucrum necatum (SIGKILL) -> INTERRUPTUS, non absens
#   VI   CURSUS_NULLUS=1 -> scriptum sine involucro currit, nihil notatum
#   VII  scriptum se ipsum iterum exsequens (exemplar ianuarum) -> cursus
#        UNUS, non duo
#   VIII extra omne repositorium (contextus vacuus) -> cursus notatus,
#        commissum et ramus vacui, nulla ruina (planta D T2: setenv cum
#        NIHIL ruebat)
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="mensor-cursus" currit="bin/mensor" tegit="tools/cursus.sh tools/mensor.c lib/mensura.c include/mensura.h"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
if [ ! -x bin/mensor ]; then
    echo "fumus mensor_cursus: bin/mensor absens (./tools/mensor_struere.sh) - nihil actum"
    exit 2
fi
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
export MENSOR_DIARIUM="$T/diarium.tsv" MENSOR_VOLUMEN="$T/v.volumen" MENSOR_ACTA="$T/acta"
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
grep -qx "ad effusionem" "$T/o2" && grep -qx "ad erratum" "$T/e2" \
  && [ -n "$acta" ] && [ -f "$MENSOR_ACTA/$acta" ] \
  && grep -q "ad effusionem" "$MENSOR_ACTA/$acta" && grep -q "ad erratum" "$MENSOR_ACTA/$acta"
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

echo
if [ "$fracta" -eq 0 ]; then echo "fumus mensor_cursus: sanum (VIII/VIII)"; exit 0; fi
echo "fumus mensor_cursus: FRACTUM ($fracta)"; exit 1
