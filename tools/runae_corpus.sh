#!/bin/bash

# tools/runae_corpus.sh - corpus Lapidis pro runae (U1)
#
# Specimen textus (IV KiB, paragraphi integri) linguae cuiusque ex
# ../lapide/01_Preliminares{,_<lingua>}.html per parsatorem html domus
# (tools/runae_corpus.c) in probationes/fixa/runae/corpus/<lingua>.txt.
# Paginae sine suffixo ANGLICAE sunt; '_lt' Lapidis = LATINA -> la.txt.
#
# Usus: ./tools/runae_corpus.sh [radix_lapidis]   # ordinarius ../lapide
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta html/build/*.o cursoris html veteris - 2026-10-14).

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
LAPIS="${1:-$RADIX_DIR/../lapide}"
BIN="$RADIX_DIR/build/runae_corpus"
EXITUS="$RADIX_DIR/probationes/fixa/runae/corpus"

if [ ! -d "$LAPIS" ]; then
    echo "runae_corpus: radix Lapidis absens: $LAPIS" >&2
    exit 2
fi
mkdir -p "$RADIX_DIR/build" "$EXITUS"
BUILD_DIR="$RADIX_DIR/build/runae_corpus.d"
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SCRIPT_DIR/runae_corpus.c" "$BIN" >&2 || exit 1

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
