#!/bin/bash

# silva/renominare.sh - renominatio identificatorum (parcum 01KYWYQMJ7)
#
# Usage:
#   ./silva/renominare.sh <vetus> <novum>            # candidati ex nexus
#   ./silva/renominare.sh <vetus> <novum> plagulae... # explicite
#   ... -scribere                                    # applicare (planum aliter)
#   ... -via <plagula>                               # staticum disambiguare
#   ./silva/renominare.sh -proba                     # porta aurea (fixa)
# Exit: 0 planum/scriptum | 1 refusio | 2 apparatus | 3 recuso | 4 fractum
#
# Semper ex radice repositorii currit (cd infra). Candidati sine
# plagulis datis ex build/nexus.tsv derivantur (sedes + usus vetus);
# post -scribere relatio residualis textualis (grep) super arborem.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
)
# GENERATUM AB AEDILE - fontes derivati
source "$SILVA_DIR/renominare_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 2
    fi
    obj_files="$obj_files $obj"
done

RENOMINARE_SRC="$SILVA_DIR/instrumenta/principalia/renominare.c"
RENOMINARE_BIN="$BUILD_DIR/renominare"
if [ ! -f "$RENOMINARE_BIN" ] || [ "$RENOMINARE_SRC" -nt "$RENOMINARE_BIN" ] \
   || [ -n "$(newest_header "$RENOMINARE_BIN")" ]; then
    echo "  [renominare] renominare.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
        -I"$SILVA_DIR/instrumenta" "$RENOMINARE_SRC" \
        "$SILVA_DIR/instrumenta/silva_lexicon.c" $obj_files \
        -o "$RENOMINARE_BIN" || exit 2
fi

cd "$RADIX_DIR"

# -proba: porta aurea super fixa (scripta seorsum)
if [ "${1:-}" = "-proba" ]; then
    exec "$SILVA_DIR/renominare_proba.sh"
fi

VETUS="${1:?renominare: vetus deest}"
NOVUM="${2:?renominare: novum deest}"
shift 2

# vexilla et plagulae separanda
declare -a VEXILLA=()
declare -a PLAGULAE=()
SCRIBERE=0
while [ $# -gt 0 ]; do
    case "$1" in
        -scribere) SCRIBERE=1; VEXILLA+=("$1") ;;
        -via)      VEXILLA+=("$1" "${2:?-via plagula deest}"); shift ;;
        -intra)    INTRA="${2:?-intra functio deest}"
                   VEXILLA+=("$1" "$2"); shift ;;
        -linea)    VEXILLA+=("$1" "${2:?-linea numerus deest}"); shift ;;
        -machina|-v|-lista) VEXILLA+=("$1") ;;
        *)         PLAGULAE+=("$1") ;;
    esac
    shift
done

# candidati ex nexus.tsv si plagulae non datae (sedes + usus;
# nexus.sh sanationem sui ipsius facit). Modo -intra: plagulae =
# sedes FUNCTIONIS (localis in plagula definiente vivit)
if [ ${#PLAGULAE[@]} -eq 0 ]; then
    CLAVIS_NEXUS="${INTRA:-$VETUS}"
    "$SILVA_DIR/nexus.sh" "$CLAVIS_NEXUS" >/dev/null 2>&1 || true
    if [ ! -f "$RADIX_DIR/build/nexus.tsv" ]; then
        echo "renominare: nexus.tsv deest (curre ./silva/nexus.sh -renovare)" >&2
        exit 2
    fi
    if [ -n "${INTRA:-}" ]; then
        while IFS= read -r via; do
            PLAGULAE+=("$via")
        done < <(awk -F'\t' -v t="$INTRA" \
                     '$1==t && $2=="sedes" {print $4}' \
                     "$RADIX_DIR/build/nexus.tsv" | sort -u)
    else
        while IFS= read -r via; do
            PLAGULAE+=("$via")
        done < <(awk -F'\t' -v t="$VETUS" '$1==t {print $4}' \
                     "$RADIX_DIR/build/nexus.tsv" | sort -u)
    fi
    if [ ${#PLAGULAE[@]} -eq 0 ]; then
        echo "renominare: nullae plagulae candidatae ($CLAVIS_NEXUS ignotum nexui)" >&2
        exit 1
    fi
    echo "candidatae ex nexu: ${#PLAGULAE[@]} plagulae" >&2
fi

# -scribere: arbor sordida in plagulis affectis = refusio
if [ "$SCRIBERE" = 1 ]; then
    sordidae="$(git status --porcelain -- "${PLAGULAE[@]}" 2>/dev/null)"
    if [ -n "$sordidae" ]; then
        echo "renominare: plagulae affectae sordidae (git = revocatio - committe primum):" >&2
        echo "$sordidae" >&2
        exit 1
    fi
fi

"$RENOMINARE_BIN" "$VETUS" "$NOVUM" \
    ${VEXILLA[@]+"${VEXILLA[@]}"} "${PLAGULAE[@]}"
exitus=$?

# relatio residualis post scriptionem: mentio textualis usquam?
if [ "$SCRIBERE" = 1 ] && [ "$exitus" = 0 ]; then
    echo "--- relatio residualis (textus, commenta, .m, scripta) ---"
    grep -rn --include='*.c' --include='*.h' --include='*.m' \
         --include='*.sh' --include='*.stml' -w "$VETUS" \
         --exclude-dir=build --exclude-dir=.git \
         --exclude-dir=node_modules . 2>/dev/null \
        | grep -v '^\./silva/build/' | head -40 || true
fi
exit "$exitus"
