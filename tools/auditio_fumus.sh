#!/bin/bash
# tools/auditio_fumus.sh - porta natalis fontium conversorum auditionis
# (-gabc, gabc plan T8; -musicxml, musicxml M12)
#
# bin/auditio EX FONTIBUS HODIERNIS: fumus ipse prius struit (~7 s) -
# binarium stalum portam mutam faceret (planta in fonte, porta viridis:
# visum 2026-11-04).
# Sonum NON probat (-sonare per cantorem ad apparatum it - nulla via
# tacendi, auditio.worklog.md 2026-11-02); probat liber conversus,
# fons libri (-partitura), vexilla et exitus:
#   I.   -gabc salve: liber in stdout, carmina_legere eum accipit
#        (-liber ... -carmina), clavis f ionius, exitus 0
#   II.  -gabc ... -partitura: fons libri, titulus ex name
#   III. -gabc et -musicxml simul; -do sine -gabc; -do malum: exitus 2
#   IV.  -do B3 / -do Bb3: do fixum (nota prima o2 b / o2 b-)
#   V.   approximatio: exitus 3; -strictus: exitus 1, stdout vacuum
#   VI.  -titulus; cantus sine name -> nomen plagulae; vitium -> 1
#   VII. -musicxml nata_lux: liber legitur (via communis conversionis)
#   VIII. -psalmus 116 -signum "8 G" (psalmodia plan-2 T7): gabc in
#        stdout quod -gabc accipit; Gloria, -sine-gloria, -titulus,
#        -antiqua; -signum malum et admonitiones: exitus 1 cum
#        'via:linea:columna: causa'; usus: exitus 2; -partitura
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="auditio-fumus" instrumentum="bin/auditio" tegit="tools/auditio_fumus.sh tools/auditio_struere.sh musica/fontes/salve_2435.gabc musica/psalmi/116.txt" currit="tools/auditio.c lib/gabc.c lib/gabc_carmen.c lib/musicxml.c lib/musicxml_carmen.c lib/psalmodia.c lib/psalmodia_tabula.c lib/syllabae_latinae.c lib/toni_psalmorum_infixum.c"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
# obiecta fontium tectorum (currit) tolluntur: struere.sh aedilis per
# '-nt' iudicat, et bash 3.2 secundas solas comparat - fons restitutus
# eadem secunda ac obiectum plantatum (fumus < I s) obiectum servaret
for o in tools__auditio lib__gabc lib__gabc_carmen lib__musicxml lib__musicxml_carmen \
         lib__psalmodia lib__psalmodia_tabula lib__syllabae_latinae lib__toni_psalmorum_infixum; do
    rm -f "build/aedilis/obiecta/$o.o"
done
if ! ./tools/auditio_struere.sh > /dev/null 2>&1 || [ ! -x bin/auditio ]; then
    echo "  FRACTUM ./tools/auditio_struere.sh"
    echo "fumus auditio: 1 FRACTA"
    exit 1
fi
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
fracta=0
credo () {   # <condicio 0/1> <titulus>
    if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi
}
SALVE=musica/fontes/salve_2435.gabc

# I. liber conversus
bin/auditio -gabc "$SALVE" > "$T/s.stml" 2> "$T/s.err"; rc=$?
[ "$rc" -eq 0 ]; credo $? "-gabc salve: exitus 0 (est $rc)"
head -1 "$T/s.stml" | grep -qx '<carmina>'; credo $? "-gabc salve: stdout liber <carmina>"
grep -q 'clavis="f ionius"' "$T/s.stml"; credo $? "-gabc salve: clavis f ionius"
bin/auditio -liber "$T/s.stml" -carmina > "$T/c.out" 2>&1; rc=$?
[ "$rc" -eq 0 ] && grep -q 'salve_regina_simple_tone' "$T/c.out"; credo $? "liber conversus legitur (-liber -carmina, exitus $rc)"
grep -q 'nota structurae: do = F4' "$T/s.err"; credo $? "relatio in stderr (do = F4)"

# II. fons libri
bin/auditio -gabc "$SALVE" -partitura > "$T/p.out" 2>/dev/null; rc=$?
[ "$rc" -eq 0 ] && [ "$(head -1 "$T/p.out")" = "carmen salve_regina_simple_tone: f ionius, liberum, 80" ]
credo $? "-gabc ... -partitura: linea prima titulus (exitus $rc)"

# III. usus
bin/auditio -gabc "$SALVE" -musicxml x.mxl > /dev/null 2>&1; rc=$?
[ "$rc" -eq 2 ]; credo $? "-gabc et -musicxml simul: exitus 2 (est $rc)"
bin/auditio -do F4 -carmen salve > /dev/null 2>&1; rc=$?
[ "$rc" -eq 2 ]; credo $? "-do sine -gabc: exitus 2 (est $rc)"
bin/auditio -gabc "$SALVE" -do H9 > /dev/null 2>&1; rc=$?
[ "$rc" -eq 2 ]; credo $? "-do H9: exitus 2 (est $rc)"

# IV. do fixum: nota prima Salve est do octava infra
bin/auditio -gabc "$SALVE" -do B3 > "$T/b.stml" 2> "$T/b.err"
bin/auditio -gabc "$SALVE" -do Bb3 > "$T/bb.stml" 2> "$T/bb.err"
grep -q '<textus!>o2 b8 ' "$T/b.stml" && grep -q '<textus!>o2 b-8 ' "$T/bb.stml"
credo $? "-do B3 / Bb3: nota prima o2 b8 / o2 b-8"
grep -q 'do = Bb3 (fixum)' "$T/bb.err"; credo $? "-do Bb3: relatio 'do = Bb3 (fixum)'"

# V. approximatio et strictus
printf '%%%%\n(c4) a(ji)\n' > "$T/si.gabc"
bin/auditio -gabc "$T/si.gabc" > /dev/null 2>&1; rc=$?
[ "$rc" -eq 3 ]; credo $? "finalis si: approximatio, exitus 3 (est $rc)"
bin/auditio -gabc "$T/si.gabc" -strictus > "$T/st.out" 2> "$T/st.err"; rc=$?
[ "$rc" -eq 1 ] && [ ! -s "$T/st.out" ] && grep -q 'recusata (strictus)' "$T/st.err"
credo $? "-strictus: exitus 1, stdout vacuum (est $rc)"

# VI. titulus et vitium
bin/auditio -gabc "$SALVE" -titulus salve 2>/dev/null | grep -q '<carmen titulus="salve"'
credo $? "-titulus salve"
printf '%%%%\n(c4) a(g)\n' > "$T/cantus_brevis.gabc"
bin/auditio -gabc "$T/cantus_brevis.gabc" 2>/dev/null | grep -q '<carmen titulus="cantus_brevis"'
credo $? "sine name: titulus = nomen plagulae"
printf '%%%%\n(c4) a(g\n' > "$T/fractus.gabc"
bin/auditio -gabc "$T/fractus.gabc" > /dev/null 2> "$T/f.err"; rc=$?
[ "$rc" -eq 1 ] && grep -q 'cantus non legitur' "$T/f.err"; credo $? "cantus cum vitio: exitus 1 (est $rc)"

# VII. -musicxml per viam communem
bin/auditio -musicxml musica/fontes/nata_lux_80317.mxl > "$T/n.stml" 2>/dev/null; rc=$?
bin/auditio -liber "$T/n.stml" -carmina > /dev/null 2>&1; rc2=$?
[ "$rc" -eq 0 ] && [ "$rc2" -eq 0 ]; credo $? "-musicxml nata_lux: liber legitur (exitus $rc, $rc2)"

# VIII. -psalmus: textus punctatus + tabula infixa -> gabc
PS=musica/psalmi/116.txt
bin/auditio -psalmus "$PS" -signum "8 G" > "$T/ps.gabc" 2> "$T/ps.err"; rc=$?
[ "$rc" -eq 0 ] && [ ! -s "$T/ps.err" ]; credo $? "-psalmus 116 -signum \"8 G\": exitus 0, stderr vacuum (est $rc)"
[ "$(sed -n 1,3p "$T/ps.gabc")" = "$(printf 'mode: 8;\nannotation: 8 G;\n%%%%')" ]
credo $? "-psalmus: caput mode, annotation, %%"
grep -qF '(c4) 1. Lau(g)dá(h)te(j) Dó(j)mi(j)num,(j) o(j)mnes(j) <b>gen</b>(k)tes:(j) *(:)' "$T/ps.gabc"
credo $? "-psalmus: versus primus cum intonatione et mediatione"
grep -qF 'Gló(j)ri(j)a(j) Pa(j)tri,(j)' "$T/ps.gabc"; credo $? "-psalmus: Gloria Patri addita"
bin/auditio -gabc "$T/ps.gabc" > "$T/psg.stml" 2>/dev/null; rc=$?
[ "$rc" -eq 0 ] && head -1 "$T/psg.stml" | grep -qx '<carmina>'; credo $? "gabc psalmi per -gabc legitur (exitus $rc)"
bin/auditio -psalmus "$PS" -signum "8 G" -sine-gloria > "$T/psg.gabc" 2>/dev/null; rc=$?
[ "$rc" -eq 0 ] && [ "$(grep -c '(::)$' "$T/psg.gabc")" -eq 2 ] && ! grep -q 'Gló' "$T/psg.gabc"
credo $? "-sine-gloria: versus II, Gloria nulla (exitus $rc)"
bin/auditio -psalmus "$PS" -signum "8 G" -titulus "Ps. 116" 2>/dev/null | head -1 | grep -qx 'name: Ps. 116;'
credo $? "-titulus: name: Ps. 116;"
bin/auditio -psalmus "$PS" -signum 6 > "$T/p6.gabc" 2>/dev/null; rc=$?
bin/auditio -psalmus "$PS" -signum 6 -antiqua > "$T/p6a.gabc" 2>/dev/null; rc2=$?
[ "$rc" -eq 0 ] && [ "$rc2" -eq 0 ] && ! cmp -s "$T/p6.gabc" "$T/p6a.gabc"
credo $? "-antiqua: tonus 6 mediatione ad libitum (exitus $rc, $rc2)"
bin/auditio -psalmus "$PS" -signum "8 G" -antiqua > "$T/p8a.out" 2> "$T/p8a.err"; rc=$?
[ "$rc" -eq 1 ] && [ ! -s "$T/p8a.out" ] && grep -qF 'tonus 8 mediationem ad libitum non habet' "$T/p8a.err"
credo $? "-antiqua in tono 8: exitus 1, causa (est $rc)"
bin/auditio -psalmus "$PS" -signum 9 > "$T/p9.out" 2> "$T/p9.err"; rc=$?
[ "$rc" -eq 1 ] && [ ! -s "$T/p9.out" ] && grep -qF 'tonus "9" nullus' "$T/p9.err"
credo $? "-signum 9: exitus 1, tonus \"9\" nullus (est $rc)"
printf 'Laudáte Dóminum, * omnes gentes.\nLaudate eum * omnes pópuli.\n' > "$T/sine.txt"
bin/auditio -psalmus "$T/sine.txt" -signum "8 G" > "$T/sine.gabc" 2> "$T/sine.err"; rc=$?
[ "$rc" -eq 1 ] && grep -qF "$T/sine.txt:2:1: verbum \"Laudate\" sine accentu" "$T/sine.err" && grep -q '(::)' "$T/sine.gabc"
credo $? "admonitio: via:linea:columna in stderr, gabc tamen, exitus 1 (est $rc)"
bin/auditio -psalmus "$T/nullus.txt" -signum "8 G" > /dev/null 2>&1; rc=$?
[ "$rc" -eq 1 ]; credo $? "-psalmus plagula absens: exitus 1 (est $rc)"
bin/auditio -psalmus "$PS" > /dev/null 2>&1; rc=$?
bin/auditio -signum "8 G" -carmen salve > /dev/null 2>&1; rc2=$?
bin/auditio -gabc "$SALVE" -sine-gloria > /dev/null 2>&1; rc3=$?
bin/auditio -psalmus "$PS" -signum "8 G" -gabc "$SALVE" > /dev/null 2>&1; rc4=$?
[ "$rc" -eq 2 ] && [ "$rc2" -eq 2 ] && [ "$rc3" -eq 2 ] && [ "$rc4" -eq 2 ]
credo $? "usus: -psalmus sine -signum, -signum/-sine-gloria sine -psalmus, -psalmus cum -gabc: exitus 2 ($rc $rc2 $rc3 $rc4)"
bin/auditio -psalmus "$PS" -signum "8 G" -partitura > "$T/pp.out" 2>/dev/null; rc=$?
[ "$rc" -eq 0 ] && head -1 "$T/pp.out" | grep -q '^carmen 116: '; credo $? "-psalmus ... -partitura: carmen 116 (exitus $rc)"

if [ "$fracta" -gt 0 ]; then echo "fumus auditio: $fracta FRACTA"; exit 1; fi
echo "fumus auditio: omnia sana"
exit 0
