#!/bin/bash
# tools/mensor_suitae_fumus.sh - porta natalis tools/mensor_suitae.sh
#
# Volumen et diarium TEMPORARIA (MENSOR_VOLUMEN), nihil verum tangitur.
# Probat: arbor praefixata condita (totum/compilatio/cursus/praevolatus,
# probationes singulae sub cursu/compilatione); MENSOR_TACET tacet;
# sessio sine finire nihil condit. Exitus 0 sanum | 1 FRACTUM | 2
# nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
if [ ! -x bin/mensor ]; then
    echo "fumus mensor_suitae: bin/mensor absens (./tools/mensor_struere.sh) - nihil actum"
    exit 2
fi
if ! command -v sqlite3 >/dev/null 2>&1; then
    echo "fumus mensor_suitae: sqlite3 absens - nihil actum"; exit 2
fi
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
export MENSOR_VOLUMEN="$T/v.volumen"
unset MENSOR_TACET
fracta=0
credo () {   # <condicio 0/1> <titulus>
    if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi
}
quot () {    # <fragmentum tituli> -> numerus ordinum mensurae
    sqlite3 "$MENSOR_VOLUMEN" \
        "select count(*) from acta where genus='mensura' and instr(datum, '$1') > 0" 2>/dev/null || echo 0
}

source tools/mensor_suitae.sh

# I. sessio plena praefixata
mensor_suitae_incipere "fumus."
[ -n "$MSU_MENSOR" ]; credo $? "sessio incepta (mensor vivus)"
t0=$(mensor_suitae_nunc); sleep 0.05; mensor_suitae_compilatio probatio_a "$t0"
t0=$(mensor_suitae_nunc); sleep 0.10; mensor_suitae_cursus     probatio_a "$t0"
[ "$(echo "$MSU_ULTIMA >= 0.09" | bc)" = "1" ]; credo $? "MSU_ULTIMA cursus probatio_a >= 0.09 ($MSU_ULTIMA)"
t0=$(mensor_suitae_nunc); mensor_suitae_cursus probatio_b "$t0"
effusum=$(mensor_suitae_tardissimae 1)
echo "$effusum" | grep -q 'probatio_a'; credo $? "tardissimae: probatio_a prima"
mensor_suitae_finire "" 2 0 1
[ "$(quot 'fumus.suita.tempus.totum')" = "1" ];        credo $? "totum conditum praefixo"
[ "$(quot 'fumus.suita.tempus.compilatio')" = "1" ];   credo $? "compilatio condita"
[ "$(quot 'fumus.suita.tempus.cursus')" = "1" ];       credo $? "cursus conditus"
[ "$(quot 'fumus.suita.tempus.praevolatus')" = "1" ];  credo $? "praevolatus conditus"
[ "$(quot 'fumus.suita.probationes.totae')" = "1" ];   credo $? "probationes.totae conditae"
[ "$(quot 'fumus.probatio.cursus.probatio_a')" = "1" ]; credo $? "probatio.cursus.probatio_a condita"
[ "$(quot 'fumus.probatio.cursus.probatio_b')" = "1" ]; credo $? "probatio.cursus.probatio_b condita"
[ "$(quot 'fumus.probatio.compilatio.probatio_a')" = "1" ]; credo $? "probatio.compilatio.probatio_a condita"
# arbor: probatio_a sub nodo cursus (parens = clavis nodi cursus)
parens_a=$(sqlite3 "$MENSOR_VOLUMEN" "select datum from acta where genus='mensura' and instr(datum,'fumus.probatio.cursus.probatio_a')>0" | cut -f3)
id_cursus=$(sqlite3 "$MENSOR_VOLUMEN" "select datum from acta where genus='mensura' and instr(datum,'fumus.suita.tempus.cursus')>0" | cut -f2)
[ -n "$parens_a" ] && [ "$parens_a" = "$id_cursus" ]; credo $? "arbor: probatio_a filia nodi cursus"
# II. MENSOR_TACET: nihil conditum
ante=$(quot 'suita.tempus.totum')
MENSOR_TACET=1 mensor_suitae_incipere "tacet."
t0=$(mensor_suitae_nunc); mensor_suitae_cursus probatio_x "$t0"
MENSOR_TACET=1 mensor_suitae_finire "" 1 0 0
[ "$(quot 'tacet.suita.tempus.totum')" = "0" ]; credo $? "MENSOR_TACET: nihil conditum"
# III. sessio sine finire: nihil conditum (diarium solum, non volumen)
mensor_suitae_incipere "orba."
t0=$(mensor_suitae_nunc); mensor_suitae_cursus probatio_y "$t0"
[ "$(quot 'orba.')" = "0" ]; credo $? "sine finire: nihil in volumine"
rm -f "$MSU_CURSUS" "$MSU_COMPILATIO"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus mensor_suitae: sanum"; exit 0; fi
echo "fumus mensor_suitae: FRACTUM ($fracta)"; exit 1
