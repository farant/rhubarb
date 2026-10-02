#!/bin/bash

# tools/natura_struere.sh - fabrica natura_examen (mos aedilis_struere.sh)
#
# Porta velox bibliothecae semanticae: onerator lib/natura.c cum
# facie CLI. Obiecta in build/natura/, binarium bin/natura_examen.
#
# Usus: ./tools/natura_struere.sh

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/natura"
mkdir -p "$BUILD_DIR" "$RADIX_DIR/bin"

# SERA (2026-10-02, frigida_probare): probationes natura (glossae,
# quaesitor, canones) hoc scriptum IPSAE vocant si binarium abest - in
# clone recenti parallelae eadem obiecta et binaria simul scribebant
# ('structor fefellit (codex 1)'). Sera domus (tools/sera.sh): vocatio
# secunda exspectat (linea una in stderr), deinde obiecta recentia et
# binaria recentia invenit et nihil nectit.
# shellcheck source=/dev/null
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/struere.sera" || exit 2

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=( "-I$RADIX_DIR/include" )

# clausura oneratoris (bin/aedilis --partes natura.c congruens)
# stml_macros: canon.c stml_expandere vocat (iudicium expansum)
FONTES=(
    natura canon stml stml_macros similitudo chorda
    chorda_aedificator piscina xar
    internamentum tabula_dispersa friatio selectio
    filum iter_directoria via provenientia
)

newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# nectere <titulus> <binarium relativum> <fons provenientiae> <argumenta
# clang...>: SOLUM si stalum, ATOMICE (tools/nexus_recens.sh - sedes una
# cum aedilis_struere.sh). Ingressus: obiecta, fontes, PLAGULA
# provenientiae (.c - obiectum eius SEMPER recompilatur, ergo mtime eius
# nihil dicit), vexilla, haec scripta.
# shellcheck source=/dev/null
source "$SCRIPT_DIR/nexus_recens.sh"
nectere () {
    local titulus="$1" binarium="$2" fons_prov="$3" prov exitus
    shift 3
    prov="$("$SCRIPT_DIR/provenientia_obiectum.sh" "$titulus" "$binarium" "$fons_prov" natura)" || exit 1
    exitus="$RADIX_DIR/$binarium"
    # shellcheck disable=SC2086
    if binarium_recens "$exitus" $obj_files "$RADIX_DIR/${prov%.o}.c" \
            "$@" "$RADIX_DIR/tools/vexilla.sh" \
            "$SCRIPT_DIR/natura_struere.sh" "$SCRIPT_DIR/nexus_recens.sh"; then
        echo "$binarium recens" >&2
        return 0
    fi
    # shellcheck disable=SC2086
    nectere_atomice "$exitus" "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
        "$@" $obj_files "$RADIX_DIR/$prov" || exit 1
    echo "$binarium paratum" >&2
}

nectere natura_examen bin/natura_examen tools/natura_examen.c \
    "$SCRIPT_DIR/natura_examen.c"
nectere natura_canones bin/natura_canones tools/natura_canones.c \
    "-I$SCRIPT_DIR" "$SCRIPT_DIR/natura_canones.c" \
    "$SCRIPT_DIR/natura_canones_emissio.c"
nectere natura_glossae bin/natura_glossae tools/natura_glossae.c \
    "$SCRIPT_DIR/natura_glossae.c"
nectere natura bin/natura tools/natura_quaesitor.c \
    "$SCRIPT_DIR/natura_quaesitor.c"
