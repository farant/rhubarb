#!/bin/bash
# materia/coquere.sh - registrum coctum ex declaratione STML coquere
# aut rancorem probare (instrumentum materia/instrumenta/coctor.c)
#
# Usage:  ./materia/coquere.sh <declaratio.stml> [-scribere] [-canon X]
# Exit:   0 recens (aut scriptum) | 1 RANCIDUM | 2 defectus
#         (declaratio absens aut recusata, canon fractus)
#
# Sine -scribere: declarationem contra materia/grammatica/registrum.canon
# iudicat, textus reddit, contra plagulas in 'sedes' declarationis
# confert (output confert, numquam tempora). -scribere eas scribit.
# Currit e RADICE repositorii (RHUBARB_RADIX): viae relativae ad
# radicem, ut probatio clientis et instrumentum eundem commentarium
# 'Fons:' generent.
#
# Obiecta in materia/build/coctor (lib + materia_coctor), seorsum ab
# obiectis suitae (sera cursoris non tangitur); binarium
# materia/build/coctor/coctor, restructum cum fons novior.
set -u
MATERIA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$MATERIA_DIR/.." && pwd)"
BUILD_DIR="$MATERIA_DIR/build/coctor"
BIN="$BUILD_DIR/coctor"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes")

# canon: stml_macros + similitudo (iudicium expansum); filum: plagulae
FONTES=(
    canon stml stml_macros similitudo chorda chorda_aedificator
    piscina xar internamentum tabula_dispersa friatio selectio filum
)

newest_header () {
    find "$RADIX_DIR/include" "$MATERIA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
    fi
    obj_files="$obj_files $obj"
done
src="$MATERIA_DIR/fontes/materia_coctor.c"
obj="$BUILD_DIR/materia_coctor.o"
if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
    echo "  [materia] materia_coctor.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
fi
obj_files="$obj_files $obj"

SRC="$MATERIA_DIR/instrumenta/coctor.c"
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$(find $obj_files -newer "$BIN" 2>/dev/null | head -1)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $obj_files -o "$BIN" || exit 2
fi

cd "$RADIX_DIR" || exit 2
RHUBARB_RADIX="$RADIX_DIR" "$BIN" "$@"
exit $?
