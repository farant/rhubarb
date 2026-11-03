#!/bin/bash
# tools/nexus_fumus.sh - porta natalis percursus INCREMENTALIS nexus
# (silva/build/celer/nexus_percursus), arbore ficta in directorio
# temporario
#
# I.    plenus (tabula nulla): tabulae + build/nexus.sigilla.tsv nata
# II.   quietum: 'nulla mutatio'
# III.  caput TACTUM (contentus idem, budget B2 N1): 'nulla mutatio',
#       I tacta - olim clausura tota re-iudicata
# IV.   caput MUTATUM: ipsum + includens re-iudicata (I per clausuram)
# V.    paritas: tabulae incrementales octetim = plenus
#
# CUR: nulla porta modum incrementalem asserebat (paritas solum manu,
# worklog nexus 2026-07-14) - planta 'contentus semper idem' editionem
# veram celaret et tabulam stalam relinqueret.
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="nexus" tegit="silva/instrumenta/principalia/nexus_percursus.c tools/nexus_fumus.sh"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
# instrumentum recens (nexus.sh struit si stalum; tabula vera
# incrementaliter renovatur - effectus secundarius innocuus)
./silva/nexus.sh -renovare > /dev/null 2>&1 \
    || { echo "fumus nexus: nihil actum (nexus.sh -renovare fractum)"; exit 2; }
PERCURSUS="$RADIX/silva/build/celer/nexus_percursus"
[ -x "$PERCURSUS" ] || { echo "fumus nexus: nihil actum ($PERCURSUS deest)"; exit 2; }
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
mkdir -p "$T/include" "$T/lib" "$T/silva/fontes" "$T/build" || exit 2
# mtime servatum (cp -p): systema mtime >= stampa = plenus coactum
cp -p silva/fontes/systema_c89.h "$T/silva/fontes/" || exit 2
printf '#ifndef A_H\n#define A_H\nint a_functio(void);\n#endif\n' > "$T/include/a.h"
printf '#ifndef B_H\n#define B_H\nint b_functio(void);\n#endif\n' > "$T/include/b.h"
printf '#include "a.h"\nint a_functio(void) { return 1; }\n' > "$T/lib/x.c"
printf '#include "b.h"\nint b_functio(void) { return 2; }\n' > "$T/lib/y.c"
# fixturae in praeterito (mtime < stampa cursus primi): aequaevum
# suspectum alias in cursu II sigillaretur
touch -t 202001010000 "$T/include/a.h" "$T/include/b.h" "$T/lib/x.c" "$T/lib/y.c"
_curre () { ( cd "$T" && "$PERCURSUS" "$@" ) 2>&1; }

o=$(_curre)
[ -s "$T/build/nexus.tsv" ] && grep -q $'^lib/x.c\t' "$T/build/nexus.sigilla.tsv" 2>/dev/null
credo $? "I plenus: tabulae et sigilla nata"
o=$(_curre)
echo "$o" | grep -q 'nulla mutatio'
credo $? "II quietum: nulla mutatio"
touch "$T/include/a.h"
o=$(_curre)
echo "$o" | grep -q 'nulla mutatio - tabulae intactae (1 tactae'
credo $? "III caput tactum (contentus idem): nulla mutatio, I tacta ($(echo "$o" | grep incrementale | head -1))"
printf '#ifndef A_H\n#define A_H\nint a_functio(void);\nint a_altera(void);\n#endif\n' > "$T/include/a.h"
o=$(_curre)
echo "$o" | grep -q 'incrementale: 2 re-iudicatae (1 per clausuram capitum)'
credo $? "IV caput mutatum: ipsum + includens re-iudicata ($(echo "$o" | grep incrementale | head -1))"
grep -v '^# nexus.tsv GENERATUM' "$T/build/nexus.tsv" > "$T/inc_ord"
grep -v '^# inclusiones.tsv GENERATUM' "$T/build/inclusiones.tsv" > "$T/inc_marg"
o=$(_curre -plenus)
grep -v '^# nexus.tsv GENERATUM' "$T/build/nexus.tsv" | cmp -s - "$T/inc_ord" \
    && grep -v '^# inclusiones.tsv GENERATUM' "$T/build/inclusiones.tsv" | cmp -s - "$T/inc_marg"
credo $? "V paritas: incrementale = plenus (ordines et margines)"

if [ "$fracta" -eq 0 ]; then
    echo "fumus nexus: sanum"
    exit 0
fi
echo "fumus nexus: FRACTUM ($fracta)"
exit 1
