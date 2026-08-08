#!/bin/sh
# census_orbi.sh - orbos in censu invenire (disciplina KAOS)
#
# Res ORBA est nisi: (a) a causat citata, (b) causa cum pendet-ex
# (spinae ancorata), aut (c) 'PARENTELA EXTRA DOCUMENTUM' in nota
# fert (exemptio DECLARATA - radix, personae).
#
# Usus: ./tools/census_orbi.sh <via.census>
# Exitus: 0 = nihil orbum; 1 = orbi nominati; 2 = NIHIL lectum
# (plagula vacua aut sine rebus - defectus portae, non salus).
# NB: unum ad="#..." per lineam legitur (stilus domus).

si_via="$1"
if [ -z "$si_via" ] || [ ! -f "$si_via" ]; then
    echo "census_orbi: plagula deest: $si_via" >&2
    exit 2
fi

awk '
    /nomen="#/ {
        if (match($0, /nomen="#[^"]*"/)) {
            n = substr($0, RSTART + 8, RLENGTH - 9)
            declarata[n] = NR
            praesens = n
        }
    }
    /<causat ad="#/ || /causat="#/ {
        if (match($0, /causat( ad)?="#[^"]*"/)) {
            s = substr($0, RSTART, RLENGTH)
            sub(/^causat( ad)?="#/, "", s)
            sub(/"$/, "", s)
            iustificata[s] = 1
        }
    }
    /<pendet-ex ad="#/ || /pendet-ex="#/ {
        if (praesens != "") ancorata[praesens] = 1
    }
    /PARENTELA EXTRA DOCUMENTUM/ {
        if (praesens != "") exempta[praesens] = 1
    }
    END {
        quot = 0; orbae = 0
        for (n in declarata) quot++
        if (quot == 0) exit 2
        for (n in declarata) {
            if (!(n in iustificata) && !(n in ancorata) \
                && !(n in exempta)) {
                printf "ORBUM: #%s (linea %d)\n", n, declarata[n]
                orbae++
            }
        }
        ni = 0; na = 0; ne = 0
        for (n in iustificata) if (n in declarata) ni++
        for (n in ancorata) na++
        for (n in exempta) ne++
        printf "census_orbi: res %d / iustificatae %d / ancoratae %d / exemptae %d / ORBAE %d\n", \
            quot, ni, na, ne, orbae
        exit (orbae > 0 ? 1 : 0)
    }
' "$si_via"
