#!/bin/bash

# tools/aedilis_porta.sh - PORTA CORPORIS aedilis (Phasis B)
#
# Quaeque probatio radicis: (1) derivatio + emissio scriptorum
# (bin/aedilis), (2) structura per scriptum emissum, (3) cursus
# probationis (nisi GUI/reticularis), (4) differentia-clausurae
# contra clang -MM (unio super TU clausurae). Porta per codicem
# exitus; summarium demum.
#
# Praesupponit: bin/aedilis paratum (./tools/aedilis_struere.sh)
# et artificia generata recentia (suite semel cursa - capsulae
# speculi, schemata, etc.).

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
mkdir -p build/aedilis

# ex compile_tests.sh: aedificantur, non curruntur
GUI_LISTA="probatio_fenestra probatio_vitrea probatio_vitrea_tabella probatio_vitrea_hospes probatio_delineare probatio_tempus probatio_pagina probatio_navigator probatio_combinado probatio_gradientum probatio_capsula_caudae probatio_elementa probatio_imago probatio_dithering"
# aedificantur; cursus retem vivam requirit
RETICULARIS_LISTA="probatio_tls probatio_tcp"

est_in_lista () {
    local quaesitum="$1"
    local v
    for v in $2; do
        [ "$v" = "$quaesitum" ] && return 0
    done
    return 1
}

numerus=0
fracta=""
initium=$(date +%s)

for f in probationes/probatio_*.c; do
    basis="$(basename "$f" .c)"
    case "$basis" in
        *_benchmark) continue ;;
    esac
    numerus=$((numerus + 1))

    if ! ./bin/aedilis "$f" > /dev/null 2>"build/aedilis/porta_$basis.err"; then
        echo "FRACTA (derivatio): $basis"
        head -3 "build/aedilis/porta_$basis.err"
        fracta="$fracta $basis(derivatio)"
        continue
    fi
    if ! bash "build/aedilis/$basis/struere.sh" > /dev/null 2>"build/aedilis/porta_$basis.err"; then
        echo "FRACTA (structura): $basis"
        tail -5 "build/aedilis/porta_$basis.err"
        fracta="$fracta $basis(structura)"
        continue
    fi
    if ! est_in_lista "$basis" "$GUI_LISTA" \
        && ! est_in_lista "$basis" "$RETICULARIS_LISTA"; then
        if ! "build/aedilis/$basis/$basis" > /dev/null 2>&1; then
            echo "FRACTA (cursus): $basis"
            fracta="$fracta $basis(cursus)"
            continue
        fi
    fi
    if ! ./bin/aedilis "$f" --differentia > "build/aedilis/porta_$basis.diff" 2>&1; then
        echo "DIFFERENTIA: $basis"
        grep -E "NOS SOLI|ORACULUM SOLUM|RECUSAT" \
            "build/aedilis/porta_$basis.diff" | head -5
        fracta="$fracta $basis(differentia)"
        continue
    fi
    echo "bona: $basis"
done

finis=$(date +%s)
echo ""
echo "PORTA AEDILIS: $numerus probationes, $((finis - initium))s"
if [ -n "$fracta" ]; then
    echo "FRACTAE:$fracta"
    exit 1
fi
echo "OMNES BONAE"
exit 0
