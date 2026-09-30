#!/bin/bash

# tessera/amalgamare.sh - Amalgama generare et verificare (dev-time)
#
# Usage:
#   ./amalgamare.sh          # generat amalgama/tessera.c + verificat
#
# MECHANISMUS ex silva/instrumenta/silva_amalgama.{h,c} (dependentia
# trans-plicaturarum dev-time solum - lexator silvae tokenizat);
# MANIFESTUM in tessera/instrumenta/principalia/amalgamator.c.
# Verificatio: compilatio standalone severitate plena (NULLA
# suppressio - excludenda = custodia zero-inusitatorum), hospes
# (pollutio + aequivalentia, sine terminali per pontem memoriae),
# intersectio nm = 0 contra objecta rhubarb propria.

set -u

TESSERA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$TESSERA_DIR/.." && pwd)"
SILVA_DIR="$RADIX_DIR/silva"
BUILD_DIR="$TESSERA_DIR/build"
AMALGAMA_DIR="$TESSERA_DIR/amalgama"
mkdir -p "$BUILD_DIR" "$AMALGAMA_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
    "-I$RADIX_DIR/materia/fontes"
    "-I$SILVA_DIR/instrumenta"
)

# ---- 1. dependency objects (mechanism runtime: rhubarb libs +
#         silva lexer + the shared mechanism) ----
declare -a MECHANISMI_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio"
)
# capita mutata sine recompilo = corruptio ABI (excubitor: mech_*
# stala post latina.h - eadem classis mixturae)
newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" "$RADIX_DIR/materia/fontes" \
        "$SILVA_DIR/instrumenta" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${MECHANISMI_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/mech_$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done
for f in "silva_token" "silva_lexema"; do
    src="$SILVA_DIR/fontes/$f.c"
    obj="$BUILD_DIR/mech_$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done
# lexema silvae = MateriaToken + cauda C89 (phasis V T7b): lexator
# mechanismi materia_token nectere debet
src="$RADIX_DIR/materia/fontes/materia_token.c"
obj="$BUILD_DIR/mech_materia_token.o"
if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
    echo "  [materia] materia_token.c"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"
# Mechanismus COMMUNIS ex lista communi - vide tools/
# mechanismus_fontes.sh (lista ter descripta divergit semel iam et
# amalgama huius proiecti per menses confici non potuit).
# shellcheck source=/dev/null
. "$RADIX_DIR/tools/mechanismus_fontes.sh"
for m in $(mechanismus_silvae_fontes); do
    src="$RADIX_DIR/$m"
    obj="$BUILD_DIR/mech_$(basename "$m" .c).o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $(basename "$m") (mechanismus)"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# ---- 1.5 PORTA VETUSTATIS: manifesta derivationi congruere ----
# POST obiecta, ANTE amalgamationem: probatio excludendorum
# amalgamatorem NECTIT, qui obiecta calefacta petit - porta
# vere-prima in arbore frigida se ipsam frangeret (mensuratum
# 2026-08-20). Ratio plena: tools/porta_vetustatis.sh
echo "  [porta 0] vetustas manifestorum"
# AMALGAMA_COMPARARE: porta generata IV manifesta IAM iudicavit (bis
# currere ~XXX s silvae constabat) - hic omittitur
if [ "${AMALGAMA_COMPARARE:-0}" != 1 ] && [ -z "${FABRICA_SCRIPTURA:-}" ]; then
    "$RADIX_DIR/tools/porta_vetustatis.sh" tessera "$BUILD_DIR/vetustas" || exit 1
fi

# ---- 2. build + run the amalgamator (tessera manifest) ----
echo "  [tessera] amalgamator.c (manifestum)"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$TESSERA_DIR/instrumenta/principalia/amalgamator.c" $obj_files \
    -o "$BUILD_DIR/amalgamator" || exit 1

# AMALGAMA_COMPARARE=1: generare in build/, conferre cum COMMISSO, exire
# ANTE verificationes (porta generata VI; generatio ~0,2 s): amalgama
# commissa == regeneratio hodierna. Olim nulla porta id videbat -
# amalgamata_probare compilat tantum, generata I caput solum confert
# (quaestio ...G73P, quater icta). Exitus 0 recens, 1 rancida, 2 fracta.
# FABRICA_SCRIPTURA (bin/fabrica iudicare -plenus, plan 1a T4):
# generatio SOLA in directorium scripturae, via eadem ac commissa
# (tessera/amalgama/tessera.c); nihil in arbore scribitur, nulla
# verificatio - iudex octetos confert. Exitus 0 scriptum, 2 fractum.
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    mkdir -p "$FABRICA_SCRIPTURA/tessera/amalgama" || exit 2
    "$BUILD_DIR/amalgamator" "$RADIX_DIR" \
        "$FABRICA_SCRIPTURA/tessera/amalgama/tessera.c" \
        > "$BUILD_DIR/scriptura_tessera.log" 2>&1 || exit 2
    exit 0
fi
if [ "${AMALGAMA_COMPARARE:-0}" = 1 ]; then
    "$BUILD_DIR/amalgamator" "$RADIX_DIR" "$BUILD_DIR/comparatio_tessera.c" \
        > "$BUILD_DIR/comparatio_tessera.log" 2>&1 || exit 2
    if cmp -s "$BUILD_DIR/comparatio_tessera.c" "$AMALGAMA_DIR/tessera.c"; then
        echo "amalgama tessera: recens"
        exit 0
    fi
    echo "amalgama tessera: RANCIDA ($(diff "$AMALGAMA_DIR/tessera.c" \
        "$BUILD_DIR/comparatio_tessera.c" | grep -c '^[<>]') lineae) -" \
        "./tessera/amalgamare.sh regenerat"
    exit 1
fi
"$BUILD_DIR/amalgamator" "$RADIX_DIR" "$AMALGAMA_DIR/tessera.c" || exit 1

# ---- 3. VERIFICATIO: standalone compile under the FULL flag set ----
echo "  [verificatio] amalgama/tessera.c standalone (severitas plena)"
clang "${GCC_FLAGS[@]}" \
    -c "$AMALGAMA_DIR/tessera.c" -o "$BUILD_DIR/amalgama_verificatio.o" || exit 1

# ---- 4. VERIFICATIO: hospes (pollutio + aequivalentia, headless) ----
echo "  [verificatio] hospes.c (pollutio + aequivalentia)"
clang "${GCC_FLAGS[@]}" -I"$AMALGAMA_DIR" \
    "$TESSERA_DIR/instrumenta/principalia/hospes.c" \
    "$BUILD_DIR/amalgama_verificatio.o" \
    -o "$BUILD_DIR/hospes" || exit 1
"$BUILD_DIR/hospes" || exit 1

# ---- 5. VERIFICATIO: intersectio nm (symbola communia = 0) ----
nm -gU "$BUILD_DIR/amalgama_verificatio.o" | awk '{print $3}' | sort > "$BUILD_DIR/syms_amalgama.txt"
: > "$BUILD_DIR/syms_rhubarb.txt"
for f in piscina chorda_aedificator utf8 chorda; do
    if [ -f "$BUILD_DIR/mech_$f.o" ]; then
        nm -gU "$BUILD_DIR/mech_$f.o" 2>/dev/null | awk '{print $3}' >> "$BUILD_DIR/syms_rhubarb.txt"
    fi
done
sort -o "$BUILD_DIR/syms_rhubarb.txt" "$BUILD_DIR/syms_rhubarb.txt"
COMMUNIA=$(comm -12 "$BUILD_DIR/syms_amalgama.txt" "$BUILD_DIR/syms_rhubarb.txt" | wc -l | tr -d ' ')
if [ "$COMMUNIA" != "0" ]; then
    echo "amalgamare: FRACTA - $COMMUNIA symbola communia cum rhubarb:"
    comm -12 "$BUILD_DIR/syms_amalgama.txt" "$BUILD_DIR/syms_rhubarb.txt" | head -10
    exit 1
fi

# ---- CENSURA: censor latinae (porta officinae aequata -
# desideratum 01KY2P5X; corpus regressionis censoris in porta
# SILVAE probatur - hic fontes tesserae puri sunto) ----
if ! "$RADIX_DIR/silva/censor.sh" "$TESSERA_DIR"/fontes/*.c "$TESSERA_DIR"/fontes/*.h \
        "$TESSERA_DIR"/instrumenta/principalia/*.c \
        "$TESSERA_DIR"/probationes/probatio_*.c \
        > "$BUILD_DIR/censura.txt" 2>/dev/null; then
    echo "amalgamare: CENSURA LATINAE FRACTA:"
    grep '\[CENSURA\]\|\[SINE ARBORE\]' "$BUILD_DIR/censura.txt" | head -10
    exit 1
fi

echo "amalgamare: VERIFICATUM (standalone + hospes + nm-intersectio 0 + censura)"
