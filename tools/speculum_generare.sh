#!/bin/bash
# speculum_generare.sh - generator fontium speculi
#
# Colligit copiam recompilationis app (clausura vera per clang -MM),
# worklogs, scripta, provenientiam - et capsulam emittit in
# build/speculum/<titulus>/ per bin/capsula_generare.
#
# USUS (a compile_tests.sh invocatum):
#   SPECULUM_VEXILLA="<vexilla>" SPECULUM_INCLUSA="<-I...>" \
#   tools/speculum_generare.sh <titulus> <app.c> <fabrica> \
#       -- <fontes .c...> -- <fontes .m...> -- <vendor...>
#
# Decisiones: project-specs/speculum-spec-v2.md (domus = build/,
# compressio obligatoria, quine enumeratur non inseritur).

# (sine set -u: bash 3.2 in macOS tabulata vacua "${x[@]}" sub eo
# frangit; probationes explicitae infra)

CAERULEUS="\033[34m"
FLAVUS="\033[33m"
RUBER="\033[31m"
NULLUS="\033[0m"

si_fracta() { echo -e "${RUBER}speculum_generare: $1${NULLUS}" >&2; exit 1; }

[ $# -ge 4 ] || si_fracta "argumenta desunt (titulus app fabrica -- fontes...)"

TITULUS="$1"; APP="$2"; FABRICA="$3"; shift 3
[ -n "$TITULUS" ] || si_fracta "titulus vacuus"
[ -f "$APP" ] || si_fracta "app non exstat: $APP"
[ "${1:-}" == "--" ] && shift

C_FONTES=()
while [ $# -gt 0 ] && [ "$1" != "--" ]; do C_FONTES+=("$1"); shift; done
[ $# -gt 0 ] && [ "$1" == "--" ] && shift
OBJC_FONTES=()
while [ $# -gt 0 ] && [ "$1" != "--" ]; do OBJC_FONTES+=("$1"); shift; done
[ $# -gt 0 ] && [ "$1" == "--" ] && shift
VENDOR_FONTES=("$@")

read -ra INCLUSA <<< "${SPECULUM_INCLUSA:-}"
VEXILLA="${SPECULUM_VEXILLA:-}"

STAGE="build/speculum/$TITULUS"
SECTIO="speculi_$TITULUS"
EXITUS_C="$STAGE/capsula_${SECTIO}.c"

FONTES=("${C_FONTES[@]}" "${OBJC_FONTES[@]}" "$APP")

# ------------------------------------------------------------------
# porta recentiae: si nihil recentius quam capsula emissa, salta
# (provenientia inter mutationes fontium consenescit - acceptum)
# ------------------------------------------------------------------
if [ -f "$EXITUS_C" ]; then
    RECENTIOR=$(find "${FONTES[@]}" "$FABRICA" "$0" include lib \
        -newer "$EXITUS_C" 2>/dev/null | head -1)
    if [ -z "$RECENTIOR" ]; then
        echo -e "${CAERULEUS}speculum ($TITULUS): recens${NULLUS}"
        exit 0
    fi
fi

INITIUM=$(date +%s)
echo -e "${CAERULEUS}speculum ($TITULUS): genero...${NULLUS}"

# ------------------------------------------------------------------
# clausura vera per clang -MM (forma benedicta: gesta/tabularium.sh)
# viae absolutae = systema (abiciuntur); vendor/ -> exclusa
# ------------------------------------------------------------------
CRUDA=$(clang -MM "${INCLUSA[@]}" "${FONTES[@]}" 2>/dev/null \
    | awk '{ if (sub(/\\$/,"")) printf "%s ", $0; else print }')
CLAUSURA=$(printf '%s\n' "$CRUDA" | tr ' ' '\n' \
    | grep -v ':$' | grep -v '^$' | grep -v '^/' | sort -u)

if [ -z "$CLAUSURA" ]; then
    # garrulum, numquam caecum: superserie glob reccidimus
    echo -e "${FLAVUS}speculum: clang -MM vacuum - superserie utor${NULLUS}" >&2
    CLAUSURA=$( { printf '%s\n' "${FONTES[@]}"; ls include/*.h; } | sort -u )
fi

VENDOR_CLAUSA=$(printf '%s\n' "$CLAUSURA" | grep '^vendor/' || true)
CLAUSURA=$(printf '%s\n' "$CLAUSURA" | grep -v '^vendor/' || true)

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

OBIECTA=""
for f in "${C_FONTES[@]}"; do
    OBIECTA="$OBIECTA build/$(basename "$f" .c).o"
done
for f in "${OBJC_FONTES[@]}"; do
    OBIECTA="$OBIECTA build/$(basename "$f" .m).o"
done
for f in "${VENDOR_FONTES[@]}"; do
    OBIECTA="$OBIECTA build/$(basename "$f" .c).o"
done

{
    echo "app=$TITULUS"
    echo "plagula_app=$APP"
    echo "commissum=$COMMISSUM"
    echo "sordidum=$SORDIDUM"
    echo "compilator=$COMPILATOR"
    echo "vexilla=$VEXILLA"
    echo "tempus=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
    echo "mandatum=clang $VEXILLA -Wno-overlength-strings ${SPECULUM_INCLUSA:-} $APP$OBIECTA -framework Cocoa -framework Security -framework WebKit -o bin/$(basename "$APP" .c)"
} > "$STAGE/speculum/proventus.txt"

# ------------------------------------------------------------------
# exclusa (TSV: via, magnitudo, cksum, causa)
# quine: emissa ipsa enumeratur, corpus non inseritur (spec #17)
# ------------------------------------------------------------------
{
    for f in "${VENDOR_FONTES[@]}"; do
        [ -f "$f" ] || continue
        printf '%s\t%s\t%s\tvendor\n' "$f" \
            "$(wc -c < "$f" | tr -d ' ')" "$(cksum "$f" | awk '{print $1}')"
    done
    if [ -n "$VENDOR_CLAUSA" ]; then
        while IFS= read -r f; do
            [ -f "$f" ] || continue
            printf '%s\t%s\t%s\tvendor\n' "$f" \
                "$(wc -c < "$f" | tr -d ' ')" "$(cksum "$f" | awk '{print $1}')"
        done <<< "$VENDOR_CLAUSA"
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
STATA=("${OMNIA[@]}" "speculum/fontes.txt" "speculum/proventus.txt" "speculum/exclusa.txt")
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
