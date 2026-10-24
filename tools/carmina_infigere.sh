#!/bin/bash
# tools/carmina_infigere.sh - carmina.stml + instrumentarium.stml ->
# lib/carmina_infixum.c (GENERATUM; liber ordinarius et instrumentarium
# include/carmen.h). Sub
# FABRICA_SCRIPTURA exitus in scripturam iudicis scribitur (viae
# eaedem), arbor intacta.
#
# Usus: ./tools/carmina_infigere.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
FONS=carmina.stml
INSTRUMENTARIUM=instrumentarium.stml
RADIX="${FABRICA_SCRIPTURA:-.}"
EXITUS="$RADIX/lib/carmina_infixum.c"
[ -f "$FONS" ] || { echo "carmina_infigere: $FONS deest" >&2; exit 2; }
[ -f "$INSTRUMENTARIUM" ] || {
    echo "carmina_infigere: $INSTRUMENTARIUM deest" >&2; exit 2; }
mkdir -p "$RADIX/lib" || exit 2
{
    printf '/* GENERATUM a tools/carmina_infigere.sh ex carmina.stml et instrumentarium.stml - noli manu mutare */\n'
    printf '#include "carmina_infixum.h"\n\n#include <string.h>\n\n'
    printf 'interior constans insignatus character _octeti[] = {\n'
    od -An -v -tx1 "$FONS" | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
    printf '};\n\n'
    printf 'chorda\ncarmina_infixum_textus (\n    Piscina* piscina)\n{\n'
    printf '    chorda c;\n\n'
    printf '    c.mensura  = (i32)magnitudo(_octeti);\n'
    printf '    c.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura);\n'
    printf '    memcpy(c.datum, _octeti, (size_t)c.mensura);\n'
    printf '    redde c;\n}\n\n'
    printf 'interior constans insignatus character _octeti_instrumentarii[] = {\n'
    od -An -v -tx1 "$INSTRUMENTARIUM" | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
    printf '};\n\n'
    printf 'chorda\ninstrumentarium_infixum_textus (\n    Piscina* piscina)\n{\n'
    printf '    chorda c;\n\n'
    printf '    c.mensura  = (i32)magnitudo(_octeti_instrumentarii);\n'
    printf '    c.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura);\n'
    printf '    memcpy(c.datum, _octeti_instrumentarii, (size_t)c.mensura);\n'
    printf '    redde c;\n}\n'
} > "$EXITUS.tmp" && mv "$EXITUS.tmp" "$EXITUS"
