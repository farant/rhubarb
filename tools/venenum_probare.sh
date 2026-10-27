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
# II. suitae (compositum probationes_veneni) quae piscinas notant et reficiunt in ansis calidis:
#     piscina, magnus (Euclides alternus), fractio, situs, polynomium
#     (officinae), matrix (Bareiss in officinis alternis), laqueus,
#     extensio (inversa et norma per Bareiss), quaternio (nullam
#     piscinam ipse reficit, sed icosianos super extensionem et Q per
#     sanitatores agit), radices (acervus VCA per notas LIFO, opus post
#     quodque opus et quamque bisectionem refectum), cellae (candidata
#     catalogi refutata piscinam ad notam reficiunt), reticulum (officina
#     per vocationem publicam, effecta transcripta), chorda_aedificator
#     (piscina_conari_ampliare via II sub veneno SEMPER movet: appendix
#     sui sine re-derivatione fontis venenum legeret, 2026-10-21), sub
#     veneno et sanitatoribus.
# Exitus 0 sana | 1 FRACTA | 2 nihil actum.
# <porta titulus="venenum" gradus="fusio" tegit="probationes/probatio_extensio.c probationes/probatio_fractio.c probationes/probatio_laqueus.c probationes/probatio_magnus.c probationes/probatio_matrix.c probationes/probatio_piscina.c probationes/probatio_polynomium.c probationes/probatio_quaternio.c probationes/probatio_radices.c probationes/probatio_reticulum.c probationes/probatio_situs.c probationes/probatio_cellae.c probationes/probatio_plexus.c probationes/probatio_nervus.c probationes/probatio_dependentia.c probationes/probatio_chorda_aedificator.c" currit="tools/fabrica.c lib/anulus.c lib/congruentia.c lib/cyclotomia.c lib/extensio.c lib/fractio.c lib/laqueus.c lib/contractio.c lib/interpolatio.c lib/magnus.c lib/matrix.c lib/polynomium.c lib/quaternio.c lib/radices.c lib/reticulum.c lib/situs.c lib/cellae.c lib/surdus.c lib/plexus.c lib/nervus.c lib/dependentia.c" translatio="numquam"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
source tools/vexilla.sh
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
# -Wno-deprecated-declarations: SDK macOS sprintf 'deprecated' notat SOLUM
# sub sanitatoribus (aedificatio ordinaria tacet); C89 snprintf caret, ergo
# sprintf domus legitimum est (cyclotomia, extensio)
SANITAS=("-g" "-O1" "-fsanitize=address,undefined" "-fno-sanitize-recover=all"
    "-Wno-deprecated-declarations")
# (eadem vexilla in actione probationes_veneni - mutata utrimque mutanda)

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

# II. suitae: compositum probationes_veneni (aedificatio.stml, 2026-10-22)
# - membra nominata, vexilla variantis in clave membri: obiecta sub
# sanitatoribus per thesaurum compilatoris SEMEL structa (olim suita
# quaeque catenam suam iterum compilabat: ~CDLXXXVII s), membra quorum
# ingressus immutati sunt servantur (~I s). Membra et fontes eorum
# clausura aedilis dat, non lista manu.
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >&2 \
    || { echo "venenum: bin/fabrica struere nequit - NULLA PROBATIO CURSA"; exit 2; }
bin/fabrica probare probationes_veneni
exitus=$?
[ "$exitus" -eq 2 ] && { echo "venenum: NULLA PROBATIO CURSA (fabrica exitus 2)"; exit 2; }
credo "$exitus" "probationes_veneni (fabrica probare)"
echo
if [ "$fracta" -eq 0 ]; then echo "venenum: sana"; exit 0; fi
echo "venenum: FRACTA ($fracta)"; exit 1
