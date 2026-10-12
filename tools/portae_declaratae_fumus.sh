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
#
# Debetur omni scripto crustae (annotatio quaelibet mutari potest),
# registro (pythonica/silva.py) et nucleo C (clausura tools/fabrica.c:
# lib/portae.c, crusta, effectus) - III s, vilis.
#
# Exitus: 0 sanum | 1 FRACTUM | 2 bin/fabrica deest.
# <porta titulus="portae-declaratae" tegit="*.sh pythonica/silva.py" currit="tools/fabrica.c"/>
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

if [ "$fracta" -ne 0 ]; then echo "fumus portae declaratae: FRACTUM"; exit 1; fi
echo "fumus portae declaratae: sanum"
exit 0
