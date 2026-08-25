#!/bin/bash
# speculum_generare.sh - generator fontium speculi
#
# Colligit copiam recompilationis app (clausura vera AB AEDILE -
# manifestum artificium veritatis), worklogs, scripta,
# provenientiam - et capsulam emittit in build/speculum/<titulus>/
# per bin/capsula_generare.
#
# USUS (a compile_tests.sh invocatum):
#   [SPECULUM_VEXILLA="<vexilla>"] \
#   tools/speculum_generare.sh <titulus> <app.c> <fabrica>
#
# Decisiones: project-specs/speculum-spec-v2.md (domus = build/,
# compressio obligatoria, quine enumeratur non inseritur).
# AB AEDILE (2026-07-21): listae fontium non iam traduntur -
# clausura ex manifesto aedilis legitur (copia recompilationis
# HONESTA = clausura nexus, non mundus necte-omnia).

# (sine set -u: bash 3.2 in macOS tabulata vacua "${x[@]}" sub eo
# frangit; probationes explicitae infra)

CAERULEUS="\033[34m"
FLAVUS="\033[33m"
RUBER="\033[31m"
NULLUS="\033[0m"

si_fracta() { echo -e "${RUBER}speculum_generare: $1${NULLUS}" >&2; exit 1; }

[ $# -eq 3 ] || si_fracta "argumenta: <titulus> <app.c> <fabrica>"

TITULUS="$1"; APP="$2"; FABRICA="$3"
[ -n "$TITULUS" ] || si_fracta "titulus vacuus"
[ -f "$APP" ] || si_fracta "app non exstat: $APP"

VEXILLA="${SPECULUM_VEXILLA:-}"

STAGE="build/speculum/$TITULUS"
SECTIO="speculi_$TITULUS"
EXITUS_C="$STAGE/capsula_${SECTIO}.c"
BASIS=$(basename "$APP" .c)
MANIFESTUM="build/aedilis/$BASIS/manifestum.stml"

# ------------------------------------------------------------------
# porta recentiae: si nihil recentius quam capsula emissa, salta
# (provenientia inter mutationes fontium consenescit - acceptum)
# ------------------------------------------------------------------
if [ -f "$EXITUS_C" ]; then
    RECENTIOR=$(find "$APP" "$FABRICA" "$0" aedilis.stml include lib \
        -newer "$EXITUS_C" 2>/dev/null | head -1)
    if [ -z "$RECENTIOR" ]; then
        echo -e "${CAERULEUS}speculum ($TITULUS): recens${NULLUS}"
        exit 0
    fi
fi

INITIUM=$(date +%s)
echo -e "${CAERULEUS}speculum ($TITULUS): genero...${NULLUS}"

# ------------------------------------------------------------------
# clausura vera ab aedile: derivatio recens -> manifestum in
# capsulam, partes ex SUPERFICIE QUAESTIONIS (--partes: O/C/S/V
# via, TSV) - NON ex manifesto STML rasili. Forma pulchra
# attributa trans lineas frangit (stml decretum quintum
# 2026-08-25); exemplaria linearum suturam tagum-attributum
# transeuntia ('<obiectum via=') tacite mendacia fiunt. S omissa
# ut olim (capita systematis non sunt plagulae nostrae);
# obiecta sub build/ = generata (annotationes) -> exclusa.
# ------------------------------------------------------------------
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh || si_fracta "aedilis non structus"
./bin/aedilis "$APP" > /dev/null || si_fracta "derivatio aedilis fracta: $APP"
[ -f "$MANIFESTUM" ] || si_fracta "manifestum deest: $MANIFESTUM"

PARTES=$(./bin/aedilis "$APP" --partes | awk -F'\t' '$1 != "S"') \
    || si_fracta "partes aedilis fractae: $APP"

CLAUSURA=$( { echo "$APP"
    printf '%s\n' "$PARTES" | awk -F'\t' \
        '$1 != "V" && $2 !~ /^build\// && $2 !~ /^vendor\//  { print $2 }'
    } | sort -u )
VENDOR_CLAUSA=$(printf '%s\n' "$PARTES" | awk -F'\t' \
    '($1 == "V") || ($2 ~ /^vendor\//) { print $2 }' | sort -u)
GENERATA=$(printf '%s\n' "$PARTES" | awk -F'\t' \
    '$1 == "O" && $2 ~ /^build\// { print $2 }' | sort -u)

[ -n "$CLAUSURA" ] || si_fracta "clausura vacua ex manifesto"

# ------------------------------------------------------------------
# documenta: worklog pro omni fonte lib/ incluso
# ------------------------------------------------------------------
DOCUMENTA=()
while IFS= read -r f; do
    case "$f" in
        lib/*.c|lib/*.m)
            w="${f%.*}.worklog.md"
            [ -f "$w" ] && DOCUMENTA+=("$w")
            ;;
    esac
done <<< "$CLAUSURA"

# ------------------------------------------------------------------
# scripta: fabrica + hic generator + bona speculi (si iam exstant)
# ------------------------------------------------------------------
SCRIPTA=("$FABRICA" "tools/speculum_generare.sh")
if [ -d "lib/speculum_assets" ]; then
    while IFS= read -r f; do
        [ -n "$f" ] && SCRIPTA+=("$f")
    done < <(find lib/speculum_assets -type f | sort)
fi

# ------------------------------------------------------------------
# statio: arborem mundam struere
# limes magnitudinis: corpora > 1MB (biblia_dr, tabulae generatae,
# capsulae librorum) in exclusa eunt, non inseruntur - speculum
# codicem monstrat, non data-ut-codicem
# ------------------------------------------------------------------
LIMES_MAGNITUDINIS=1048576

rm -rf "$STAGE"
mkdir -p "$STAGE/speculum"

OMNIA=()
MAGNA=()
while IFS= read -r f; do
    [ -n "$f" ] || continue
    if [ "$(wc -c < "$f" | tr -d ' ')" -gt "$LIMES_MAGNITUDINIS" ]; then
        MAGNA+=("$f")
    else
        OMNIA+=("$f")
    fi
done <<< "$CLAUSURA"
OMNIA+=("${DOCUMENTA[@]}" "${SCRIPTA[@]}")

for f in "${OMNIA[@]}"; do
    [ -f "$f" ] || si_fracta "fons stationis non exstat: $f"
    mkdir -p "$STAGE/$(dirname "$f")"
    cp "$f" "$STAGE/$f"
done

# fontes.txt = index clausurae (inputus generatoris; decisio quine)
printf '%s\n' "${OMNIA[@]}" | sort -u > "$STAGE/speculum/fontes.txt"

# ------------------------------------------------------------------
# provenientia (clavis=valor; latus C in JSON transformat)
# ------------------------------------------------------------------
COMMISSUM=$(git rev-parse HEAD 2>/dev/null || echo "ignotum")
SORDIDUM="falsum"
[ -n "$(git status --porcelain 2>/dev/null)" ] && SORDIDUM="verum"
COMPILATOR=$(clang --version 2>/dev/null | head -1)

{
    echo "app=$TITULUS"
    echo "plagula_app=$APP"
    echo "commissum=$COMMISSUM"
    echo "sordidum=$SORDIDUM"
    echo "compilator=$COMPILATOR"
    echo "vexilla=$VEXILLA"
    echo "tempus=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
    echo "mandatum=bash build/aedilis/$BASIS/struere.sh (manifestum: $MANIFESTUM; regeneratio: bin/aedilis $APP)"
} > "$STAGE/speculum/proventus.txt"

# manifestum aedilis ipsum in capsulam - speculum veritatem
# dependentiarum suarum secum fert
cp "$MANIFESTUM" "$STAGE/speculum/manifestum.stml"

# ------------------------------------------------------------------
# exclusa (TSV: via, magnitudo, cksum, causa)
# quine: emissa ipsa enumeratur, corpus non inseritur (spec #17)
# ------------------------------------------------------------------
{
    if [ -n "$VENDOR_CLAUSA" ]; then
        while IFS= read -r f; do
            [ -f "$f" ] || continue
            printf '%s\t%s\t%s\tvendor\n' "$f" \
                "$(wc -c < "$f" | tr -d ' ')" "$(cksum "$f" | awk '{print $1}')"
        done <<< "$VENDOR_CLAUSA"
    fi
    if [ -n "$GENERATA" ]; then
        while IFS= read -r f; do
            [ "$f" = "$EXITUS_C" ] && continue
            if [ -f "$f" ]; then
                printf '%s\t%s\t%s\tgeneratum (annotatio)\n' "$f" \
                    "$(wc -c < "$f" | tr -d ' ')" \
                    "$(cksum "$f" | awk '{print $1}')"
            else
                printf '%s\t0\t0\tgeneratum (annotatio, nondum)\n' "$f"
            fi
        done <<< "$GENERATA"
    fi
    for f in "${MAGNA[@]}"; do
        printf '%s\t%s\t%s\tmagnitudo>1MB\n' "$f" \
            "$(wc -c < "$f" | tr -d ' ')" "$(cksum "$f" | awk '{print $1}')"
    done
    printf '%s\t0\t0\tgeneratum - continet hos ipsos fontes\n' "$EXITUS_C"
} > "$STAGE/speculum/exclusa.txt"

# ------------------------------------------------------------------
# toml: viae explicitae (glob per-directorium etiam DIRECTORIA
# congruebat - introitus vacui; index explicitus id occidit)
# ------------------------------------------------------------------
STATA=("${OMNIA[@]}" "speculum/fontes.txt" "speculum/proventus.txt" "speculum/exclusa.txt" "speculum/manifestum.stml")
GLOBAE=""
while IFS= read -r f; do
    [ -z "$f" ] && continue
    GLOBAE="$GLOBAE\"$f\", "
done < <(printf '%s\n' "${STATA[@]}" | sort -u)
GLOBAE="${GLOBAE%, }"

{
    echo "# generatum a tools/speculum_generare.sh - noli manu mutare"
    echo "${SECTIO}_files = [ $GLOBAE ]"
    echo "${SECTIO}_compress = true"
} > "$STAGE/speculum.toml"

# ------------------------------------------------------------------
# emissio
# ------------------------------------------------------------------
[ -x "bin/capsula_generare" ] || si_fracta "bin/capsula_generare deest"
./bin/capsula_generare "$STAGE/speculum.toml" > /dev/null \
    || si_fracta "capsula_generare fracta"
[ -f "$EXITUS_C" ] || si_fracta "emissio deest: $EXITUS_C"

FINIS=$(date +%s)
NUMERUS=$(wc -l < "$STAGE/speculum/fontes.txt" | tr -d ' ')
MAGNITUDO=$(wc -c < "$EXITUS_C" | tr -d ' ')
echo -e "${CAERULEUS}speculum ($TITULUS): $NUMERUS plagulae + meta -> $EXITUS_C (${MAGNITUDO}B, $((FINIS - INITIUM))s)${NULLUS}"
