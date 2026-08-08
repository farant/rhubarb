#!/bin/sh
# census_orbi.sh - orbos in censu invenire (disciplina KAOS)
#
# Res ORBA est nisi: (a) a causat citata, (b) causa cum pendet-ex
# (spinae ancorata), (c) PRAEDICATA - de= aut minatur= ad rem
# aliam ferens, aut (d) exempta DECLARATE (extra-ordinem="verum"
# in causa, aut 'PARENTELA EXTRA DOCUMENTUM' in nota).
#
# CUR (c): disciplina KAOS omnia MEDIA praesumit, sed census
# etiam PRAEDICATA fert - signum de re, periculum rei minans.
# Sententia de re ancorata orba non est: subiecto suo pendet.
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
    /<de ad="#/ || /[ \t]de="#/ || /minatur="#/ || /<minatur ad="#/ {
        if (praesens != "") praedicata[praesens] = 1
    }
    /PARENTELA EXTRA DOCUMENTUM/ || /extra-ordinem="verum"/ {
        if (praesens != "") exempta[praesens] = 1
    }
    END {
        quot = 0; orbae = 0
        for (n in declarata) quot++
        if (quot == 0) exit 2
        for (n in declarata) {
            if (!(n in iustificata) && !(n in ancorata) \
                && !(n in praedicata) && !(n in exempta)) {
                printf "ORBUM: #%s (linea %d)\n", n, declarata[n]
                orbae++
            }
        }
        ni = 0; na = 0; np = 0; ne = 0
        for (n in iustificata) if (n in declarata) ni++
        for (n in ancorata) na++
        for (n in praedicata) np++
        for (n in exempta) ne++
        printf "census_orbi: res %d / iustificatae %d / ancoratae %d / praedicata %d / exemptae %d / ORBAE %d\n", \
            quot, ni, na, np, ne, orbae
        exit (orbae > 0 ? 1 : 0)
    }
' "$si_via"
