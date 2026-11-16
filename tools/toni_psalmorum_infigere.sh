#!/bin/bash
# tools/toni_psalmorum_infigere.sh - musica/toni_psalmorum.stml ->
# lib/toni_psalmorum_infixum.c (GENERATUM; tabula tonorum psalmorum,
# include/psalmodia_tabula.h; psalmodia plan-2 T7, mos
# cantuale_infigere.sh). Sub FABRICA_SCRIPTURA exitus in scripturam
# iudicis scribitur (viae eaedem), arbor intacta.
#
# Usus: ./tools/toni_psalmorum_infigere.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
FONS=musica/toni_psalmorum.stml
RADIX="${FABRICA_SCRIPTURA:-.}"
EXITUS="$RADIX/lib/toni_psalmorum_infixum.c"
[ -f "$FONS" ] || { echo "toni_psalmorum_infigere: $FONS deest" >&2; exit 2; }
mkdir -p "$RADIX/lib" || exit 2
{
    printf '/* GENERATUM a tools/toni_psalmorum_infigere.sh ex musica/toni_psalmorum.stml - noli manu mutare */\n'
    printf '#include "toni_psalmorum_infixum.h"\n\n#include <string.h>\n\n'
    printf 'interior constans insignatus character _octeti[] = {\n'
    od -An -v -tx1 "$FONS" | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
    printf '};\n\n'
    printf 'chorda\ntoni_psalmorum_infixum_textus (\n    Piscina* piscina)\n{\n'
    printf '    chorda c;\n\n'
    printf '    c.mensura  = (i32)magnitudo(_octeti);\n'
    printf '    c.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura);\n'
    printf '    memcpy(c.datum, _octeti, (size_t)c.mensura);\n'
    printf '    redde c;\n}\n'
} > "$EXITUS.tmp" && mv "$EXITUS.tmp" "$EXITUS"
