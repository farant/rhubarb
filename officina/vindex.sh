#!/bin/bash

# officina/vindex.sh - VINDEX: debugger TUI (M3)
#
# Usage:
#   ./officina/vindex.sh <suita.c> [-plagulae a,b,c] [-imago via:linea]
#
# App super amalgamata TRIA (saltuarius exemplar): officina + silva +
# tessera obiecta intrant; fontes/ include dirs CONSULTO absunt
# (limes amalgamatis = API). Currendum ex radice repositorii.

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build/vindex"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/silva/amalgama"
    "-I$RADIX_DIR/silva/instrumenta"
    "-I$RADIX_DIR/tessera/amalgama"
    "-I$OFF_DIR/amalgama"
    "-I$OFF_DIR/instrumenta"
)

# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$OFF_DIR/vindex_fontes_generata.sh"

# capita mutata sine recompilo = corruptio ABI (excubitor:
# vindex_onerator.o/amalgama_officina.o stala post silva.h v0.2 -
# mixtura corrumpens in proximo cursu; auditus prior hanc plagulam
# per COMMENTARIUM "silva.h" falso exoneravit)
newest_header () {
    find "$RADIX_DIR/include" "$OFF_DIR/instrumenta" \
        "$RADIX_DIR/silva/amalgama" "$RADIX_DIR/silva/instrumenta" "$RADIX_DIR/silva/fontes" \
        "$RADIX_DIR/tessera/amalgama" \
        "$OFF_DIR/amalgama" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for nom in "tessera" "silva" "officina"; do
    src="$RADIX_DIR/$nom/amalgama/$nom.c"
    obj="$BUILD_DIR/amalgama_$nom.o"
    # officina amalgam: silva.h = the ONE documented external dep
    # (silva amalgama self-contained; tessera postulata_posix.h poscit)
    extra=""
    silva_h_recentior=""
    if [ "$nom" = "tessera" ]; then
        extra="-I$RADIX_DIR/include"   # postulata_posix.h (sutura)
    fi
    if [ "$nom" = "officina" ]; then
        extra="-I$RADIX_DIR/silva/amalgama"
        [ "$RADIX_DIR/silva/amalgama/silva.h" -nt "$obj" ] 2>/dev/null \
            && silva_h_recentior="1"
    fi
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$silva_h_recentior" ]; then
        echo "  [amalgama] $nom.c"
        clang "${GCC_FLAGS[@]}" $extra -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# silva_lexicon (compositio systematis - praeparator eam vocat)
src="$RADIX_DIR/silva/instrumenta/silva_lexicon.c"
obj="$BUILD_DIR/silva_lexicon.o"
if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
    || [ -n "$(newest_header "$obj")" ]; then
    echo "  [lexicon] silva_lexicon.c"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "-I$RADIX_DIR/silva/fontes" -c "$src" -o "$obj" || exit 1
fi
obj_files="$obj_files $obj"

for f in "praeparator" "vindex_onerator" "vindex_visum"; do
    src="$OFF_DIR/instrumenta/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] \
        || [ -n "$(newest_header "$obj")" ]; then
        echo "  [vindex] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

src="$OFF_DIR/instrumenta/principalia/vindex.c"
bin="$BUILD_DIR/vindex"
if [ ! -f "$bin" ] || [ "$src" -nt "$bin" ] || [ -n "$(find $obj_files -newer "$bin" 2>/dev/null | head -1)" ]; then
    echo "  [vindex] vindex.c (principale)"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$src" $obj_files -o "$bin" || exit 1
fi

# canalis excubitoris: verdictum post-constructionem (tacet nisi stala)
"$RADIX_DIR/excubitor.sh" -tacitus "officina/build/vindex/" >&2 || true

cd "$RADIX_DIR"
exec "$bin" "$@"
