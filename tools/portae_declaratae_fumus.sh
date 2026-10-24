#!/bin/bash
# tools/portae_declaratae_fumus.sh - porta 'portae-declaratae' (plan
# portae-declaratae T5): opertura portarum ex REPOSITORIO solo.
#
#   I   'bin/fabrica portae declaratae' super registrum PORTAE verum:
#       omnis clavis cursorem annotatum UNUM habet, annotationes omnes
#       legibiles, possessio instrumentorum una - recusatio nominata
#   II  clone frigidus: annales absentes (RHUBARB_ANNALES nusquam)
#       portas debitas non mutant - tools/portae_debitae.sh cursorem
#       radicis ut 'radix' nominat, exitus 0
#   III 'bin/fabrica portae relatio' (unitates-codicis U5 a) in arbore
#       git TEMPORARIA: bibliotheca a (caput + fons), porta g cum
#       'tegit lib/*' solo, instrumentum tools/orphanus.c - responsa
#       exacta: bibliotheca codex II tecti I, INTECTA include/a.h et
#       tools/orphanus.c, RATIO g textus I (lib/a.c per tegit solum)
#   IV  'bin/fabrica portae translatio' (translatio receptorum T2) in
#       arbore git TEMPORARIA: g tegit lib/*, h legit docs/*, n
#       translatio="numquam". Sine receptis: g h nullum, n numquam.
#       Plagula NOVA docs/x.md (non tracta): ARBOR mutatur, index verus
#       intactus, g portat, h lecta; lib/a.c mutatum: g debita; arbor
#       recepti ignota: debita; arbor eadem, digestum alienum:
#       instrumenta
#
# Debetur omni scripto crustae (annotatio quaelibet mutari potest),
# registro (pythonica/silva.py) et nucleo C (clausura tools/fabrica.c:
# lib/portae.c, crusta, effectus) - III s, vilis.
#
# Exitus: 0 sanum | 1 FRACTUM | 2 bin/fabrica deest.
# <porta titulus="portae-declaratae" genus="praeiudicium" tegit="*.sh pythonica/silva.py" currit="tools/fabrica.c" legit="*.c *.sh *.py tools/unci-git/*"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
[ -x bin/fabrica ] || { echo "fumus portae declaratae: bin/fabrica deest (./tools/fabrica_struere.sh)"; exit 2; }
T="$(mktemp -d "${TMPDIR:-/tmp}/portae_declaratae.XXXXXX")"
trap 'rm -rf "$T"' EXIT
fracta=0

python3 -c 'import sys; sys.path.insert(0, "pythonica"); import silva; sys.stdout.write(silva.portae_tabula())' > "$T/portae.tsv" || exit 2
bin/fabrica portae declaratae -machina -portae "$T/portae.tsv" > "$T/o1" 2>&1; rc1=$?
n=$(grep -c '^PORTA	' "$T/o1")
if [ "$rc1" -eq 0 ] && ! grep -q '^RECUSATIO' "$T/o1" && [ "$n" -gt 0 ]; then
    echo "  I   declarationes: $n portae, nulla recusatio OK"
else
    echo "  I   FRACTUM (rc=$rc1)"; grep '^RECUSATIO' "$T/o1" | sed 's/^/      /' | head -20; fracta=1
fi

RHUBARB_ANNALES="$T/annales_nusquam" ./tools/portae_debitae.sh compile_tests.sh > "$T/o2" 2>&1; rc2=$?
if [ "$rc2" -eq 0 ] && grep -q '^  radix$' "$T/o2"; then
    echo "  II  clone frigidus (annales absentes): radix debetur OK"
else
    echo "  II  FRACTUM (rc=$rc2)"; sed 's/^/      /' "$T/o2" | head -10; fracta=1
fi

M="$T/mundus3"
mkdir -p "$M/include" "$M/lib" "$M/tools" "$M/crusta"
cp crusta/effectus_mandata.stml "$M/crusta/"
grep -v '<familia' unitates.stml > "$M/unitates.stml"
: > "$M/fabrica.stml"; : > "$M/aedilis.stml"
printf '<unitas titulus="m" genus="/elementa_programmatis/subsystema" descriptio="m"/>\n' > "$M/unitas.stml"
echo '/* a */' > "$M/include/a.h"
echo '/* a */' > "$M/lib/a.c"
echo '/* o */' > "$M/tools/orphanus.c"
printf '#!/bin/sh\n# <porta titulus="g" tegit="lib/*"/>\nexit 0\n' > "$M/tools/g_fumus.sh"
printf 'g\ttools/g_fumus.sh\n' > "$T/portae3.tsv"
(cd "$M" && git init -q && git add -A && "$RADIX/bin/fabrica" portae relatio -machina -portae "$T/portae3.tsv" -tempora /dev/null) > "$T/o3" 2>&1; rc3=$?
g3=$(awk -F'\t' '$1 == "GENUS" && $2 == "bibliotheca" { print $3 "/" $4 }' "$T/o3")
i3=$(awk -F'\t' '$1 == "INTECTUM" { print $5 }' "$T/o3" | sort | tr '\n' ' ')
r3=$(awk -F'\t' '$1 == "RATIO" && $2 == "g" && $3 == "textus" { print $4 }' "$T/o3")
if [ "$rc3" -eq 0 ] && [ "$g3" = "2/1" ] && [ "$i3" = "include/a.h tools/orphanus.c " ] && [ "$r3" = "1" ]; then
    echo "  III relatio in arbore temporaria: bibliotheca II/I tecti, intecta include/a.h tools/orphanus.c, textus solus I OK"
else
    echo "  III FRACTUM (rc=$rc3, bibliotheca '$g3' pro 2/1, intecta '$i3', textus '$r3' pro 1)"; sed 's/^/      /' "$T/o3" | head -12; fracta=1
fi

M4="$T/mundus4"
mkdir -p "$M4/lib" "$M4/tools" "$M4/crusta"
cp crusta/effectus_mandata.stml "$M4/crusta/"
: > "$M4/fabrica.stml"; : > "$M4/aedilis.stml"
echo '/* a */' > "$M4/lib/a.c"
printf '#!/bin/sh\n# <porta titulus="g" tegit="lib/*"/>\nexit 0\n' > "$M4/tools/g_fumus.sh"
printf '#!/bin/sh\n# <porta titulus="h" legit="docs/*"/>\nexit 0\n' > "$M4/tools/h_fumus.sh"
printf '#!/bin/sh\n# <porta titulus="n" translatio="numquam"/>\nexit 0\n' > "$M4/tools/n_fumus.sh"
printf 'g\ttools/g_fumus.sh\nh\ttools/h_fumus.sh\nn\ttools/n_fumus.sh\n' > "$T/portae4.tsv"
tr4 () { (cd "$M4" && "$RADIX/bin/fabrica" portae translatio -machina -portae "$T/portae4.tsv" "$@"); }
genus4 () { awk -F'\t' -v p="$1" '$1 == "TRANSLATIO" && $2 == p { print $3 }'; }
(cd "$M4" && git init -q && git add -A) || exit 2
tr4 g h n > "$T/o4a" 2>&1; rc4a=$?
a0=$(awk -F'\t' '$1 == "ARBOR" { print $2 }' "$T/o4a")
dg=$(awk -F'\t' '$1 == "INSTRUMENTA" && $2 == "g" { print $3 }' "$T/o4a")
dh=$(awk -F'\t' '$1 == "INSTRUMENTA" && $2 == "h" { print $3 }' "$T/o4a")
v4a="$(genus4 g < "$T/o4a")/$(genus4 h < "$T/o4a")/$(genus4 n < "$T/o4a")"
mkdir -p "$M4/docs"; echo 'x' > "$M4/docs/x.md"
tr4 -receptum g "$a0" "$dg" -receptum h "$a0" "$dh" g h n > "$T/o4b" 2>&1; rc4b=$?
a1=$(awk -F'\t' '$1 == "ARBOR" { print $2 }' "$T/o4b")
v4b="$(genus4 g < "$T/o4b")/$(genus4 h < "$T/o4b")/$(genus4 n < "$T/o4b")"
c4b=$(awk -F'\t' '$1 == "TRANSLATIO" && $2 == "g" { print $4 }' "$T/o4b")
st4=$(cd "$M4" && git status --porcelain docs)
echo '/* a2 */' > "$M4/lib/a.c"
v4c=$(tr4 -receptum g "$a0" "$dg" g 2>&1 | genus4 g)
v4d=$(tr4 -receptum g 0000000000000000000000000000000000000000 "$dg" g 2>&1 | genus4 g)
a2=$(tr4 g 2>&1 | awk -F'\t' '$1 == "ARBOR" { print $2 }')
v4e=$(tr4 -receptum g "$a2" alienum g 2>&1 | genus4 g)
if [ "$rc4a" -eq 0 ] && [ "$rc4b" -eq 0 ] && [ "$v4a" = "nullum/nullum/numquam" ] \
    && [ -n "$a0" ] && [ "$a0" != "$a1" ] && [ "$st4" = "?? docs/" ] \
    && [ "$v4b" = "portat/lecta/numquam" ] \
    && [ "$c4b" = "1 via mutata, nulla huius portae" ] && [ "$v4c" = "debita" ] \
    && [ "$v4d" = "debita" ] && [ "$v4e" = "instrumenta" ]; then
    echo "  IV  translatio: nullum/numquam, plagula nova -> portat/lecta (index verus intactus), debita, arbor ignota, instrumenta OK"
else
    echo "  IV  FRACTUM (rc=$rc4a/$rc4b, sine receptis '$v4a', cum '$v4b', status '$st4', causa g '$c4b', mutata '$v4c', ignota '$v4d', digestum '$v4e')"; sed 's/^/      /' "$T/o4b" | head -10; fracta=1
fi

if [ "$fracta" -ne 0 ]; then echo "fumus portae declaratae: FRACTUM"; exit 1; fi
echo "fumus portae declaratae: sanum"
exit 0
