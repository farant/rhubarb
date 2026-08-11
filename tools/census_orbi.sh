#!/bin/sh
# census_orbi.sh - orbos in censu invenire (disciplina KAOS)
#
# REGULA: nihil natat. Quaeque res per catenam citationum
# QUAMLIBET ad radicem declaratam pertingere debet; margines
# NON DIRIGUNTUR - res quae aliam citat ei coniungitur, et
# quae ab alia citatur pariter.
#
# CUR NON 'a fine iustificata': disciplina KAOS omnia MEDIA
# praesumit, sed census plura fert - PARTICIPES (qui fini non
# serviunt sed eum persequuntur), PRAEDICATA (signum de re,
# periculum rei minans), SODALES (praeceptum artis suae).
# Indicem nominum marginum crescentem tenere = portam
# lente dilatare donec nihil vetet. Pertinentia sola regula.
#
# RADIX = res 'extra-ordinem="verum"' aut cum 'PARENTELA EXTRA
# DOCUMENTUM' in nota: TERMINUS DECLARATUS, non lacuna.
#
# Usus: ./tools/census_orbi.sh <via.census>
# Exitus: 0 = nihil natat; 1 = orbi nominati; 2 = NIHIL lectum
# (defectus portae, non salus).

si_via="$1"
if [ -z "$si_via" ] || [ ! -f "$si_via" ]; then
    echo "census_orbi: plagula deest: $si_via" >&2
    exit 2
fi

# claves externae librarii (W1): nomina in seminibus declarata
# CITARI possunt sine declaratione locali - custos olim caecus
# numerum nudum clamabat (quaestio 01KZRS606V)
semina="natura/cocta/semina.census"
[ -f "$semina" ] || semina="/dev/null"

awk '
    FNR == NR {
        if (match($0, /nomen="&[A-Za-z0-9_-]+;"/)) {
            externa[substr($0, RSTART + 8, RLENGTH - 10)] = 1
        }
        next
    }
    {
        linea = $0

        # signum "&nomen;" (decretum 2026-08-10; olim "#nomen")
        if (match(linea, /nomen="&[A-Za-z0-9_-]+;"/)) {
            praesens = substr(linea, RSTART + 8, RLENGTH - 10)
            declarata[praesens] = NR
        }

        reliquum = linea
        while (match(reliquum, /"&[A-Za-z0-9_-]+;"/)) {
            citatum = substr(reliquum, RSTART + 2, RLENGTH - 4)
            if (praesens != "" && citatum != praesens) {
                vicini[praesens] = vicini[praesens] " " citatum
                vicini[citatum] = vicini[citatum] " " praesens
            }
            reliquum = substr(reliquum, RSTART + RLENGTH)
        }

        if (linea ~ /extra-ordinem="verum"/ ||
            linea ~ /PARENTELA EXTRA DOCUMENTUM/) {
            if (praesens != "") semina[praesens] = 1
        }
    }
    END {
        quot = 0
        for (n in declarata) quot++
        if (quot == 0) exit 2

        quot_seminum = 0
        for (n in semina) {
            if (n in declarata) {
                agmen[++cauda] = n
                tactum[n] = 1
                quot_seminum++
            }
        }
        if (quot_seminum == 0) {
            print "census_orbi: RADIX NULLA declarata - " \
                  "adde extra-ordinem=\"verum\"" > "/dev/stderr"
            exit 2
        }

        caput = 0
        while (caput < cauda) {
            hic = agmen[++caput]
            quot_vicinorum = split(vicini[hic], v, " ")
            for (i = 1; i <= quot_vicinorum; i++) {
                if (v[i] != "" && !(v[i] in tactum)) {
                    tactum[v[i]] = 1
                    agmen[++cauda] = v[i]
                }
            }
        }

        orbae = 0
        for (n in declarata) {
            if (!(n in tactum)) {
                printf "ORBUM: &%s; (linea %d)\n", n, declarata[n]
                orbae++
            }
        }

        externae_citatae = 0
        ignota = 0
        for (n in vicini) {
            if (n in declarata) continue
            if (n in externa) { externae_citatae++; continue }
            printf "CAVE: citatio ad nomen IGNOTUM: &%s;\n", n
            ignota++
        }

        printf "census_orbi: res %d / radices %d / pertingentes %d / ORBAE %d", \
            quot, quot_seminum, quot - orbae, orbae
        if (externae_citatae > 0)
            printf " / externae %d", externae_citatae
        printf "\n"
        exit (orbae > 0 || ignota > 0 ? 1 : 0)
    }
' "$semina" "$si_via"
