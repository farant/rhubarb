#!/bin/bash

# silva/examen.sh - effusor iudicii C89 (examen, M4a chunk D)
#
# Usage:
#   ./silva/examen.sh <plagula.c> [-posix|-nudum] [-machina] [-v]
#   ./silva/examen.sh -census [codex] [radix] [-posix]
# Exit:  0 ACCIPE | 1 REICE | 2 INFRA (apparatus) | 3 RECUSO (fines)
#        (-census: 0 = percursus perfectus, 1 = apparatus)
#
# NB: semper ex radice repositorii currit (cd infra) - viae datae
# relativae ad radicem sunto.

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
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/examen_fontes_generata.sh"

# Obiecta vetusta capitibus novis = valores enum falsi (exemplar
# percursus.sh; obiecta communia in silva/build/ conpartita)
newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

EXAMEN_SRC="$SILVA_DIR/instrumenta/principalia/examen.c"
EXAMEN_BIN="$BUILD_DIR/examen"
# silva_lexicon.c in linea nectendi explicite (exemplar amalgamare:71
# pro silva_amalgama.c) - modulus instrumentorum communis, extra
# fontes/ quia instrumentum est, non substratum amalgamandum
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    -I"$SILVA_DIR/instrumenta" \
    "$EXAMEN_SRC" \
    "$SILVA_DIR/instrumenta/silva_lexicon.c" $obj_files \
    -o "$EXAMEN_BIN" || exit 1

# manifestum vigiliae (invarians: aedificator indicem VERUM scribit -
# binarium directe vocatum contra hunc fontes suos vigilat)
{
    for f in "${RADIX_FONTES[@]}"; do echo "$RADIX_DIR/lib/$f.c"; done
    ls "$SILVA_DIR"/fontes/*.c "$SILVA_DIR"/fontes/*.h
    echo "$EXAMEN_SRC"
    echo "$SILVA_DIR/instrumenta/silva_lexicon.c"
    echo "$SILVA_DIR/instrumenta/silva_lexicon.h"
    ls "$RADIX_DIR"/include/*.h
} > "$BUILD_DIR/examen.manifestum"

cd "$RADIX_DIR"

# ----------------------------------------------------------------
# -census [codex] [radix]: percursus corporis (desideratum
# 01KXRXW70T) - formulam manualem codificat: index plagularum .c
# (praetermissis build/.git/results/node_modules/vendor/occultis -
# vendor extra dominium iudicii, ut fixa/ in adiudicationibus;
# radix data ipsa numquam praetermittitur - "-census 54 vendor"
# vendorem consulto percurrit; percursus.c etiam .h ambulat,
# census .c solum), xargs -P 4 super binarium recens, ordines
# crudi in build/census.tsv MANENT (re-sectio sine percursu novo);
# ordo VERDICTUM_PLAGULAE\tvia\tACCIPE|REICE|INFRA per plagulam
# additus - verdicta attribuenda ("quae plagulae REICE").
# Cum codice: sedes singulae + summa per plagulam + subtotalis
# fixa/. Sine codice: tabula codex x sedes x plagulae. Verdicta
# ACCIPE/REICE numerata; plagulae INFRA (exitus 2) NOMINATAE -
# nihil tacite demissum. Instrumentum oculorum, non porta.
# ----------------------------------------------------------------
census_modus=0
for a in "$@"; do
    if [ "$a" = "-census" ]; then census_modus=1; fi
done

if [ "$census_modus" -eq 0 ]; then
    exec "$EXAMEN_BIN" "$@"
fi

CODEX=""
CRADIX="."
CENSUS_POSIX=""
for a in "$@"; do
    case "$a" in
        -census) ;;
        -posix)  CENSUS_POSIX="-posix" ;;
        *[!0-9]*|'')
            if [ -d "$a" ]; then
                CRADIX="${a%/}"
            else
                echo "census: argumentum ignotum: $a" >&2
                exit 1
            fi
            ;;
        *) CODEX="$a" ;;
    esac
done

CENSUS_TSV="$BUILD_DIR/census.tsv"
CENSUS_INFRA="$BUILD_DIR/census.infra"
CENSUS_INDEX="$BUILD_DIR/census.plagulae"

find "$CRADIX" \
    \( -type d ! -path "$CRADIX" \( -name '.?*' -o -name build \
        -o -name results -o -name node_modules \
        -o -name vendor \) \) -prune \
    -o -type f -name '*.c' ! -name '.*' -print \
    | sed 's|^\./||' | LC_ALL=C sort > "$CENSUS_INDEX"

n_plagulae="$(wc -l < "$CENSUS_INDEX" | tr -d ' ')"
if [ "$n_plagulae" -eq 0 ]; then
    echo "census: nullae plagulae sub $CRADIX" >&2
    exit 1
fi
echo "census: $n_plagulae plagulae percurrendae (xargs -P 4)..." >&2

: > "$CENSUS_TSV"
: > "$CENSUS_INFRA"
export CENSUS_BIN="$EXAMEN_BIN"
export CENSUS_TSV CENSUS_INFRA CENSUS_POSIX

xargs -P 4 -n 1 sh -c '
    "$CENSUS_BIN" -machina $CENSUS_POSIX "$1" >> "$CENSUS_TSV"
    case "$?" in
        0) v=ACCIPE ;;
        1) v=REICE ;;
        *) v=INFRA; echo "$1" >> "$CENSUS_INFRA" ;;
    esac
    printf "VERDICTUM_PLAGULAE\t%s\t%s\n" "$1" "$v" \
        >> "$CENSUS_TSV"
    exit 0
' sh < "$CENSUS_INDEX"

echo ""
if [ -n "$CODEX" ]; then
    SECTIO="$BUILD_DIR/census.sectio"
    awk -F'\t' -v cx="$CODEX" \
        '$1 != "VERDICTUM" && NF >= 7 && $5 == cx' "$CENSUS_TSV" \
        | LC_ALL=C sort -t"$(printf '\t')" -k1,1 -k2,2n > "$SECTIO"

    echo "CENSUS codicis $CODEX  ($n_plagulae plagulae${CENSUS_POSIX:+ -posix})"
    awk -F'\t' \
        '{ printf "  %s:%s:%s  [%s] %s\n", $1, $2, $3, $4, $7 }' \
        "$SECTIO"
    awk -F'\t' '
        { n[$1]++; if (index($1, "fixa/") > 0) f += 1 }
        END {
            print ""
            print "per plagulam:"
            for (v in n) printf "%5d  %s\n", n[v], v | "sort -rn"
            close("sort -rn")
            printf "sedes %d in %d plagulis (in fixa/: %d)\n",
                NR, length(n), f + 0
        }
    ' "$SECTIO"
else
    echo "CENSUS omnium codicum  ($n_plagulae plagulae${CENSUS_POSIX:+ -posix})"
    awk -F'\t' '
        $1 == "VERDICTUM" { next }
        NF >= 7 {
            sedes[$5]++
            pf[$5 "\t" $1] = 1
            if (index($1, "fixa/") > 0) fixa[$5]++
        }
        END {
            for (k in pf) { split(k, a, "\t"); plag[a[1]]++ }
            for (c in sedes)
                printf "  codex %-4d sedes %5d  plagulae %4d  (in fixa/ %d)\n",
                    c, sedes[c], plag[c], fixa[c] + 0 | "sort -k2,2n"
            close("sort -k2,2n")
        }
    ' "$CENSUS_TSV"
fi

awk -F'\t' '
    $1 == "VERDICTUM_PLAGULAE" { v[$3]++ }
    END { printf "verdicta: ACCIPE %d, REICE %d, INFRA %d\n",
        v["ACCIPE"] + 0, v["REICE"] + 0, v["INFRA"] + 0 }
' "$CENSUS_TSV"

if [ -s "$CENSUS_INFRA" ]; then
    echo "INFRA (apparatus - NON iudicatae):"
    sed 's/^/  /' "$CENSUS_INFRA"
fi
echo "tabula cruda: silva/build/census.tsv" \
    "(re-sectio: awk -F'\\t' '\$5==N')"
exit 0
