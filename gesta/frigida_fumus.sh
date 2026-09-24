#!/bin/bash
# gesta/frigida_fumus.sh - porta natalis LAUNCHERI gesta/frigida.sh
#
# CUR: probatio_frigida.c MACHINAM probat (frigida_currere), non
# launcherum. Et launcher est quod die 2026-09-21 FRACTUM inventum
# est: index obiectorum manu scriptus 'processus' nesciebat, nexus
# cadebat, et NEMO sciebat duos menses - porta nulla eum currebat.
# Haec porta launcherum VERUM aedificat et currit.
#
# NIHIL in tabulario vivo scribit: gradus I-II usum solum petunt;
# gradus III recusationem petit (synonymum -> nihil scriptum), et
# numerus linearum annalium ante et post ASSERITUR idem.
#
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
AN="gesta/annales/tabularium.jsonl"
ante=$(wc -l < "$AN" | tr -d ' ')

# I. launcher aedificat, NECTIT et currit: sine argumentis = usus, exitus 2
eff=$(./gesta/frigida.sh 2>&1); rc=$?
[ "$rc" -eq 2 ]; credo $? "sine argumentis: exitus 2 (erat $rc) - aedificatio et nexus sani"
echo "$eff" | grep -q -- '-nexus <res> <verbum> <alterum>'; credo $? "usus formas novas nominat"
echo "$eff" | grep -q 'ld: \|symbol(s) not found\|error:'; [ $? -ne 0 ]; credo $? "nullus error nexus aut compilationis in effusu"

# II. vexillum ignotum: exitus 2, formae validae ostensae
eff=$(./gesta/frigida.sh -statum x y 2>&1); rc=$?
[ "$rc" -eq 2 ]; credo $? "vexillum ignotum: exitus 2 (erat $rc)"

# III. IANUA UNA per launcherum verum: synonymum recusatur (exitus 1),
#      imperium validum ostenditur, NIHIL scribitur
eff=$(./gesta/frigida.sh -nexus vocabularium-tagorum pendet-ex vocabularium-tagorum 2>&1); rc=$?
# exitus 1 SOLUS non sufficit: nexus fractus QUOQUE 1 reddit (planta
# launcheri id ostendit - linea haec 'ok' manebat launchero fracto)
[ "$rc" -eq 1 ] && echo "$eff" | grep -q 'frigida RECUSATA'; credo $? "synonymum per launcherum: exitus 1 (erat $rc) ET recusatio machinae"
echo "$eff" | grep -q 'IMPERIUM VALIDUM: ./gesta/frigida.sh -nexus'; credo $? "imperium validum ostensum"
# IV. -MAPPA per launcherum verum: lectio sine operando, arbor
#      nominum ex tabulario VIVO (radices Frani), exitus 0, nihil scriptum
eff=$(./gesta/frigida.sh -mappa 2>&1); rc=$?
[ "$rc" -eq 0 ]; credo $? "-mappa: exitus 0 (erat $rc)"
echo "$eff" | grep -q 'MAPPA (nomina)'; credo $? "-mappa: caput formae nominum"
echo "$eff" | grep -q 'Tabularii project management'; credo $? "-mappa: radix viva nominata"
echo "$eff" | grep -q 'parca '; [ $? -ne 0 ]; credo $? "-mappa: numeri absunt (nomina sola)"

# V. -MAPPA-PLENA: visiones et principia vivae ut lineae captae STML
eff=$(./gesta/frigida.sh -mappa-plena 2>&1); rc=$?
[ "$rc" -eq 0 ]; credo $? "-mappa-plena: exitus 0 (erat $rc)"
echo "$eff" | grep -q '^<principium (> PRINCIPIUM'; credo $? "-mappa-plena: principium domus gradu 0"
echo "$eff" | grep -q '^  <visio (> VISIO tabularii'; credo $? "-mappa-plena: visio sub regione sua"

# VI. -INVENTARIUM: cellae inventarii VIVI in forma machinae (lector
#     silva.inventarium). Invariantes, non numeri fixi: area (>= XXX
#     lineae), forma (quattuor campi per lineam), nomen ordinis notum
eff=$(./gesta/frigida.sh -inventarium 'suitae probationum' 2>&1); rc=$?
[ "$rc" -eq 0 ]; credo $? "-inventarium: exitus 0 (erat $rc)"
n=$(printf '%s\n' "$eff" | grep -c .)
[ "$n" -ge 30 ]; credo $? "-inventarium: lineae >= 30 (erant $n)"
mali=$(printf '%s\n' "$eff" | awk -F'\t' 'NF && NF != 4' | wc -l | tr -d ' ')
[ "$mali" -eq 0 ]; credo $? "-inventarium: quattuor campi per lineam ($mali malae)"
printf '%s\n' "$eff" | grep -q "^compile_tests.sh	in PORTAE	ita-non	ita$"; credo $? "-inventarium: radix in PORTAE"

post=$(wc -l < "$AN" | tr -d ' ')
[ "$ante" = "$post" ]; credo $? "annales vivi INTACTI ($ante -> $post lineae)"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus frigida: sanum"; exit 0; fi
echo "fumus frigida: FRACTUM ($fracta)"; exit 1
