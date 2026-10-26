#!/bin/bash
# tools/conspectus_infigere.sh - declarationes lib/conspectus_*.stml ->
# lib/conspectus_infixa.c (GENERATUM; proiectiones D4b, mos
# organa_infigere.sh). Clavis = nomen plagulae sine 'conspectus_' et
# '.stml' ("dies"). Sub FABRICA_SCRIPTURA exitus in scripturam iudicis
# scribitur (viae eaedem), arbor intacta.
#
# Usus: ./tools/conspectus_infigere.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
RADIX="${FABRICA_SCRIPTURA:-.}"
EXITUS="$RADIX/lib/conspectus_infixa.c"
mkdir -p "$RADIX/lib" || exit 2

# paria "titulus via", ordine C
paria () {
    local f t
    for f in $(LC_ALL=C ls lib/conspectus_*.stml 2>/dev/null); do
        t=$(basename "$f" .stml)
        printf '%s %s\n' "${t#conspectus_}" "$f"
    done
}

{
    printf '/* GENERATUM a tools/conspectus_infigere.sh ex lib/conspectus_*.stml - noli manu mutare */\n'
    printf '#include "conspectus_infixa.h"\n\n#include <string.h>\n\n'
    paria | while read -r titulus via; do
        printf '/* %s */\n' "$via"
        printf 'interior constans insignatus character _declaratio_%s[] = {\n' "$titulus"
        od -An -v -tx1 "$via" | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
        printf '};\n\n'
    done
    printf 'nomen structura {\n'
    printf '    constans character* titulus;\n'
    printf '    constans insignatus character* octeti;\n'
    printf '    i32 mensura;\n'
    printf '} DeclaratioInfixa;\n\n'
    printf 'interior constans DeclaratioInfixa _declarationes[] = {\n'
    paria | while read -r titulus via; do
        printf '    { "%s", _declaratio_%s, (i32)magnitudo(_declaratio_%s) },\n' \
            "$titulus" "$titulus" "$titulus"
    done
    printf '    { NIHIL, NIHIL, 0 }\n};\n\n'
    printf 'chorda\nconspectus_declaratio_infixa (\n'
    printf '    constans character* titulus,\n'
    printf '               Piscina* piscina)\n{\n'
    printf '    chorda c;\n'
    printf '       i32 i;\n\n'
    printf '    c.mensura  = 0;\n'
    printf '    c.datum    = NIHIL;\n'
    printf '    si (!titulus || !piscina)\n    {\n        redde c;\n    }\n'
    printf '    per (i = 0; _declarationes[i].titulus; i++)\n    {\n'
    printf '        si (strcmp(_declarationes[i].titulus, titulus) == 0)\n        {\n'
    printf '            c.datum = (i8*)piscina_allocare(piscina,\n'
    printf '                (memoriae_index)_declarationes[i].mensura);\n'
    printf '            memcpy(c.datum, _declarationes[i].octeti,\n'
    printf '                (size_t)_declarationes[i].mensura);\n'
    printf '            c.mensura = _declarationes[i].mensura;\n'
    printf '            redde c;\n'
    printf '        }\n    }\n'
    printf '    redde c;\n}\n'
} > "$EXITUS.tmp" && mv "$EXITUS.tmp" "$EXITUS"
