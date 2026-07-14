#!/bin/bash

# excubitor.sh - EXCUBITOR: custos staleness aedificationum
#
# Usage:
#   ./excubitor.sh             # omnia obiecta + res fabricae
#   ./excubitor.sh <filtrum>   # finalis '/' = directe in directorio
#                              # (officina/build/ sine subpiscinis);
#                              # '/' medius = prooemium profundum;
#                              # sine '/' = substring (ubique)
#   ./excubitor.sh -omnia      # sine tecto enumerationis (5/gregem)
#   ./excubitor.sh -tacitus    # NIHIL nisi stala (canalis
#                              # post-constructionem in scriptis)
#
# Exit: 0 PURUS | 1 STALA inventa | 2 graphus deest / usus
#
# VERIFICAT ET EXPLICAT SOLUM - scripta constructionis executores
# manent. Graphus = build/inclusiones.tsv (percursus fundit;
# ./silva/nexus.sh -renovare) + fabrica.tsv (regulae manuales:
# praefixa obiectorum, generata, binaria). Conventio: <basis>.o <-
# <basis>.c (.m) inventa per corpus; principalia/ numquam obiecta
# dant (mains dev-temporis directe in binaria compilantur).
#
# CAVEATA NOTA (phase-log: FUTURUM NOMINATUM EXCUBITOR): mtime non
# semantica (mutatio commentarii = eadem vox); binaria solum per
# regulas fabricae sequuntur donec aedilis manifesta verorum
# nexuum ferat.

set -u

RADIX_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$RADIX_DIR"

GRAPHUS="build/inclusiones.tsv"
FABRICA="fabrica.tsv"
FILTRUM=""
OMNIA=0
TACITUS=0

for a in "$@"; do
    case "$a" in
        -omnia) OMNIA=1 ;;
        -tacitus) TACITUS=1 ;;
        -*) echo "excubitor: flagrum ignotum $a" >&2 ; exit 2 ;;
        *)  FILTRUM="$a" ;;
    esac
done

if [ ! -f "$GRAPHUS" ]; then
    echo "EXCUBITOR: $GRAPHUS deest" >&2
    echo "  (genera: ./silva/nexus.sh -renovare)" >&2
    exit 2
fi

# --------------------------------------------------
# flumen mtime: "TAG mtempus via" (viae sine spatiis - lex domus)
#   M = plagula (fons/caput/res); O = obiectum inspiciendum;
#   G = graphus ipse
# --------------------------------------------------
FLUMEN="$(mktemp)"
trap 'rm -f "$FLUMEN"' EXIT

{
    # fontes et capita (extra build/ et fixa/ - plagulae fixae
    # nomina fontium verorum communicant sed inputus constructionis
    # numquam sunt; ambiguitas falsa sine hac exclusione)
    find . \( -name build -o -name .git -o -name node_modules \
            -o -name results -o -name censoris -o -name fixa \) \
        -prune -o \
        \( -name '*.c' -o -name '*.h' -o -name '*.m' \) -print0 \
        2>/dev/null | xargs -0 stat -f 'M %m %N' 2>/dev/null

    # obiecta sub omnibus directoriis build/
    find . -type d -name build -not -path '*/node_modules/*' \
        2>/dev/null | while read -r d; do
        find "$d" -name '*.o' -print0 2>/dev/null \
            | xargs -0 stat -f 'O %m %N' 2>/dev/null
    done

    # res fabricae exactae (generata/binaria: res + fontes plagulae)
    awk -F'\t' '!/^#/ && ($1=="generatum" || $1=="binarium") {
        print $2; if ($3 !~ /\/$/) print $3
    }' "$FABRICA" 2>/dev/null | sort -u | while read -r p; do
        stat -f 'M %m %N' "$p" 2>/dev/null
    done

    # directoria fabricae: plagulae omnes sub eis
    awk -F'\t' '!/^#/ && ($1=="generatum" || $1=="binarium") \
        && $3 ~ /\/$/ { print $3 }' "$FABRICA" 2>/dev/null \
        | sort -u | while read -r d; do
        find "$d" -type f -print0 2>/dev/null \
            | xargs -0 stat -f 'M %m %N' 2>/dev/null
    done

    stat -f 'G %m %N' "$GRAPHUS" 2>/dev/null
} > "$FLUMEN"

# --------------------------------------------------
# iudicium: graphus + fabrica + flumen in uno awk
# --------------------------------------------------
awk -F'\t' -v graphus="$GRAPHUS" -v fabrica="$FABRICA" \
    -v filtrum="$FILTRUM" -v omnia="$OMNIA" -v tacitus="$TACITUS" '

# ---- graphus inclusionum: margines ex -> ad (status neglectus:
#      margo praetermissa quoque dependentia textualis est) ----
FILENAME == graphus {
    if ($0 ~ /^#/ || NF < 2) next
    clavis = $1 SUBSEP $2
    if (!(clavis in margo_visa)) {
        margo_visa[clavis] = 1
        margines[$1] = ($1 in margines) \
            ? margines[$1] "\n" $2 : $2
    }
    next
}

# ---- fabrica: regulae manuales ----
FILENAME == fabrica {
    if ($0 ~ /^#/ || NF < 2) next
    if ($1 == "praefixum") {
        praefixa[++n_praefixa] = $2
    } else if ($1 == "obiectum") {
        regula_obiecti[$2] = $3
    } else if ($1 == "generatum" || $1 == "binarium") {
        n_res++
        res_via[n_res] = $2
        res_fons[n_res] = $3
        res_species[n_res] = $1
    }
    next
}

# ---- flumen mtime (spatiis separatum) ----
{
    split($0, campi, " ")
    via = campi[3]
    sub(/^\.\//, "", via)
    mtempus[via] = campi[2] + 0
    if (campi[1] == "O") {
        obiecta[++n_obiecta] = via
    } else if (campi[1] == "G") {
        graphus_t = campi[2] + 0
    } else {
        # index fontium: basis -> viae (pro resolutione obiectorum);
        # dedup - res fabricae iam in percursione statae iterum
        # adveniunt (ambiguitas falsa sine hac)
        if (via ~ /\.(c|m)$/ && !(via in fons_visa)) {
            fons_visa[via] = 1
            basis = via
            sub(/^.*\//, "", basis)
            sub(/\.(c|m)$/, "", basis)
            fontes_basis[basis] = (basis in fontes_basis) \
                ? fontes_basis[basis] "\n" via : via
        }
        # recentissima plagula fontium (pro aetate graphi)
        if (via ~ /\.[ch]$/ && campi[2] + 0 > fons_recentissimus_t) {
            fons_recentissimus_t = campi[2] + 0
            fons_recentissimus_via = via
        }
    }
    next
}

# ---- functiones ----

# clausura inclusionum: mtempus recentissimum trans graphum a fonte;
# RES_T/RES_VIA/RES_CATENA globales (awk unum valorem reddit)
function clausura_recentissima(fons,    pila, np, visum, nodus, \
    filii, nf, k, filius, catena) {
    delete visum
    np = 1
    pila[1] = fons
    catena[fons] = fons
    visum[fons] = 1
    RES_T = -1 ; RES_VIA = "" ; RES_CATENA = ""
    while (np > 0) {
        nodus = pila[np] ; np--
        if (nodus in mtempus && mtempus[nodus] > RES_T) {
            RES_T = mtempus[nodus]
            RES_VIA = nodus
            RES_CATENA = catena[nodus]
        }
        if (nodus in margines) {
            nf = split(margines[nodus], filii, "\n")
            for (k = 1; k <= nf; k++) {
                filius = filii[k]
                if (!(filius in visum)) {
                    visum[filius] = 1
                    # externa (sine /) sine mtempore transeunt
                    pila[++np] = filius
                    catena[filius] = filius " <- " catena[nodus]
                }
            }
        }
    }
    delete catena
}

# fontem obiecti resolvere: regula exacta -> praefixum -> conventio.
# principalia/ excluditur (mains directe in binaria). Ambiguitas:
# praelatio prooemii communis (officina/build/x.o -> officina/...);
# aliter OMNES candidati (conservativum). Reddit "via1\nvia2..." aut "".
function fontem_resolvere(obiectum,    basis, k, cand, nc, viae, \
    lista, m, prooemium, praelati, np2, omnes, no) {
    if (obiectum in regula_obiecti) {
        return regula_obiecti[obiectum]   # regula via plena
    }
    basis = obiectum
    sub(/^.*\//, "", basis)
    if (basis in regula_obiecti) {
        return regula_obiecti[basis]
    }
    sub(/\.o$/, "", basis)
    cand = ""
    if (basis in fontes_basis) cand = fontes_basis[basis]
    if (cand == "") {
        for (k = 1; k <= n_praefixa; k++) {
            if (index(basis, praefixa[k]) == 1) {
                m = substr(basis, length(praefixa[k]) + 1)
                if (m in fontes_basis) { cand = fontes_basis[m]; break }
            }
        }
    }
    if (cand == "") return ""
    nc = split(cand, viae, "\n")
    # principalia numquam obiecta dant
    omnes = "" ; no = 0
    for (k = 1; k <= nc; k++) {
        if (viae[k] !~ /\/principalia\//) {
            omnes = (no++) ? omnes "\n" viae[k] : viae[k]
        }
    }
    if (no == 0) return ""
    if (no == 1) return omnes
    # praelatio prooemii (pars prima viae obiecti; build radicis -> lib)
    prooemium = obiectum
    sub(/\/.*$/, "", prooemium)
    if (prooemium == "build") prooemium = "lib"
    nc = split(omnes, viae, "\n")
    praelati = "" ; np2 = 0
    for (k = 1; k <= nc; k++) {
        if (index(viae[k], prooemium "/") == 1) {
            praelati = (np2++) ? praelati "\n" viae[k] : viae[k]
        }
    }
    if (np2 >= 1) return praelati
    ambigua++
    return omnes
}

# filtrum tres gradus: finalis "/" = DIRECTE in directorio (canales
# scriptorum - piscinae subiectae alienis scriptis exclusae);
# "/" medius = prooemium profundum; sine "/" = substring
function congruit(via,    reliquum) {
    if (filtrum == "") return 1
    if (filtrum ~ /\/$/) {
        if (index(via, filtrum) != 1) return 0
        reliquum = substr(via, length(filtrum) + 1)
        return index(reliquum, "/") == 0
    }
    if (index(filtrum, "/") > 0) return index(via, filtrum) == 1
    return index(via, filtrum) > 0
}

function nuntiare(grex, linea) {
    grex_n[grex]++
    if (grex_n[grex] <= 5 || omnia) {
        relatio = relatio "\n" linea
    } else if (grex_n[grex] == 6) {
        relatio = relatio "\n  ... (et alia; -omnia enumerat)"
    }
}

END {
    stala = 0 ; inspecta = 0 ; orphana = 0 ; orphana_lista = ""

    # ---- obiecta contra clausuras fontium ----
    for (i = 1; i <= n_obiecta; i++) {
        ob = obiecta[i]
        if (!congruit(ob)) continue
        fontes_ob = fontem_resolvere(ob)
        if (fontes_ob == "") {
            orphana++
            orphana_lista = orphana_lista "\n  ORPHANUM " ob
            continue
        }
        inspecta++
        nf2 = split(fontes_ob, ff, "\n")
        pessimus_t = -1
        for (j = 1; j <= nf2; j++) {
            clausura_recentissima(ff[j])
            if (RES_T > pessimus_t) {
                pessimus_t = RES_T
                pessimus_via = RES_VIA
                pessima_catena = RES_CATENA
            }
        }
        if (pessimus_t > mtempus[ob]) {
            stala++
            grex = ob ; sub(/\/[^\/]*$/, "", grex)
            nuntiare(grex, "  STALUM " ob \
                "\n    POST " pessimus_via \
                (pessima_catena == pessimus_via ? "" : \
                    "\n    (" pessima_catena ")"))
        }
    }

    # ---- res fabricae: generata et binaria ----
    for (i = 1; i <= n_res; i++) {
        rv = res_via[i]
        if (!congruit(rv)) continue
        if (!(rv in mtempus)) continue   # nondum aedificata
        inspecta++
        rf = res_fons[i]
        pessimus_t = -1 ; pessimus_via = "" ; pessima_catena = ""
        if (rf ~ /\/$/) {
            # directorium: plagula recentissima sub eo (res ipsa exclusa)
            for (via in mtempus) {
                if (index(via, rf) == 1 && via != rv \
                    && mtempus[via] > pessimus_t) {
                    pessimus_t = mtempus[via]
                    pessimus_via = via
                }
            }
            pessima_catena = pessimus_via
        } else if (rf ~ /\.[chm]$/) {
            clausura_recentissima(rf)
            pessimus_t = RES_T
            pessimus_via = RES_VIA
            pessima_catena = RES_CATENA
        } else if (rf in mtempus) {
            pessimus_t = mtempus[rf]
            pessimus_via = rf
            pessima_catena = rf
        }
        if (pessimus_t > mtempus[rv]) {
            stala++
            nuntiare(res_species[i], "  STALUM [" res_species[i] "] " rv \
                "\n    POST " pessimus_via \
                (pessima_catena == pessimus_via ? "" : \
                    "\n    (" pessima_catena ")"))
        }
    }

    # ---- relatio ----
    # -tacitus: PURUS = silentium totale (disciplina uncorum;
    # etiam CAUTIO aetatis tacet - inter commissiones graphus
    # semper paulum vetus, uncus post-commissionem renovat)
    if (tacitus && stala == 0) exit 0
    print "=== EXCUBITOR ==="
    if (graphus_t < fons_recentissimus_t) {
        print "CAUTIO: graphus vetustior quam " fons_recentissimus_via
        print "        (renovare: ./silva/nexus.sh -renovare)"
    }
    if (relatio != "") print substr(relatio, 2)
    if (orphana > 0 && omnia) print substr(orphana_lista, 2)
    printf "inspecta: %d", inspecta
    if (orphana > 0) printf "; orphana: %d (obiecta sine fonte%s)", \
        orphana, (omnia ? "" : "; -omnia enumerat")
    if (ambigua > 0) printf "; ambigua: %d (omnes candidati inspecti)", \
        ambigua
    print ""
    if (stala > 0) {
        print "VERDICTUM: STALA " stala
        exit 1
    }
    print "VERDICTUM: PURUS"
    exit 0
}
' "$GRAPHUS" "$FABRICA" "$FLUMEN"
