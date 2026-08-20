#!/bin/bash

# tools/amalgama_excludenda_generare.sh - messis excludendorum
#
# AMALGAMA gradus 2 scalae: excludenda (functiones vendicatae
# inusitatae quae ex amalgamate cadunt) COMPILATORE metuntur, non
# manu recordantur. Ansa ad punctum fixum: listae vacuae -> amalgama
# generatum -> compilatio SINE -Werror -> admonitiones
# unused-function -> nomina (renominatione reversa: praefixum
# "<proiectum>_" demptum) -> classificatio per basem (praefixum
# longissimum; residua per plagulam definitionis "^nomen (") ->
# listae auctae -> iterum, donec admonitio nulla.
#
# Fructus: excludenda_generata.h iuxta manifestum (macra eadem
# EXCLUDENDA_* - politica intacta). Custodia: vectis proiecti
# (amalgamare.sh) post messem currendus - amalgama byte-identicum
# si listae manuales priores punctum fixum iam erant.
#
# Usus: ./tools/amalgama_excludenda_generare.sh <proiectum>

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

si_fracta() { echo "amalgama_excludenda_generare: $1" >&2; exit 1; }

# -probare: derivare et CONFERRE, nihil scribere (porta vetustatis).
# Exitus: 0 = recens, 1 = rancidum (regeneratio debetur).
PROBARE=0
PROIECTUM=""
for arg in "$@"; do
    case "$arg" in
        -probare) PROBARE=1 ;;
        -*)       si_fracta "vexillum ignotum: $arg" ;;
        *)
            [ -z "$PROIECTUM" ] || si_fracta "proiectum bis datum"
            PROIECTUM="$arg"
            ;;
    esac
done
[ -n "$PROIECTUM" ] \
    || si_fracta "usus: amalgama_excludenda_generare.sh [-probare] <proiectum>"
SEDES="$PROIECTUM/instrumenta/principalia"
POLITICA="$SEDES/fontes_politica.sh"
EXITUS="$SEDES/excludenda_generata.h"
[ -f "$POLITICA" ] || si_fracta "politica deest: $POLITICA"
# shellcheck source=/dev/null
. "$POLITICA"
command -v bases_excludendae >/dev/null 2>&1 \
    || si_fracta "bases_excludendae deest in politica"

BASES="$(bases_excludendae)"
PRAEFIXUM="${PROIECTUM}_"

# functiones PROTECTAE (non_statica manifesti = superficies publica):
# demissio earum = catena viva callee amisit - error verus, numquam
# tacite excludendus (lectio silvae 2026-07-21: signum B per catenam
# VIVAM ascendit et stml_legere ipsum devoravit)
PROTECTA="$(awk '/NON_STATICA\[\]/,/};/' "$SEDES/amalgamator.c" \
    | grep -o '"[A-Za-z_][A-Za-z0-9_]*"' | tr -d '"')"

declare -a VEXILLA_SINE_WERROR=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
declare -a VEXILLA_PLENA=("${VEXILLA_SINE_WERROR[@]}" "-Werror")

STATIO="build/aedilis/excludenda/$PROIECTUM"
rm -rf "$STATIO"
mkdir -p "$STATIO"

# ------------------------------------------------------------------
# CUSTODIA EXITUS: _caput_emittere in EXITUM ipsum scribit, et ante
# ansam semel (listae VACUAE), quia amalgamator singulis gyris cum
# listis novis recompilatur - ergo scriptio in plagulam veram opus
# mechanismi est, non effectus finalis.
#
# Consequentia mensurata 2026-08-20: cursus qui gyro PRIMO frangitur
# (amalgamator strui non potuit) EXITUM vacuum post se relinquebat -
# CLIII nomina deleta, plagula commissa corrupta, nullo verbo dicto.
# Ergo: exemplar ante omnia servatur et in fractura REDDITUR.
# In modo -probare idem mechanismum praebet: cursus scribit, deinde
# comparamus, deinde exemplar semper redditur.
# ------------------------------------------------------------------
EXEMPLAR="$STATIO/exitus_exemplar"
EXITUS_REDDENDUS=0

[ -f "$EXITUS" ] && cp "$EXITUS" "$EXEMPLAR"

_exitum_reddere() {
    if [ "$EXITUS_REDDENDUS" -eq 1 ] && [ -f "$EXEMPLAR" ]; then
        cp "$EXEMPLAR" "$EXITUS"
    fi
}
trap _exitum_reddere EXIT

# fractura quaelibet ante finem = exemplar redditur
[ -f "$EXEMPLAR" ] && EXITUS_REDDENDUS=1

# ------------------------------------------------------------------
# recipe constructionis amalgamatoris per proiectum (obiecta
# dependentiarum a vecte proiecti calefacta - vectis prius currendus)
# ------------------------------------------------------------------
_amalgamatorem_struere() {
    case "$PROIECTUM" in
        silva)
            # NB haec recipe GEMINA est eius in silva/amalgamare.sh -
            # ambae amalgamatorem struunt, neutra alteram consulit.
            # Divergerunt semel iam: silva_unitates.c in amalgamare.sh
            # (fed3e78) additum est, hic NUMQUAM - unde hic scriptor
            # pro silva omnino frangebatur, et nemo id sciebat quia
            # nemo eum curreret. Porta vetustatis (gradus 0
            # amalgamandi) hanc classem in posterum capit; recipe
            # communicanda manet OSTIUM nominatum.
            clang "${VEXILLA_PLENA[@]}" -Iinclude -Isilva/fontes \
                -Isilva/instrumenta \
                silva/instrumenta/principalia/amalgamator.c \
                silva/instrumenta/silva_amalgama.c \
                silva/instrumenta/silva_unitates.c \
                silva/build/piscina.o silva/build/chorda.o \
                silva/build/chorda_aedificator.o silva/build/xar.o \
                silva/build/tabula_dispersa.o silva/build/friatio.o \
                silva/build/silva_token.o silva/build/silva_lexema.o \
                -o "$STATIO/amalgamator"
            ;;
        tessera|officina)
            clang "${VEXILLA_PLENA[@]}" -Iinclude -Isilva/fontes \
                -Isilva/instrumenta \
                "$SEDES/amalgamator.c" \
                "$PROIECTUM"/build/mech_*.o \
                -o "$STATIO/amalgamator"
            ;;
        *) si_fracta "proiectum ignotum: $PROIECTUM" ;;
    esac
}

_amalgama_compilare_admonens() {
    local inclusa_extra=""
    [ "$PROIECTUM" = "officina" ] && inclusa_extra="-Isilva/amalgama"
    # shellcheck disable=SC2086
    clang "${VEXILLA_SINE_WERROR[@]}" $inclusa_extra \
        -c "$STATIO/amalgama_messis.c" -o "$STATIO/amalgama_messis.o" \
        2> "$STATIO/admonitiones.txt"
}

# ------------------------------------------------------------------
# classificatio nominis originalis -> basis. PLAGULA DEFINITIONIS
# PRIMA (auctoritas - functiones alieno praefixo exsistunt:
# chorda_internare* in lib/internamentum.c vivunt; praefixum-primum
# eas in EXCLUDENDA falsae plagulae ponebat, amalgamator numquam
# demittebat, punctum fixum numquam attingebatur - lectio silvae
# 2026-07-21); praefixum longissimum = reservum.
# ------------------------------------------------------------------
_classificare() {
    local nomen="$1" b optimum="" optimum_mensura=0
    for b in $BASES; do
        if grep -q "^${nomen}[ ]*(" "lib/$b.c" 2>/dev/null; then
            echo "$b"; return 0
        fi
    done
    for b in $BASES; do
        case "$nomen" in
            "${b}_"*)
                if [ ${#b} -gt $optimum_mensura ]; then
                    optimum="$b"; optimum_mensura=${#b}
                fi ;;
        esac
    done
    if [ -n "$optimum" ]; then echo "$optimum"; return 0; fi
    return 1
}

# ------------------------------------------------------------------
# emissio capitis (listae in $STATIO/lista_<basis>.txt)
# ------------------------------------------------------------------
_caput_emittere() {
    {
        echo "/* excludenda_generata.h - GENERATUM (messis compilatoris) - NE MANU EDITES"
        echo " *"
        echo " * Functiones vendicatae inusitatae ($PROIECTUM): punctum fixum"
        echo " * admonitionum unused-function sub severitate plena. Functio nova"
        echo " * consumpta = regenerare (introitus sponte cadit); ne manu dele."
        echo " * Regeneratio: ./tools/amalgama_excludenda_generare.sh $PROIECTUM"
        echo " */"
        local b macro
        for b in $BASES; do
            macro="$(excludenda_pro "$b")"
            [ "$macro" != "NIHIL" ] || si_fracta "basis sine macro: $b"
            echo ""
            echo "interior constans character* constans ${macro}[] = {"
            if [ -s "$STATIO/lista_$b.txt" ]; then
                LC_ALL=C sort -u "$STATIO/lista_$b.txt" \
                    | sed 's/^/    "/; s/$/",/'
            fi
            echo "    NIHIL"
            echo "};"
        done
    } > "$EXITUS"
}

# ------------------------------------------------------------------
# ansa messis
# ------------------------------------------------------------------
for b in $BASES; do : > "$STATIO/lista_$b.txt"; done
_caput_emittere

GYRUS=0
dum_finis=0
while [ $dum_finis -eq 0 ]; do
    GYRUS=$((GYRUS + 1))
    # ansa monotona (listae solum crescunt, numero functionum
    # limitatae) - tectum = obex fugae solum. X -> XXX 2026-07-21:
    # silva tres bases novas vendicavit, cascata inusitatorum per
    # strata DAG vocationum decorticatur (gyrus unus = stratum unum;
    # 123 nomina gyro IX legitima erant).
    [ $GYRUS -le 30 ] || si_fracta "punctum fixum non attactum (gyri XXX)"

    _amalgamatorem_struere || si_fracta "constructio amalgamatoris (gyrus $GYRUS)"
    "$STATIO/amalgamator" "$PWD" "$STATIO/amalgama_messis.c" \
        > /dev/null || si_fracta "cursus amalgamatoris (gyrus $GYRUS)"
    COMPILATIO_BONA=1
    _amalgama_compilare_admonens || COMPILATIO_BONA=0
    cp "$STATIO/admonitiones.txt" "$STATIO/admonitiones_$GYRUS.txt"
    cp "$STATIO/amalgama_messis.c" "$STATIO/amalgama_messis_$GYRUS.c"

    # signum A: functiones inusitatae (admonitio) -> excludendae.
    # Classis altera (2026-07-21): statica se-recursiva sub vocatore
    # excluso = "not needed and will not be emitted" (unneeded-
    # internal-declaration), quam "unused function" non tegit.
    { grep -o "unused function '[^']*'" "$STATIO/admonitiones.txt" \
        | sed "s/unused function '//; s/'//"
      grep -o "function '[^']*' is not needed and will not be emitted" \
        "$STATIO/admonitiones.txt" \
        | sed "s/function '//; s/' is not needed.*//"
    } | sort -u > "$STATIO/inusitatae.txt"

    # signum B: declaratio implicita (error) = vocatio functionis a
    # servanda demptae -> VOCANS ipse mortuus-in-contextu; functio
    # continens per definitionem columnae 0 proxime priorem
    grep "error: implicit declaration of function" \
        "$STATIO/admonitiones.txt" \
        | sed "s/^[^:]*:\([0-9]*\):.*/\1/" | sort -un \
        > "$STATIO/lineae_implicitae.txt"
    while IFS= read -r linea; do
        [ -n "$linea" ] || continue
        awk -v l="$linea" '
            NR > l { exit }
            /^[A-Za-z_][A-Za-z0-9_]*[ ]*\(/ {
                f = $0; sub(/[ ]*\(.*/, "", f); functio = f }
            END { if (functio != "") print functio }
        ' "$STATIO/amalgama_messis.c" >> "$STATIO/inusitatae.txt"
    done < "$STATIO/lineae_implicitae.txt"
    sort -u -o "$STATIO/inusitatae.txt" "$STATIO/inusitatae.txt"

    NOVAE=0
    while IFS= read -r renominata; do
        [ -n "$renominata" ] || continue
        nomen="${renominata#"$PRAEFIXUM"}"
        case " $(echo $PROTECTA) " in
            *" $nomen "*)
                si_fracta "signum functionem PROTECTAM demittere vult: $nomen (gyrus $GYRUS) - catena viva callee amisit; vide $STATIO/admonitiones.txt"
                ;;
        esac
        basis="$(_classificare "$nomen")" \
            || si_fracta "nomen inclassificabile: $nomen (renominata: $renominata)"
        if ! grep -Fxq "$nomen" "$STATIO/lista_$basis.txt"; then
            echo "$nomen" >> "$STATIO/lista_$basis.txt"
            NOVAE=$((NOVAE + 1))
        fi
    done < "$STATIO/inusitatae.txt"

    if [ "$COMPILATIO_BONA" -eq 0 ] && [ "$NOVAE" -eq 0 ]; then
        echo "amalgama_excludenda_generare: compilatio fracta sine signis:" >&2
        grep "error:" "$STATIO/admonitiones.txt" | head -8 >&2
        exit 1
    fi

    echo "  gyrus $GYRUS: $NOVAE nomina nova"
    if [ "$NOVAE" -eq 0 ]; then
        dum_finis=1
    else
        _caput_emittere
    fi
done

SUMMA=0
for b in $BASES; do
    n=$(wc -l < "$STATIO/lista_$b.txt" | tr -d ' ')
    SUMMA=$((SUMMA + n))
done

if [ "$PROBARE" -eq 1 ]; then
    # exemplar per laqueum EXIT redditur - hic solum iudicamus
    if [ ! -f "$EXEMPLAR" ]; then
        echo "amalgama_excludenda_generare ($PROIECTUM): RANCIDUM - $EXITUS deest"
        exit 1
    fi
    if cmp -s "$EXITUS" "$EXEMPLAR"; then
        echo "amalgama_excludenda_generare ($PROIECTUM): recens ($SUMMA nomina)"
        exit 0
    fi
    echo "amalgama_excludenda_generare ($PROIECTUM): RANCIDUM - $EXITUS derivationi non congruit"
    echo "  regenera: ./tools/amalgama_excludenda_generare.sh $PROIECTUM"
    exit 1
fi

# successus: exitus novus MANET (laqueus eum non reddat)
EXITUS_REDDENDUS=0
echo "amalgama_excludenda_generare ($PROIECTUM): $EXITUS ($SUMMA nomina, $GYRUS gyri)"
echo "  (nunc curre ./$PROIECTUM/amalgamare.sh - portae plenae + byte-identitas)"
