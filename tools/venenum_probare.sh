#!/bin/bash
# tools/venenum_probare.sh - porta: suitae piscinarum temporariarum sub
# VENENO (piscina compilata cum -DPISCINA_VENENUM=1) et ASan/UBSan
#
# piscina_reficere memoriam nec liberat nec delet: valor qui
# refectionem superstat (vitium vitae) recte legi videtur et suita
# ordinaria viridis manet. Sub veneno octeti liberati 0xA5 fiunt, ergo
# talis lectio effectum falsum dat (recensio polynomium-II: duo vitia
# plantata - officina falsa refecta, refectio ante transcriptionem -
# sola via venenata capta).
#
# I.  specimen: lectio post refectionem 0xA5 (CLXV) videt sub veneno et
#     valorem veterem sine eo - modus vivus et ordinarie extinctus
#     (porta muta hic caperetur);
# II. suitae quae piscinas notant et reficiunt in ansis calidis:
#     piscina, magnus (Euclides alternus), fractio, situs, polynomium
#     (officinae), matrix (Bareiss in officinis alternis), sub veneno et
#     sanitatoribus.
# Exitus 0 sana | 1 FRACTA | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
source tools/vexilla.sh
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
SANITAS=("-g" "-O1" "-fsanitize=address,undefined" "-fno-sanitize-recover=all")
BASIS=(lib/piscina.c lib/chorda.c lib/chorda_aedificator.c lib/credo.c lib/sors.c)

# I. specimen
cat > "$T/specimen.c" <<'FONS'
#include "latina.h"
#include "piscina.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina*        piscina = piscina_generare_dynamicum("specimen", 4096);
    PiscinaNotatio  nota    = piscina_notare(piscina);
    i8*             octeti  = (i8*)piscina_allocare(piscina, XVI);

    octeti[ZEPHYRUM] = XLII;
    piscina_reficere(piscina, nota);
    imprimere("%u\n", (i32)octeti[ZEPHYRUM]);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
FONS
clang "${VEXILLA_C89[@]}" -Iinclude "${SANITAS[@]}" -DPISCINA_VENENUM=1 \
    -o "$T/specimen_venenatum" "$T/specimen.c" lib/piscina.c 2>&1 | head -5
clang "${VEXILLA_C89[@]}" -Iinclude "${SANITAS[@]}" \
    -o "$T/specimen_purum" "$T/specimen.c" lib/piscina.c 2>&1 | head -5
[ "$("$T/specimen_venenatum" 2>&1)" = "165" ]; credo $? "sub veneno lectio post refectionem 0xA5 videt"
[ "$("$T/specimen_purum" 2>&1)" = "42" ]; credo $? "sine veneno valor vetus (modus ordinarie extinctus)"

# II. suitae
for suita in "piscina:" "magnus:lib/magnus.c" \
    "fractio:lib/magnus.c lib/fractio.c" \
    "situs:lib/magnus.c lib/fractio.c lib/situs.c" \
    "polynomium:lib/magnus.c lib/fractio.c lib/polynomium.c" \
    "matrix:lib/magnus.c lib/fractio.c lib/polynomium.c lib/congruentia.c \
lib/anulus.c lib/matrix.c"; do
    titulus="${suita%%:*}"
    read -r -a fontes <<< "${suita#*:}"
    if ! clang "${VEXILLA_C89[@]}" -Wno-overlength-strings -Iinclude \
        "${SANITAS[@]}" -DPISCINA_VENENUM=1 -o "$T/$titulus" \
        "probationes/probatio_$titulus.c" ${fontes[@]+"${fontes[@]}"} \
        "${BASIS[@]}" > "$T/$titulus.aedificatio" 2>&1; then
        head -5 "$T/$titulus.aedificatio"
        credo 1 "probatio_$titulus aedificata"
        continue
    fi
    "$T/$titulus" > "$T/$titulus.log" 2>&1
    exitus=$?
    if [ "$exitus" -ne 0 ]; then
        grep -a "FRACTA\|ERROR\|runtime error" "$T/$titulus.log" | head -5
    fi
    credo "$exitus" "probatio_$titulus sub veneno ($(grep -a -o 'Totalis: *[0-9]*' "$T/$titulus.log" | tr -s ' '))"
done
echo
if [ "$fracta" -eq 0 ]; then echo "venenum: sana"; exit 0; fi
echo "venenum: FRACTA ($fracta)"; exit 1
