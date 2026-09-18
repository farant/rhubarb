#!/bin/bash
# tools/diagnostica.sh - diagnostica plagularum per clientes materiae
#
# Usage:  ./tools/diagnostica.sh <plagula...> [-machina] [-sine-excerpto]
# Exit:   0 nullum erratum | 1 erratum | 2 nihil iudicatum
# Obiecta SUA in build/diagnostica/: aedificia crustae et css obiecta
# materiae et bibliothecae sua quaeque ferunt - coniuncta colliderent.
set -u
RADIX_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/diagnostica"
BIN="$BUILD_DIR/diagnostica"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$RADIX_DIR/materia/fontes" "-I$RADIX_DIR/crusta/fontes" "-I$RADIX_DIR/css/fontes")
mkdir -p "$BUILD_DIR" || exit 2
CAPITA_NOVA=""
if [ -f "$BIN" ]; then
    CAPITA_NOVA="$(find "$RADIX_DIR/include" "$RADIX_DIR/materia/fontes" "$RADIX_DIR/crusta/fontes" "$RADIX_DIR/css/fontes" -name '*.h' -newer "$BIN" 2>/dev/null | head -1)"
fi
OBJ=""
compilare () {
    # in aequalitate temporum RECOMPILA ('-nt' secundis mensuratur)
    if [ ! -f "$2" ] || ! [ "$2" -nt "$1" ] || [ -n "$CAPITA_NOVA" ]; then
        rm -f "$2"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$1" -o "$2" >&2 || {
            echo "diagnostica: compilatio fracta: $1" >&2; exit 2; }
    fi
    OBJ="$OBJ $2"
}
for f in piscina chorda chorda_aedificator xar friatio tabula_dispersa \
         internamentum stml stml_macros selectio similitudo canon credo \
         processus_posix css_lexema excerptum utf8 \
         iter_directoria via; do
    compilare "$RADIX_DIR/lib/$f.c" "$BUILD_DIR/$f.o"
done
for m in materia_lexicon materia_token materia_nodus materia_scribere \
         materia_arbor materia_arbor_aequalitas materia_coctor \
         materia_diagnostica materia_annotationes materia_excusatio \
         materia_pictor materia_exemplaria; do
    compilare "$RADIX_DIR/materia/fontes/$m.c" "$BUILD_DIR/$m.o"
done
for src in "$RADIX_DIR"/crusta/fontes/*.c "$RADIX_DIR"/css/fontes/*.c; do
    compilare "$src" "$BUILD_DIR/$(basename "$src" .c).o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$RADIX_DIR/tools/diagnostica.c" ] \
   || [ -n "$(find "$BUILD_DIR" -name '*.o' -newer "$BIN" 2>/dev/null | head -1)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$RADIX_DIR/tools/diagnostica.c" $OBJ -o "$BIN" >&2 || {
        echo "diagnostica: ligatio fracta" >&2; exit 2; }
fi
exec "$BIN" "$@"
