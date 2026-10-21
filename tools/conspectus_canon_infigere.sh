#!/bin/bash
# tools/conspectus_canon_infigere.sh - conspectus.canon -> lib/conspectus_canon.c
# (GENERATUM; proiectiones D4a - mos organum_canon_infigere.sh). Sub FABRICA_SCRIPTURA exitus in
# scripturam iudicis scribitur (viae eaedem), arbor intacta.
#
# Usus: ./tools/conspectus_canon_infigere.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
FONS=conspectus.canon
RADIX="${FABRICA_SCRIPTURA:-.}"
EXITUS="$RADIX/lib/conspectus_canon.c"
[ -f "$FONS" ] || { echo "conspectus_canon_infigere: $FONS deest" >&2; exit 2; }
mkdir -p "$RADIX/lib" || exit 2
{
    printf '/* GENERATUM a tools/conspectus_canon_infigere.sh ex conspectus.canon - noli manu mutare */\n'
    printf '#include "conspectus_canon.h"\n\n#include <string.h>\n\n'
    printf 'interior constans insignatus character _octeti[] = {\n'
    od -An -v -tx1 "$FONS" | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
    printf '};\n\n'
    printf 'chorda\nconspectus_canon_textus (\n    Piscina* piscina)\n{\n'
    printf '    chorda c;\n\n'
    printf '    c.mensura  = (i32)magnitudo(_octeti);\n'
    printf '    c.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura);\n'
    printf '    memcpy(c.datum, _octeti, (size_t)c.mensura);\n'
    printf '    redde c;\n}\n'
} > "$EXITUS.tmp" && mv "$EXITUS.tmp" "$EXITUS"
