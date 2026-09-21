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
post=$(wc -l < "$AN" | tr -d ' ')
[ "$ante" = "$post" ]; credo $? "annales vivi INTACTI ($ante -> $post lineae)"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus frigida: sanum"; exit 0; fi
echo "fumus frigida: FRACTUM ($fracta)"; exit 1
