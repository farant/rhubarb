#!/bin/bash
# tools/excubitor_fumus.sh - porta natalis excubitor.sh (custos
# staleness obiectorum), arbore ficta in directorio temporario
#
# I.    filtrum 'sub/build/' (via angusta, budget B2 H2): caput
#       recentius obiecto -> exitus 1, STALUM sub/build/a.o nominatum
# II.   obiectum recentius -> -tacitus tacet, exitus 0
# III.  sine filtro (via plena): idem STALUM
# IV.   renovatio graphi: fons recentior SUB build/ ignoratur (prune),
#       extra build/ silva/nexus.sh -renovare vocat
#
# CUR: nulla porta verdicta excubitoris asserebat (diagnostica solum
# signaturam quaerit) - planta 'via angusta *.c pro *.o' (semper
# PURUS) omnes portas praeteribat.
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="excubitor" tegit="excubitor.sh tools/excubitor_fumus.sh"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
cp excubitor.sh "$T/" || exit 2
mkdir -p "$T/build" "$T/src" "$T/sub/build" "$T/silva" || exit 2
printf 'src/a.c\tsrc/h.h\n' > "$T/build/inclusiones.tsv"
: > "$T/fabrica.tsv"
printf '#include "h.h"\n' > "$T/src/a.c"
printf '/* h */\n' > "$T/src/h.h"
: > "$T/sub/build/a.o"
printf '#!/bin/sh\necho renovatum >> "%s/renovationes"\n' "$T" > "$T/silva/nexus.sh"
chmod +x "$T/silva/nexus.sh"
# graphus recentissimus omnium fontium (renovatio non vocatur nisi IV)
touch -t 202601010000 "$T/src/a.c" "$T/sub/build/a.o"
touch -t 202601020000 "$T/src/h.h"
touch -t 202601030000 "$T/build/inclusiones.tsv"

echo "--- I. filtrum sub/build/: caput recentius ---"
out=$(cd "$T" && ./excubitor.sh -tacitus -sine-renovatione sub/build/ 2>&1); rc=$?
[ "$rc" -eq 1 ]; credo $? "exitus 1 (rc $rc)"
echo "$out" | grep -q "STALUM sub/build/a.o"; credo $? "STALUM sub/build/a.o nominatum"
echo "$out" | grep -q "POST src/h.h"; credo $? "causa: POST src/h.h"

echo "--- II. obiectum recentius: tacet ---"
touch -t 202601040000 "$T/sub/build/a.o"
out=$(cd "$T" && ./excubitor.sh -tacitus -sine-renovatione sub/build/ 2>&1); rc=$?
[ "$rc" -eq 0 ] && [ -z "$out" ]; credo $? "exitus 0, silentium (rc $rc)"
touch -t 202601010000 "$T/sub/build/a.o"

echo "--- III. sine filtro: via plena idem ---"
out=$(cd "$T" && ./excubitor.sh -sine-renovatione 2>&1); rc=$?
[ "$rc" -eq 1 ] && echo "$out" | grep -q "STALUM sub/build/a.o"; credo $? "STALUM sine filtro (rc $rc)"

echo "--- IV. renovatio graphi: build/ praetermissum ---"
printf '/* novus */\n' > "$T/sub/build/novus.c"
touch -t 202601050000 "$T/sub/build/novus.c"
(cd "$T" && ./excubitor.sh -tacitus sub/build/ > /dev/null 2>&1)
[ ! -f "$T/renovationes" ]; credo $? "fons recentior sub build/: renovatio NON vocata"
touch -t 202601050000 "$T/src/a.c"
(cd "$T" && ./excubitor.sh -tacitus sub/build/ > /dev/null 2>&1)
[ -f "$T/renovationes" ]; credo $? "fons recentior extra build/: nexus.sh -renovare vocatum"

if [ "$fracta" -ne 0 ]; then
    echo "fumus excubitoris: FRACTUM ($fracta)"
    exit 1
fi
echo "fumus excubitoris: sanum"
exit 0
