#!/bin/bash

# officina/legatus.sh - LEGATUS: servus LSP silvae (chunk B)
#
# Usage:
#   ./officina/legatus.sh          # stdio LSP (cliens spawnat)
#
# Aedificatio incrementalis (exemplar colloquium.sh) - garrulitas
# ad stderr SOLUM (stdout purus est protocollo LSP). Aedificatio
# IUDICIS SOLIUS: amalgama silvae + praeparator + legatus - NULLA
# fontes officinae (machinula/regio absunt consulto).

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build/legatus"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$RADIX_DIR/silva/instrumenta"
    "-I$OFF_DIR/instrumenta"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$OFF_DIR/legatus_fontes_generata.sh"

# ── clausura vera inclusionum (01KXZYFVER; exemplar tabularium.sh)
# Unum clang -MM (~0.3s): dependentiae verae per obiectum (pro
# aedificatore) et unio earum (pro manifesto vigiliae) ex UNO
# fonte. Antea manifestum capita lib/ OMITTEBAT (sub-tectio: editio
# piscina.h numquam monebat NEC recompilabat) et conditio nexus
# semper vera erat (renexus quovis lancinatu). Cautio: basis
# 'legatus' bis exsistit (instrumenta/ + principalia/) - lineae
# congruae plures = unio dependentiarum, superset tutum. Si -MM
# cadit: newest_header = glob superset (garrulum, numquam caecum).
newest_header () {
    find "$RADIX_DIR/include" "$RADIX_DIR/silva/amalgama" \
         "$RADIX_DIR/silva/instrumenta" "$OFF_DIR/instrumenta" \
         -name '*.h' -newer "$1" 2>/dev/null | head -1
}
FONTES_OMNES=()
for f in "${RADIX_FONTES[@]}"; do FONTES_OMNES+=("$RADIX_DIR/lib/$f.c"); done
FONTES_OMNES+=("$RADIX_DIR/silva/amalgama/silva.c")
FONTES_OMNES+=("$RADIX_DIR/silva/instrumenta/nexus_ordines.c")
FONTES_OMNES+=("$RADIX_DIR/silva/instrumenta/silva_lexicon.c")
FONTES_OMNES+=("$OFF_DIR/instrumenta/praeparator.c")
FONTES_OMNES+=("$OFF_DIR/instrumenta/legatus.c")
FONTES_OMNES+=("$OFF_DIR/instrumenta/principalia/legatus.c")

DEPENDENTIAE="$(clang -MM "${INCLUDE_FLAGS[@]}" "${FONTES_OMNES[@]}" 2>/dev/null \
    | awk '{ if (sub(/\\$/,"")) printf "%s ", $0; else print }')"

deps_obiecti () {   # $1 = basis (sine .c); lineae plures = unio tuta
    printf '%s\n' "$DEPENDENTIAE" | grep "^$1\.o:" | tr ' ' '\n' | grep '^/'
}
recentius_ex () {   # $1 = scopus; stdin = viae; prima recentior imprimitur
    while IFS= read -r via; do
        [ -n "$via" ] && [ "$via" -nt "$1" ] && { echo "$via"; return 0; }
    done
    return 0
}
fons_stalus () {    # $1 = basis, $2 = scopus; exitus 0 = recompilandum
    if [ -n "$DEPENDENTIAE" ]; then
        [ -n "$(deps_obiecti "$1" | recentius_ex "$2")" ]
    else
        [ -n "$(newest_header "$2")" ]
    fi
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus "$f" "$obj"; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# silva.h mutatio formae sine recompilo = corruptio ABI (stalum
# obiectum + amalgama recens - inventum v0.2, SilvaMacroVista) -
# nunc per clausuram veram tecta: silva.h in dependentiis omnium
# consumptorum est, fons_stalus eos recompilat
src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus silva "$obj"; then
    echo "  [amalgama] silva.c" >&2
    clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

src="$RADIX_DIR/silva/instrumenta/nexus_ordines.c"
obj="$BUILD_DIR/nexus_ordines.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus nexus_ordines "$obj"; then
    echo "  [ordines] nexus_ordines.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

# silva_lexicon (compositio systematis - praeparator et legatus)
src="$RADIX_DIR/silva/instrumenta/silva_lexicon.c"
obj="$BUILD_DIR/silva_lexicon.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus silva_lexicon "$obj"; then
    echo "  [lexicon] silva_lexicon.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for unit in praeparator legatus; do
    src="$OFF_DIR/instrumenta/$unit.c"
    obj="$BUILD_DIR/$unit.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus "$unit" "$obj"; then
        echo "  [$unit] $unit.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

BIN="$BUILD_DIR/legatus"
src="$OFF_DIR/instrumenta/principalia/legatus.c"
# antea: [ -n "$obj_files" ] semper verum -> renexus QUOVIS
# lancinatu; nunc obiectum recentius binario aut fons/capita
# principalis (exemplar tabularium.sh)
obj_recentius=""
for o in $obj_files; do
    if [ "$o" -nt "$BIN" ]; then obj_recentius="$o"; break; fi
done
if [ ! -f "$BIN" ] || [ "$src" -nt "$BIN" ] || [ -n "$obj_recentius" ] || fons_stalus legatus "$BIN"; then
    echo "  [nexus] legatus" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$src" $obj_files -o "$BIN" || exit 1
fi

# -radix semper appenditur: modus MCP rootUri caret (LSP rootUri
# praevalet cum adest - _radicem_statuere cfg->radix... ordo: cfg
# vincit, ideo LSP quoque radicem rectam accipit)
# vigilia (lib/vigilia): -signum = sigillum binarii ->
# serverInfo.version; -manifestum = clausura quam HIC launcher
# compilat (invarians: aedificator indicem scribit - vigil et
# aedificator dissentire non possunt)
MANIFEST="$BUILD_DIR/legatus.vigilia"
{
    echo "# clausura vera launcheri (clang -MM; 01KXZYFVER)"
    if [ -n "$DEPENDENTIAE" ]; then
        printf '%s\n' "$DEPENDENTIAE" | tr ' ' '\n' | grep '^/' | sort -u
    else
        # regressus: -MM cecidit - lista vetus manu scripta
        for f in "${FONTES_OMNES[@]}"; do echo "$f"; done
        echo "$RADIX_DIR/silva/amalgama/silva.h"
        echo "$RADIX_DIR/silva/instrumenta/nexus_ordines.h"
        echo "$RADIX_DIR/silva/instrumenta/silva_lexicon.h"
        echo "$OFF_DIR/instrumenta/praeparator.h"
        echo "$OFF_DIR/instrumenta/legatus.h"
    fi
} > "$MANIFEST" 2>/dev/null
SIGNUM="$(shasum -a 256 "$BIN" 2>/dev/null | cut -c1-64)"
if [ -n "$SIGNUM" ]; then
    cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR" -signum "$SIGNUM" -manifestum "$MANIFEST"
else
    cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR"
fi
