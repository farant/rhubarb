#!/bin/bash

# officina/amalgamare.sh - Amalgama generare et verificare (dev-time)
#
# Usage:
#   ./amalgamare.sh          # generat amalgama/officina.c + verificat
#
# MECHANISMUS ex silva/instrumenta/silva_amalgama.{h,c} (tessera
# exemplar); MANIFESTUM in officina/instrumenta/principalia/
# amalgamator.c. Verificatio: compilatio standalone severitate plena
# (excludenda = custodia zero-inusitatorum), hospes (pollutio +
# aequivalentia aurea + vectis), intersectio nm = 0 contra objecta
# rhubarb propria.

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
SILVA_DIR="$RADIX_DIR/silva"
BUILD_DIR="$OFF_DIR/build"
AMALGAMA_DIR="$OFF_DIR/amalgama"
mkdir -p "$BUILD_DIR" "$AMALGAMA_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
)

# ---- 1. dependency objects (mechanism runtime) ----
declare -a MECHANISMI_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio"
)
# capita mutata sine recompilo = corruptio ABI (classis excubitoris;
# custos tesserae aequatus - desideratum 01KY2P5X)
newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" \
        "$SILVA_DIR/instrumenta" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${MECHANISMI_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/mech_$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done
for f in "silva_token" "silva_lexema"; do
    src="$SILVA_DIR/fontes/$f.c"
    obj="$BUILD_DIR/mech_$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done
# Mechanismus COMMUNIS ex lista communi - vide tools/
# mechanismus_fontes.sh (lista ter descripta divergit semel iam et
# amalgama huius proiecti per menses confici non potuit).
# shellcheck source=/dev/null
. "$RADIX_DIR/tools/mechanismus_fontes.sh"
for m in $(mechanismus_silvae_fontes); do
    src="$RADIX_DIR/$m"
    obj="$BUILD_DIR/mech_$(basename "$m" .c).o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
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
"$RADIX_DIR/tools/porta_vetustatis.sh" officina "$BUILD_DIR/vetustas" || exit 1

# ---- 2. build + run the amalgamator (officina manifest) ----
echo "  [officina] amalgamator.c (manifestum)"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$OFF_DIR/instrumenta/principalia/amalgamator.c" $obj_files \
    -o "$BUILD_DIR/amalgamator" || exit 1

"$BUILD_DIR/amalgamator" "$RADIX_DIR" "$AMALGAMA_DIR/officina.c" || exit 1

# ---- 3. VERIFICATIO: standalone compile under the FULL flag set ----
# (UNA exceptio includendi: silva/amalgama - officina a silva pendet
#  EX ARCHITECTURA; dependentia externa unica documentata)
echo "  [verificatio] amalgama/officina.c standalone (severitas plena)"
clang "${GCC_FLAGS[@]}" -I"$RADIX_DIR/silva/amalgama" \
    -c "$AMALGAMA_DIR/officina.c" -o "$BUILD_DIR/amalgama_verificatio.o" || exit 1

# ---- 3b. silva amalgam as object (hospes duplex) ----
src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
    echo "  [amalgama] silva.c (hospes duplex)"
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi

# ---- 4. VERIFICATIO: hospes (pollutio + aequivalentia + vectis) ----
echo "  [verificatio] hospes.c (pollutio + aequivalentia + demissio)"
clang "${GCC_FLAGS[@]}" -I"$AMALGAMA_DIR" -I"$RADIX_DIR/silva/amalgama" \
    "$OFF_DIR/instrumenta/principalia/hospes.c" \
    "$BUILD_DIR/amalgama_verificatio.o" \
    "$BUILD_DIR/amalgama_silva.o" \
    -o "$BUILD_DIR/hospes" || exit 1
"$BUILD_DIR/hospes" || exit 1

# ---- 5. VERIFICATIO: intersectio nm (symbola communia = 0) ----
nm -gU "$BUILD_DIR/amalgama_verificatio.o" | awk '{print $3}' | sort > "$BUILD_DIR/syms_amalgama.txt"
: > "$BUILD_DIR/syms_rhubarb.txt"
for f in piscina chorda chorda_aedificator xar; do
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

# ---- 6. CENSURA: censor latinae (instrumenta prima; DECISUS
# 2026-07-10: in portis compilationis a die primo; corpus
# regressionis censoris in porta SILVAE probatur - hic fontes
# officinae puri sunto) ----
if ! "$RADIX_DIR/silva/censor.sh" "$OFF_DIR"/fontes/*.c "$OFF_DIR"/fontes/*.h \
        "$OFF_DIR"/instrumenta/principalia/*.c \
        "$OFF_DIR"/probationes/probatio_*.c \
        > "$BUILD_DIR/censura.txt" 2>/dev/null; then
    echo "amalgamare: CENSURA LATINAE FRACTA:"
    grep '\[CENSURA\]\|\[SINE ARBORE\]' "$BUILD_DIR/censura.txt" | head -10
    exit 1
fi

echo "amalgamare: VERIFICATUM (standalone + hospes + nm-intersectio 0 + censura)"
