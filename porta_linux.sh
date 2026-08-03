#!/bin/bash
# porta_linux.sh - PORTA compilationis Linux (desideratum 01KZ40Y1ZD)
#
# Oraculum stans codicum 85-87: quod examen praedicit, gcc verus
# mensurat. Percursus lib/*.c + gesta/fontes/*.c in vase glibc
# (et musl sub -musl); fracturae contra pinnas exspectatas
# (tools/linux/exspectata_*.txt, forma via<TAB>causa) comparantur.
#
# Usus: ./porta_linux.sh            # cursus glibc
#       ./porta_linux.sh -musl      # + cursus musl
#       ./porta_linux.sh -fumus     # + tabulariumd aedificatur,
#                                   #   currit, respondet INTRA vas
# Exitus: 0 = frons tenet | 1 = fractura NOVA (nominata) | 2 = apparatus
#
# Decreta vexillorum (2026-08-03, in actis 01KZ40Y1ZD):
#   -Wno-type-limits: idioma custodiae bilateralis = stilus consultus
#   vendor (imago/stb): pinnatum - suite vera vendorem laxat
#   arbores legatae: extra scopum (referentia congelata)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")" || exit 2

SCRATCH="build/porta_linux"
mkdir -p "$SCRATCH"

MUSL=0
FUMUS=0
for a in "$@"; do
    case "$a" in
        -musl)  MUSL=1 ;;
        -fumus) FUMUS=1 ;;
        *) echo "porta_linux: argumentum ignotum $a" >&2; exit 2 ;;
    esac
done

imaginem_praestare () {  # $1 = titulus, $2 = Dockerfile
    if ! docker image inspect "$1" >/dev/null 2>&1; then
        echo "[porta] imago $1 aedificatur..." >&2
        docker build -t "$1" -f "$2" tools/linux || exit 2
    fi
}

cursus () {  # $1 = imago, $2 = pinnae, $3 = titulus laneae
    local EX="$SCRATCH/$3"
    mkdir -p "$EX"
    rm -rf "$EX/errores" "$EX/percursus_summa.tsv"
    docker run --rm -v "$PWD":/rhubarb:ro -v "$PWD/$EX":/ex \
        -w /rhubarb "$1" sh tools/linux/percursus.sh \
        > "$EX/compendium.txt" 2>&1 || { echo "porta ($3): apparatus" >&2; exit 2; }

    local novae=0
    local vanae=0
    while IFS=$'\t' read -r via exitus errores; do
        [ "$exitus" = "0" ] && continue
        if ! grep -q "^$via	" "$2"; then
            echo "PORTA ($3): FRACTURA NOVA $via ($errores errores):" >&2
            b=$(echo "$via" | tr '/' '_')
            grep -m3 'error:' "$EX/errores/$b.txt" >&2
            novae=$((novae+1))
        fi
    done < "$EX/percursus_summa.tsv"

    while IFS=$'\t' read -r via causa; do
        case "$via" in \#*|'') continue ;; esac
        exitus=$(awk -F'\t' -v v="$via" '$1==v {print $2}' "$EX/percursus_summa.tsv")
        if [ "$exitus" = "0" ]; then
            echo "porta ($3): pinna VANA $via (iam munda - remove causam: $causa)" >&2
            vanae=$((vanae+1))
        fi
    done < "$2"

    echo "porta ($3): $(cat "$EX/compendium.txt") - novae $novae, pinnae vanae $vanae"
    [ "$novae" -gt 0 ] && return 1
    return 0
}

imaginem_praestare rhubarb-proba-glibc tools/linux/Dockerfile.glibc
EXITUS=0
cursus rhubarb-proba-glibc tools/linux/exspectata_glibc.txt glibc || EXITUS=1

if [ "$MUSL" = "1" ]; then
    imaginem_praestare rhubarb-proba-musl tools/linux/Dockerfile.musl
    cursus rhubarb-proba-musl tools/linux/exspectata_musl.txt musl || EXITUS=1
fi

if [ "$FUMUS" = "1" ] && [ "$EXITUS" = "0" ]; then
    echo "[porta] fumus tabulariumd..." >&2
    docker run --rm -v "$PWD":/rhubarb:ro -w /rhubarb \
        rhubarb-proba-glibc bash tools/linux/fumus_tabulariumd.sh || EXITUS=1
fi

if [ "$EXITUS" = "0" ]; then
    echo "PORTA LINUX: TENET"
else
    echo "PORTA LINUX: FRACTA" >&2
fi
exit "$EXITUS"
