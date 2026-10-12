#!/bin/bash
# tools/organa_infigere.sh - manifesta organorum -> lib/organa_infixa.c
# (GENERATUM; status-plan S7c-1, mos organum_canon_infigere.sh).
# apps/organum_commune.stml (basis) et apps/<genus>/organum.stml omnia,
# ordine C; clavis = nomen directorii (probatio organum_montatio genus
# manifesti cum eo comparat). Sub FABRICA_SCRIPTURA exitus in
# scripturam iudicis scribitur (viae eaedem), arbor intacta.
#
# Usus: ./tools/organa_infigere.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
RADIX="${FABRICA_SCRIPTURA:-.}"
EXITUS="$RADIX/lib/organa_infixa.c"
BASIS=apps/organum_commune.stml
[ -f "$BASIS" ] || { echo "organa_infigere: $BASIS deest" >&2; exit 2; }
mkdir -p "$RADIX/lib" || exit 2

# paria "genus via", basis prima
paria () {
    printf 'organum_commune %s\n' "$BASIS"
    local f
    for f in $(LC_ALL=C ls apps/*/organum.stml); do
        printf '%s %s\n' "$(basename "$(dirname "$f")")" "$f"
    done
}

{
    printf '/* GENERATUM a tools/organa_infigere.sh ex apps/<genus>/organum.stml - noli manu mutare */\n'
    printf '#include "organa_infixa.h"\n\n#include <string.h>\n\n'
    paria | while read -r genus via; do
        printf '/* %s */\n' "$via"
        printf 'interior constans insignatus character _manifestum_%s[] = {\n' "$genus"
        od -An -v -tx1 "$via" | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
        printf '};\n\n'
    done
    printf 'nomen structura {\n'
    printf '    constans character* genus;\n'
    printf '    constans insignatus character* octeti;\n'
    printf '    i32 mensura;\n'
    printf '} OrganumInfixum;\n\n'
    printf 'interior constans OrganumInfixum _organa[] = {\n'
    paria | while read -r genus via; do
        printf '    { "%s", _manifestum_%s, (i32)magnitudo(_manifestum_%s) },\n' \
            "$genus" "$genus" "$genus"
    done
    printf '    { NIHIL, NIHIL, 0 }\n};\n\n'
    printf 'chorda\norganum_infixum_textus (\n'
    printf '    constans character* genus,\n'
    printf '               Piscina* piscina)\n{\n'
    printf '    chorda c;\n'
    printf '       i32 i;\n\n'
    printf '    c.mensura  = 0;\n'
    printf '    c.datum    = NIHIL;\n'
    printf '    si (!genus || !piscina)\n    {\n        redde c;\n    }\n'
    printf '    per (i = 0; _organa[i].genus; i++)\n    {\n'
    printf '        si (strcmp(_organa[i].genus, genus) == 0)\n        {\n'
    printf '            c.datum = (i8*)piscina_allocare(piscina,\n'
    printf '                (memoriae_index)_organa[i].mensura);\n'
    printf '            memcpy(c.datum, _organa[i].octeti,\n'
    printf '                (size_t)_organa[i].mensura);\n'
    printf '            c.mensura = _organa[i].mensura;\n'
    printf '            redde c;\n'
    printf '        }\n    }\n'
    printf '    redde c;\n}\n'
} > "$EXITUS.tmp" && mv "$EXITUS.tmp" "$EXITUS"
