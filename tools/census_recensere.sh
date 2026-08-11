#!/bin/sh
# census_recensere.sh - signa censūs contra vias suas conferre
#
# Quodque signum viam suam fert (methodum qua numerus natus est);
# hoc instrumentum vias CURRIT et fructum cum contento confert.
# Tres exitus per signum:
#   RECENS      - numerus contenti in fructu viae stat
#   MOTUS       - via currit sed numerum alium dicit (drift)
#   VIA MORTUA  - via nihil reddit: methodus ipsa periit
#     (lectio ambulationis: non solum valor putrescit, VIA
#     putrescit - migratio sigillorum viam seminum occidit)
#
# Usus: ./tools/census_recensere.sh [via.census]
# Exitus: 0 = omnia recentia; 1 = motus aut viae mortuae;
#         2 = NIHIL probatum (defectus, non salus)

census="${1:-rhubarb.census}"
radix_repositorii="$(cd "$(dirname "$0")/.." && pwd)"
cd "$radix_repositorii" || exit 2

if [ ! -f "$census" ]; then
    echo "census_recensere: plagula deest: $census" >&2
    exit 2
fi

# pars C aedificanda si stala (analysta stml, non grep)
fons_c="tools/census_recensere.c"
binarium="bin/census_recensere"
if [ ! -x "$binarium" ] || [ "$fons_c" -nt "$binarium" ]; then
    echo "census_recensere: binarium struitur..." >&2
    clausura="$(bin/aedilis "$fons_c" --partes 2>/dev/null |
        awk '$1=="O"{print $2}')"
    if [ -z "$clausura" ]; then
        echo "census_recensere: clausura deest (aedilis)" >&2
        exit 2
    fi
    # shellcheck disable=SC2086
    clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion \
        -Wsign-conversion -Wcast-qual -Wstrict-prototypes \
        -Wmissing-prototypes -Wwrite-strings -Wno-long-long \
        -Wno-overlength-strings -fbracket-depth=512 -Iinclude \
        "$fons_c" $clausura -o "$binarium" || exit 2
fi

tabula="$("$binarium" "$census")" || exit 2

recentia=0
mota=0
mortuae=0

# IFS tab solum: campi tres (titulus, contentum, via)
IFS='	'
printf '%s\n' "$tabula" | while read -r titulus contentum via; do
    [ -z "$titulus" ] && continue

    # entia vulgaria in via decodenda (&amp; ULTIMUM ne bis)
    via_nuda="$(printf '%s' "$via" | sed \
        -e 's/&quot;/"/g' -e 's/&lt;/</g' -e 's/&gt;/>/g' \
        -e 's/&amp;/\&/g')"

    numerus="$(printf '%s' "$contentum" |
        grep -oE '[0-9]+' | tail -1)"

    fructus="$(sh -c "$via_nuda" 2>/dev/null)"
    via_exitus=$?

    # via sana exitu 0 mensurat: exitus alius aut fructus
    # vacuus = methodus ipsa periit (grep sine congruentia '0'
    # imprimit SED exitu 1 - numerus verus specie mortis)
    if [ "$via_exitus" -ne 0 ] || [ -z "$fructus" ]; then
        printf 'VIA MORTUA  %s (exitus %s)\n            via: %s\n            fructus: %s\n' \
            "$titulus" "$via_exitus" "$via_nuda" "${fructus:--}"
        echo "MORTUA" >> /tmp/census_recensere.$$
    elif [ -n "$numerus" ] &&
         printf '%s' "$fructus" | grep -qw "$numerus"; then
        printf 'RECENS      %s (%s)\n' "$titulus" "$contentum"
    else
        printf 'MOTUS       %s\n            contentum: %s\n            via dicit: %s\n' \
            "$titulus" "$contentum" "$fructus"
        echo "MOTUS" >> /tmp/census_recensere.$$
    fi
done

# subcrusta 'while' variabiles perdit - vestigium per plagulam
if [ -f /tmp/census_recensere.$$ ]; then
    mota="$(grep -c MOTUS /tmp/census_recensere.$$)"
    mortuae="$(grep -c MORTUA /tmp/census_recensere.$$)"
    rm -f /tmp/census_recensere.$$
fi
summa="$(printf '%s\n' "$tabula" | grep -c .)"
recentia=$((summa - mota - mortuae))

echo "census_recensere: signa $summa / recentia $recentia / mota $mota / viae mortuae $mortuae"
[ "$summa" -eq 0 ] && exit 2
[ $((mota + mortuae)) -gt 0 ] && exit 1
exit 0
