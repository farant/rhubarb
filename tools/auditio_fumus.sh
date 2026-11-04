#!/bin/bash
# tools/auditio_fumus.sh - porta natalis fontium conversorum auditionis
# (-gabc, gabc plan T8; -musicxml, musicxml M12)
#
# bin/auditio ut instrumentum (struere: ./tools/auditio_struere.sh).
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
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="auditio-fumus" instrumentum="bin/auditio" tegit="tools/auditio_fumus.sh tools/auditio_struere.sh musica/fontes/salve_2435.gabc" currit="tools/auditio.c lib/gabc.c lib/gabc_carmen.c lib/musicxml.c lib/musicxml_carmen.c"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
if [ ! -x bin/auditio ]; then
    echo "fumus auditio: bin/auditio absens (./tools/auditio_struere.sh) - nihil actum"
    exit 2
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

if [ "$fracta" -gt 0 ]; then echo "fumus auditio: $fracta FRACTA"; exit 1; fi
echo "fumus auditio: omnia sana"
exit 0
