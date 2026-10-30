#!/bin/bash
# tools/instrumenta_nexus_fumus.sh - porta NEXUS INSTRUMENTORUM: involucra
# instrumentorum (silva/<x>.sh -> silva/build/<x>) binarium suum solum
# nectunt cum STALUM est, et scriptura ATOMICA (temporarium + mv) - ut
# vocationes concurrentes inter se non frangant (ledger …WB6Q3H: census
# convertoris VI simul fumum credo X locis fregit; selecta.sh in OMNI
# vocatione nectebat).
#
# Per involucrum in INVOLUCRA:
#   I    vocatio secunda (recens) binarium NON nectit (inodus + tempus idem)
#   II   fons tactus -> nectit (binarium novum)
#   III  VIII vocationes concurrentes RECENTES: exitus et effusio idem
#   IV   VIII vocationes concurrentes STALAE (fons tactus): idem, binarium
#        integrum post (nulla reliquia temporaria)
#   V    ingressus secundarii: caput instrumentorum et fons additus
#        (silva_lexicon.h/.c) examen renectunt
#   VI   caput in directoriis vocantis (tools/caput_recens.sh) tactum:
#        obiecta recompilantur et examen renectitur
#
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="nexus-instrumentorum" gradus="fusio" tegit="officina/cursor.sh officina/fusor.sh officina/interpretare.sh silva/ambigua.sh silva/arbor.sh silva/censor.sh silva/computus.sh silva/differre.sh silva/emendare.sh silva/emitte.sh silva/formator.sh silva/impressor.sh silva/infidelis.sh silva/origo.sh silva/percursus.sh silva/quaestio.sh silva/renominare.sh silva/selecta.sh silva/transformare.sh" currit="silva/instrumenta/principalia/selecta.c" legit="include/* silva/fontes/* silva/instrumenta/*"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
T="$(mktemp -d "${TMPDIR:-/tmp}/nexus_fumus.XXXXXX")" || exit 2
trap 'rm -rf "${T:?}"' EXIT
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }

# fixurae parvae (@T): plagula una, altera, radix minima
printf 'static int f (int a)\n{\n    return a + 1;\n}\n\nint g (void)\n{\n    return f(1);\n}\n' > "$T/parva.c"
sed 's/a + 1/a + 2/' "$T/parva.c" > "$T/parva2.c"
mkdir -p "$T/radix"; cp "$T/parva.c" "$T/radix/"

# involucrum | binarium | fons | argumenta (@T = fixurae) | modus
# modus 'solum' = INSTRUMENTUM_NECTERE_SOLUM (instrumentum grave: nexus
# solus probatur, instrumentum non curritur); 'rc' = effusio tempora fert
# (computus, percursus, fusor) - exitus solus comparatur, binarium aeque
# stricte
S="silva/instrumenta/principalia"; O="officina/instrumenta/principalia"
INVOLUCRA=(
    "silva/selecta.sh|silva/build/selecta|$S/selecta.c|@T/parva.c definitio-functionis -omnia|"
    "silva/ambigua.sh|silva/build/ambigua|$S/ambigua.c|@T/parva.c|"
    "silva/arbor.sh|silva/build/arbor|$S/arbor.c|@T/parva.c -tacitus|"
    "silva/censor.sh|silva/build/censor|$S/censor.c|@T/parva.c|"
    "silva/computus.sh|silva/build/computus|$S/computus.c|@T/parva.c -nudum|rc"
    "silva/differre.sh|silva/build/differre|$S/differre.c|@T/parva.c @T/parva2.c|"
    "silva/emendare.sh|silva/build/emendare|$S/emendare.c|85 @T/parva.c|"
    "silva/emitte.sh|silva/build/emitte|$S/emitte.c|@T/parva.c f|"
    "silva/examen.sh|silva/build/examen|$S/examen.c|@T/parva.c|"
    "silva/formator.sh|silva/build/formator|$S/formator.c|@T/parva.c|"
    "silva/impressor.sh|silva/build/impressor|$S/impressor.c|2+3*4;|"
    "silva/infidelis.sh|silva/build/infidelis|$S/infidelis.c|@T/parva.c|"
    "silva/origo.sh|silva/build/origo|$S/origo.c|@T/parva.c 3|"
    "silva/percursus.sh|silva/build/percursus|$S/percursus.c|@T/radix|rc"
    "silva/quaestio.sh|silva/build/quaestio|$S/quaestio.c||"
    "silva/renominare.sh|silva/build/renominare|$S/renominare.c||"
    "silva/transformare.sh|silva/build/transformare|$S/transformare.c|-census f @T/parva.c|"
    "officina/fusor.sh|officina/build/fusor|$O/fusor.c|@T/radix|rc"
    "officina/cursor.sh|officina/build/cursor|$O/cursor.c||solum"
    "officina/interpretare.sh|officina/build/interpretare|$O/interpretare.c||solum"
)

sigillum () { stat -f '%i %m' "$1" 2>/dev/null; }

for ordo in "${INVOLUCRA[@]}"; do
    IFS='|' read -r inv bin fons args modus <<< "$ordo"
    args="${args//@T/$T}"
    titulus="$(basename "$inv" .sh)"
    eadem () { [ "$modus" = "rc" ] || cmp -s "$1" "$2"; }
    if [ "$modus" = "solum" ]; then export INSTRUMENTUM_NECTERE_SOLUM=1
    else unset INSTRUMENTUM_NECTERE_SOLUM; fi
    # calefactio: obiecta et binarium recentia
    "./$inv" ${args} > "$T/$titulus.basis" 2> /dev/null; rcb=$?
    [ -x "$bin" ] || { echo "  FRACTUM $titulus: binarium non nexum post vocationem basis (exitus $rcb)"; fracta=$((fracta + 1)); continue; }
    ante="$(sigillum "$bin")"
    "./$inv" ${args} > "$T/$titulus.i" 2> /dev/null
    post="$(sigillum "$bin")"
    [ -n "$ante" ] && [ "$ante" = "$post" ] && eadem "$T/$titulus.basis" "$T/$titulus.i"; credo $? "I $titulus: recens -> nexus nullus (binarium idem: $ante -> $post)"

    touch "$fons"
    "./$inv" ${args} > "$T/$titulus.ii" 2> /dev/null
    novum="$(sigillum "$bin")"
    [ "$novum" != "$post" ] && eadem "$T/$titulus.basis" "$T/$titulus.ii"; credo $? "II $titulus: fons tactus -> nectit (binarium novum), effusio eadem"

    concurrere () {   # $1 = nota
        local k
        for k in 1 2 3 4 5 6 7 8; do
            ( "./$inv" ${args} > "$T/$titulus.$1.$k" 2> /dev/null; echo $? > "$T/$titulus.$1.$k.rc" ) &
        done
        wait
        local mala=0
        for k in 1 2 3 4 5 6 7 8; do
            eadem "$T/$titulus.basis" "$T/$titulus.$1.$k" || mala=$((mala + 1))
            [ "$(cat "$T/$titulus.$1.$k.rc")" = "$rcb" ] || mala=$((mala + 1))
        done
        return $mala
    }
    concurrere iii; credo $? "III $titulus: VIII concurrentes recentes - exitus et effusio eadem"
    touch "$fons"
    concurrere iv; m=$?
    reliquiae="$(ls "$(dirname "$bin")" | grep -c "^$(basename "$bin")\.nexus\.")"
    [ "$m" -eq 0 ] && [ "$reliquiae" -eq 0 ] && [ -x "$bin" ]; credo $? "IV $titulus: VIII concurrentes STALAE - eadem, binarium integrum, reliquiae temporariae $reliquiae"
done

# V  INGRESSUS SECUNDARII (ratio veteris 'semper renectere': obiecta et
#    capita mutata in binario vetusto latebant): caput instrumentorum
#    (silva/instrumenta/*.h, extra newest_header vocantis) et fons
#    additus lineae nectendi (silva_lexicon.c) examen renectere debent
unset INSTRUMENTUM_NECTERE_SOLUM
for ingressus in silva/instrumenta/silva_lexicon.h silva/instrumenta/silva_lexicon.c; do
    ./silva/examen.sh "$T/parva.c" > /dev/null 2>&1
    ante="$(sigillum silva/build/examen)"
    touch "$ingressus"
    ./silva/examen.sh "$T/parva.c" > /dev/null 2>&1
    post="$(sigillum silva/build/examen)"
    [ -n "$ante" ] && [ "$ante" != "$post" ]; credo $? "V examen: $ingressus tactus -> nectit ($ante -> $post)"
done

# VI CAPUT IN DIRECTORIIS VOCANTIS (budget B1 E2, tools/caput_recens.sh):
#    caput sub silva/fontes (caput_recens_directoria examinis) tactum ->
#    obiecta recompilantur ([dep]) et binarium renectitur. Tempus capitis
#    post restituitur (touch -r): contentum idem, cascada nulla.
CAPUT_VI="$(ls silva/fontes/*.h | head -1)"
touch -r "$CAPUT_VI" "$T/caput_vi.tempus"
./silva/examen.sh "$T/parva.c" > /dev/null 2>&1
ante="$(sigillum silva/build/examen)"
sleep 1; touch "$CAPUT_VI"
dep="$(./silva/examen.sh "$T/parva.c" 2>&1 > /dev/null | grep -c '\[dep\]')"
post="$(sigillum silva/build/examen)"
touch -r "$T/caput_vi.tempus" "$CAPUT_VI"
[ "$dep" -gt 0 ] && [ -n "$ante" ] && [ "$ante" != "$post" ]; credo $? "VI examen: $CAPUT_VI tactum -> recompilat ($dep [dep]) et nectit ($ante -> $post)"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus nexus: sanum"; exit 0; fi
echo "fumus nexus: FRACTUM ($fracta)"; exit 1
