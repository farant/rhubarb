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
#
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
T="$(mktemp -d "${TMPDIR:-/tmp}/nexus_fumus.XXXXXX")" || exit 2
trap 'rm -rf "${T:?}"' EXIT
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }

# involucrum | binarium | fons | argumenta vocationis (plagula parva)
INVOLUCRA=(
    "silva/selecta.sh|silva/build/selecta|silva/instrumenta/principalia/selecta.c|probationes/probatio_xar.c definitio-functionis -omnia"
)

sigillum () { stat -f '%i %m' "$1" 2>/dev/null; }

for ordo in "${INVOLUCRA[@]}"; do
    IFS='|' read -r inv bin fons args <<< "$ordo"
    titulus="$(basename "$inv" .sh)"
    # calefactio: obiecta et binarium recentia
    "./$inv" ${args} > "$T/$titulus.basis" 2> /dev/null; rcb=$?
    [ "$rcb" -le 1 ] || { echo "  FRACTUM $titulus: vocatio basis exitus $rcb"; fracta=$((fracta + 1)); continue; }
    ante="$(sigillum "$bin")"
    "./$inv" ${args} > "$T/$titulus.i" 2> /dev/null
    post="$(sigillum "$bin")"
    [ -n "$ante" ] && [ "$ante" = "$post" ] && cmp -s "$T/$titulus.basis" "$T/$titulus.i"; credo $? "I $titulus: recens -> nexus nullus (binarium idem: $ante -> $post)"

    touch "$fons"
    "./$inv" ${args} > "$T/$titulus.ii" 2> /dev/null
    novum="$(sigillum "$bin")"
    [ "$novum" != "$post" ] && cmp -s "$T/$titulus.basis" "$T/$titulus.ii"; credo $? "II $titulus: fons tactus -> nectit (binarium novum), effusio eadem"

    concurrere () {   # $1 = nota
        local k
        for k in 1 2 3 4 5 6 7 8; do
            ( "./$inv" ${args} > "$T/$titulus.$1.$k" 2> /dev/null; echo $? > "$T/$titulus.$1.$k.rc" ) &
        done
        wait
        local mala=0
        for k in 1 2 3 4 5 6 7 8; do
            cmp -s "$T/$titulus.basis" "$T/$titulus.$1.$k" || mala=$((mala + 1))
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

echo
if [ "$fracta" -eq 0 ]; then echo "fumus nexus: sanum"; exit 0; fi
echo "fumus nexus: FRACTUM ($fracta)"; exit 1
