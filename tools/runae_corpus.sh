#!/bin/bash

# tools/runae_corpus.sh - corpus Lapidis pro runae (U1)
#
# Specimen textus (IV KiB, paragraphi integri) linguae cuiusque ex
# ../lapide/01_Preliminares{,_<lingua>}.html per parsatorem html domus
# (tools/runae_corpus.c) in probationes/fixa/runae/corpus/<lingua>.txt.
# Paginae sine suffixo ANGLICAE sunt; '_lt' Lapidis = LATINA -> la.txt.
#
# Usus: ./tools/runae_corpus.sh [radix_lapidis]   # ordinarius ../lapide
# Obiecta html/build/*.o poscit (./html/compile_probationes.sh registrum).

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
LAPIS="${1:-$RADIX_DIR/../lapide}"
HTML_BUILD="$RADIX_DIR/html/build"
BIN="$RADIX_DIR/build/runae_corpus"
EXITUS="$RADIX_DIR/probationes/fixa/runae/corpus"

source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$RADIX_DIR/materia/fontes"
    "-I$RADIX_DIR/html/fontes")

if [ -z "$(ls "$HTML_BUILD"/html_*.o 2>/dev/null)" ]; then
    echo "runae_corpus: obiecta html absunt - ./html/compile_probationes.sh registrum primum" >&2
    exit 2
fi
if [ ! -d "$LAPIS" ]; then
    echo "runae_corpus: radix Lapidis absens: $LAPIS" >&2
    exit 2
fi
OBJ=""
for o in "$HTML_BUILD"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
mkdir -p "$RADIX_DIR/build" "$EXITUS"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SCRIPT_DIR/runae_corpus.c" \
    $OBJ -o "$BIN" || exit 1

LINGUAE="en la ar bn ceb de el es fa fr gu he hi hu id ig it ja ko ml nl
pl pt ro ru rw sv sw ta th tl tr vi yo zh"
fracta=0
for lingua in $LINGUAE; do
    case "$lingua" in
        en) pagina="$LAPIS/01_Preliminares.html" ;;
        la) pagina="$LAPIS/01_Preliminares_lt.html" ;;
        *)  pagina="$LAPIS/01_Preliminares_$lingua.html" ;;
    esac
    "$BIN" "$pagina" "$EXITUS/$lingua.txt" || fracta=1
done
exit $fracta
