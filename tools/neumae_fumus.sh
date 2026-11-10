#!/bin/bash
# tools/neumae_fumus.sh - porta natalis instrumenti neumae (gabc pars
# II, P4): cantus gabc in PNG sine fenestra
#
# bin/neumae EX FONTIBUS HODIERNIS: fumus ipse prius struit - binarium
# stalum portam mutam faceret (ut auditio_fumus.sh).
#   I.   Salve: exitus 0, PNG (signum \x89PNG), latitudo CMLX, linea
#        stdout "pagina 1/1"
#   II.  -scala 2: latitudo MCMXX
#   III. usus: sine -imago, -spatium 7, -pagina 2 (extra): exitus 2
#   IV.  cantus cum vitio: exitus 1, PNG tamen scriptum, vitium in
#        stderr
#   V.   corpus: clavis F (01568) et quilismata (03561): exitus 0
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="neumae-fumus" instrumentum="bin/neumae" tegit="tools/neumae_fumus.sh tools/neumae_struere.sh musica/fontes/salve_2435.gabc" currit="tools/neumae.c lib/gabc_pictura.c lib/gabc_compositio.c lib/neumae_ductae.c lib/gabc.c"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
for o in tools__neumae lib__gabc_pictura lib__gabc_compositio lib__neumae_ductae lib__gabc; do
    rm -f "build/aedilis/obiecta/$o.o"
done
if ! ./tools/neumae_struere.sh > /dev/null 2>&1 || [ ! -x bin/neumae ]; then
    echo "  FRACTUM ./tools/neumae_struere.sh"
    echo "fumus neumae: 1 FRACTA"
    exit 1
fi
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
fracta=0
credo () {   # <condicio 0/1> <titulus>
    if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi
}
latitudo () {   # latitudo PNG ex IHDR (octeti XVI-XIX)
    od -An -tu1 -j16 -N4 "$1" | awk 'NF{print $1*16777216 + $2*65536 + $3*256 + $4}'
}
SALVE=musica/fontes/salve_2435.gabc

# I. Salve
bin/neumae "$SALVE" -imago "$T/s.png" > "$T/s.out" 2> "$T/s.err"; rc=$?
[ "$rc" -eq 0 ]; credo $? "Salve: exitus 0 (est $rc)"
[ "$(od -An -c -N4 "$T/s.png" 2>/dev/null | tr -d ' ')" = "211PNG" ]; credo $? "Salve: PNG scriptum"
[ "$(latitudo "$T/s.png")" = "960" ]; credo $? "Salve: latitudo 960"
grep -q 'pagina 1/1' "$T/s.out"; credo $? "Salve: stdout pagina 1/1"

# II. scala
bin/neumae "$SALVE" -imago "$T/s2.png" -scala 2 > /dev/null 2>&1; rc=$?
[ "$rc" -eq 0 ] && [ "$(latitudo "$T/s2.png")" = "1920" ]; credo $? "-scala 2: latitudo 1920 (exitus $rc)"

# III. usus
bin/neumae "$SALVE" > /dev/null 2>&1; rc=$?
[ "$rc" -eq 2 ]; credo $? "sine -imago: exitus 2 (est $rc)"
bin/neumae "$SALVE" -imago "$T/x.png" -spatium 7 > /dev/null 2>&1; rc=$?
[ "$rc" -eq 2 ]; credo $? "-spatium 7: exitus 2 (est $rc)"
bin/neumae "$SALVE" -imago "$T/x.png" -pagina 2 > /dev/null 2>&1; rc=$?
[ "$rc" -eq 2 ]; credo $? "-pagina 2 (extra): exitus 2 (est $rc)"

# IV. vitium
printf '%%%%\n(c4) a(g\n' > "$T/fractus.gabc"
bin/neumae "$T/fractus.gabc" -imago "$T/f.png" > /dev/null 2> "$T/f.err"; rc=$?
[ "$rc" -eq 1 ] && [ -s "$T/f.png" ] && grep -q 'vitium' "$T/f.err"
credo $? "cantus cum vitio: exitus 1, PNG, vitium in stderr (est $rc)"

# V. corpus
for f in 01568 03561; do
    bin/neumae "probationes/fixa/gabc/gregobase/$f.gabc" -imago "$T/$f.png" > /dev/null 2>&1; rc=$?
    [ "$rc" -eq 0 ]; credo $? "corpus $f: exitus 0 (est $rc)"
done

if [ "$fracta" -gt 0 ]; then echo "fumus neumae: $fracta FRACTA"; exit 1; fi
echo "fumus neumae: omnia sana"
exit 0
