#!/bin/bash
# tools/psalterium_infigere.sh - musica/psalmi/1.txt..150.txt et
# musica/psalmi/LICENTIA -> lib/psalterium_infixum.c (GENERATUM;
# include/psalterium_infixum.h; psalterium planum T4, mos
# toni_psalmorum_infigere.sh). Octeti omnes in tabula una (psalmi
# ordine, licentia ultima) cum tabula initiorum CLI: initium psalmi N
# est _initia[N - 1], finis _initia[N]; licentia ab _initia[CL] ad
# finem tabulae. Sub FABRICA_SCRIPTURA exitus in scripturam iudicis
# scribitur (viae eaedem), arbor intacta.
#
# Usus: ./tools/psalterium_infigere.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
FONS=musica/psalmi
RADIX="${FABRICA_SCRIPTURA:-.}"
EXITUS="$RADIX/lib/psalterium_infixum.c"

PLAGULAE=()
for n in $(seq 1 150); do
    PLAGULAE+=("$FONS/$n.txt")
done
PLAGULAE+=("$FONS/LICENTIA")
for p in "${PLAGULAE[@]}"; do
    [ -s "$p" ] || { echo "psalterium_infigere: $p deest aut vacua" >&2; exit 2; }
done
mkdir -p "$RADIX/lib" || exit 2
{
    printf '/* GENERATUM a tools/psalterium_infigere.sh ex musica/psalmi/ - noli manu mutare */\n'
    printf '#include "psalterium_infixum.h"\n\n#include <string.h>\n\n'
    printf '/* initia: psalmi I-CL, deinde licentia */\n'
    printf 'interior constans i32 _initia[] = {\n'
    initium=0
    for p in "${PLAGULAE[@]}"; do
        printf '    %d,\n' "$initium"
        initium=$((initium + $(wc -c < "$p")))
    done
    printf '};\n\n'
    printf 'interior constans insignatus character _octeti[] = {\n'
    cat "${PLAGULAE[@]}" | od -An -v -tx1 \
        | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
    printf '};\n\n'
    printf 'interior chorda\n_copia (\n         i32  initium,\n         i32  finis,\n     Piscina* piscina)\n{\n'
    printf '    chorda c;\n\n'
    printf '    c.mensura  = finis - initium;\n'
    printf '    c.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura);\n'
    printf '    si (c.datum == NIHIL)\n    {\n        c.mensura = 0;\n        redde c;\n    }\n'
    printf '    memcpy(c.datum, _octeti + initium, (size_t)c.mensura);\n'
    printf '    redde c;\n}\n\n'
    printf 'chorda\npsalterium_infixum_psalmus (\n         i32  numerus,\n     Piscina* piscina)\n{\n'
    printf '    chorda c;\n\n'
    printf '    si (numerus < 1 || numerus > 150)\n    {\n'
    printf '        c.datum    = NIHIL;\n        c.mensura  = 0;\n        redde c;\n    }\n'
    printf '    redde _copia(_initia[numerus - 1], _initia[numerus], piscina);\n}\n\n'
    printf 'chorda\npsalterium_infixum_licentia (\n    Piscina* piscina)\n{\n'
    printf '    redde _copia(_initia[150], (i32)magnitudo(_octeti), piscina);\n}\n'
} > "$EXITUS.tmp" && mv "$EXITUS.tmp" "$EXITUS"
