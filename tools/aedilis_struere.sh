#!/bin/bash

# tools/aedilis_struere.sh - fabrica aedilis (mos silva/nexus.sh)
#
# Aedilis amalgama silvae nexum eget (-Isilva/amalgama), quod ansa
# generica instrumentorum non praebet - ergo scriptum fabricae
# proprium. Obiecta in build/aedilis/, binarium bin/aedilis.
#
# Usus: ./tools/aedilis_struere.sh

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/aedilis"
mkdir -p "$BUILD_DIR" "$RADIX_DIR/bin"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
# caput silvae EX AMALGAMATE IPSO (silva-migratio T19a, ...XHAW3H):
# aedilis silva.o ex silva.c COMMISSO nectit - caput vivum manu
# mutatum ante regenerationem ABI falsam dabat (T13b: offsetus
# lexematum falsi, corpus manifesti TACITE amissum). amalgamare.sh
# aedilem ipsum poscit (circulus), ergo hic NON recusatur: par
# congruens per constructionem, monitum si caput vivum differt.
CAPUT_DIR="$BUILD_DIR/caput"
"$SCRIPT_DIR/amalgama_caput.sh" "$RADIX_DIR/silva/amalgama/silva.c" \
    "$RADIX_DIR/silva/amalgama/silva.h" "$CAPUT_DIR"
case $? in
    0|3) ;;
    *)   exit 1 ;;
esac
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$CAPUT_DIR"
)
# SE-HOSPITANS: dependentiae propriae ab aedile ipso derivatae
# (snippet commissum - ovum bootstrap fractum; regeneratio:
# ./tools/fontes_generare.sh tools/aedilis_fontes_generata.sh
#  tools/aedilis.c)
source "$SCRIPT_DIR/aedilis_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# amalgama se-continens: sine -I compilatur; silva.h mutatio formae
# sine recompilo = corruptio ABI (lectio nexus.sh). -O2: parsatio
# 1.108 -> 0.661 ms/KB mensurata, compilatio tamen sub-secundo.
SILVA_H="$RADIX_DIR/silva/amalgama/silva.h"
src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/silva.o"
if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || ! [ "$obj" -nt "$SILVA_H" ]; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -O2 -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

# provenientia (fabrica T7): manifestum per bin/aedilis PRIOREM (si
# adest) - ovum primum 'ignotum' scribit, cursus proximus corrigit
PROV_OBJ="$("$SCRIPT_DIR/provenientia_obiectum.sh" aedilis bin/aedilis tools/aedilis.c)" || exit 1
# nexus SOLUM si stalum, ATOMICE (tools/nexus_recens.sh): relinkatio
# omni vocatione memorias fabricae irritabat (LC_UUID). Ingressus:
# obiecta, fons, PLAGULA provenientiae (.c - obiectum eius semper
# recompilatur), capita (include/ + caput amalgamatis), vexilla, haec
# scripta.
# shellcheck source=/dev/null
source "$SCRIPT_DIR/nexus_recens.sh"
# shellcheck disable=SC2046,SC2086
if binarium_recens "$RADIX_DIR/bin/aedilis" $obj_files \
        "$SCRIPT_DIR/aedilis.c" "$RADIX_DIR/${PROV_OBJ%.o}.c" \
        "$SCRIPT_DIR/vexilla.sh" "$SCRIPT_DIR/aedilis_struere.sh" \
        "$SCRIPT_DIR/aedilis_fontes_generata.sh" "$SCRIPT_DIR/nexus_recens.sh" \
        $(find "$RADIX_DIR/include" "$CAPUT_DIR" -name '*.h'); then
    echo "bin/aedilis recens" >&2
else
    # shellcheck disable=SC2086
    nectere_atomice "$RADIX_DIR/bin/aedilis" "${GCC_FLAGS[@]}" \
        "${INCLUDE_FLAGS[@]}" "$SCRIPT_DIR/aedilis.c" \
        $obj_files "$RADIX_DIR/$PROV_OBJ" || exit 1
    echo "bin/aedilis paratum" >&2
fi
