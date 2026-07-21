#!/bin/bash

# tools/amalgama_ligare.sh - fasciculum domesticum ligare (Phasis D)
#
# CASUS USUS ORIGINALIS: plagulas C datas cum clausura earum in
# fasciculum unum (<titulus>.h + <titulus>.c) ligare, quem proiectum
# ALTERUM includere potest - API originalis, latina inclusa (gradus
# domesticus; distributio externa = amalgamatores semantici silvae).
#
# Linea: aedilis derivat (unio --partes/--aristae super radices) ->
# recusationes (vendor / .c textualia / absentia) -> capita ordine
# topologico -> lista -> amalgama_ligator (mechanismus communis bis:
# capita -> .h, plenum -> .c) -> PORTAE: compilatio standalone
# severitate plena + consumptor .h + aequalitas symbolorum nm
# (fasciculum == unio obiectorum singulorum, nihil perditum nihil
# inventum). S41 (statica duplicata) in mechanismo gratis.
#
# Usus: ./tools/amalgama_ligare.sh <titulus> <fons.c> [...]
# Exitus: build/amalgamata/<titulus>/<titulus>.{h,c} (generata,
# numquam commissa - consumptor exportat)

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

si_fracta() { echo "amalgama_ligare: $1" >&2; exit 1; }

[ $# -ge 2 ] || si_fracta "usus: amalgama_ligare.sh <titulus> <fons.c> [...]"
TITULUS="$1"; shift

declare -a VEXILLA=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)

./tools/aedilis_struere.sh || exit 1

STATIO="build/amalgamata/$TITULUS"
rm -rf "$STATIO"
mkdir -p "$STATIO"

# ------------------------------------------------------------------
# derivatio + recusationes
# ------------------------------------------------------------------
: > "$STATIO/partes.tsv"
: > "$STATIO/aristae.tsv"
for radix in "$@"; do
    [ -f "$radix" ] || si_fracta "radix non exstat: $radix"
    ./bin/aedilis "$radix" --partes >> "$STATIO/partes.tsv" \
        || si_fracta "derivatio fracta: $radix"
    ./bin/aedilis "$radix" --aristae >> "$STATIO/aristae.tsv" \
        || si_fracta "aristae fractae: $radix"
done
sort -u "$STATIO/aristae.tsv" > "$STATIO/aristae_unicae.tsv"

VENDORES=$(awk -F'\t' '$1=="V" { print $2 }' "$STATIO/partes.tsv" | sort -u)
[ -z "$VENDORES" ] || si_fracta "vendor in clausura - non ligabile (v1):
$VENDORES"

awk -F'\t' '$1=="C" { print $2 }' "$STATIO/partes.tsv" | sort -u \
    | grep -v '^include/latina\.h$' > "$STATIO/capita.txt" || true
TEXTUALIA=$(grep -v '\.h$' "$STATIO/capita.txt" || true)
[ -z "$TEXTUALIA" ] || si_fracta "inclusio textualis .c in clausura - non ligabile (v1):
$TEXTUALIA"

{ awk -F'\t' '$1=="O" { print $2 }' "$STATIO/partes.tsv"
  printf '%s\n' "$@"; } | sort -u > "$STATIO/corpora.txt"
OBIECTIVA=$(grep '\.m$' "$STATIO/corpora.txt" || true)
[ -z "$OBIECTIVA" ] || si_fracta "Objective-C in clausura - non ligabile (v1):
$OBIECTIVA"
while IFS= read -r f; do
    [ -f "$f" ] || si_fracta "plagula clausurae non exstat: $f (generata nondum?)"
done < "$STATIO/corpora.txt"

# ------------------------------------------------------------------
# ordo topologicus capitum (candidati sorted = determinismus)
# ------------------------------------------------------------------
awk -F'\t' '
    NR==FNR { ordo[++n]=$0; in_copia[$0]=1; next }
    ($1 in in_copia) && ($2 in in_copia) { dep[$1] = dep[$1] SUBSEP $2 }
    END {
        numerus_emissorum = 0
        while (numerus_emissorum < n) {
            progressus = 0
            for (i = 1; i <= n; i++) {
                c = ordo[i]
                if (emissum[c]) continue
                paratum = 1
                m = split(dep[c], ds, SUBSEP)
                for (j = 1; j <= m; j++) {
                    if (ds[j] != "" && !emissum[ds[j]]) {
                        paratum = 0; break } }
                if (paratum) {
                    print c; emissum[c] = 1
                    numerus_emissorum++; progressus = 1 } }
            if (!progressus) {
                print "CYCLUS" > "/dev/stderr"; exit 1 } } }
' "$STATIO/capita.txt" "$STATIO/aristae_unicae.tsv" \
    > "$STATIO/capita_ordo.txt" || si_fracta "cyclus inclusionum"

{ sed 's/^/C\t/' "$STATIO/capita_ordo.txt"
  sed 's/^/O\t/' "$STATIO/corpora.txt"; } > "$STATIO/lista.tsv"

# ------------------------------------------------------------------
# ligator (mechanismus communis; obiecta silva/build a vecte silvae)
# ------------------------------------------------------------------
for o in piscina chorda chorda_aedificator xar tabula_dispersa \
         friatio silva_token silva_lexema; do
    [ -f "silva/build/$o.o" ] \
        || si_fracta "silva/build/$o.o deest (curre ./silva/amalgamare.sh prius)"
done
clang "${VEXILLA[@]}" -Iinclude -Isilva/fontes -Isilva/instrumenta \
    tools/amalgama_ligator.c silva/instrumenta/silva_amalgama.c \
    silva/build/piscina.o silva/build/chorda.o \
    silva/build/chorda_aedificator.o silva/build/xar.o \
    silva/build/tabula_dispersa.o silva/build/friatio.o \
    silva/build/silva_token.o silva/build/silva_lexema.o \
    -o "$STATIO/ligator" || si_fracta "constructio ligatoris"

"$STATIO/ligator" "$PWD" "$STATIO/lista.tsv" "$TITULUS" \
    "$STATIO/$TITULUS.h" "$STATIO/$TITULUS.c" \
    || si_fracta "cursus ligatoris"

# ------------------------------------------------------------------
# PORTAE
# ------------------------------------------------------------------
echo "  [porta] compilatio standalone (severitas plena)"
clang "${VEXILLA[@]}" -c "$STATIO/$TITULUS.c" \
    -o "$STATIO/$TITULUS.o" || si_fracta "compilatio standalone"

echo "  [porta] consumptor .h (inclusio sola, severitas plena)"
printf '#include "%s.h"\nint main(void) { return 0; }\n' "$TITULUS" \
    > "$STATIO/consumptor.c"
clang "${VEXILLA[@]}" -I"$STATIO" -c "$STATIO/consumptor.c" \
    -o "$STATIO/consumptor.o" || si_fracta "consumptor capitis"

echo "  [porta] aequalitas symbolorum (fasciculum == unio singulorum)"
: > "$STATIO/syms_singula.txt"
while IFS= read -r f; do
    b="${f##*/}"; b="${b%.*}"
    clang "${VEXILLA[@]}" -Iinclude -c "$f" \
        -o "$STATIO/singulum_$b.o" \
        || si_fracta "compilatio singuli: $f"
    nm -gU "$STATIO/singulum_$b.o" | awk '{print $3}' \
        >> "$STATIO/syms_singula.txt"
done < "$STATIO/corpora.txt"
sort -u -o "$STATIO/syms_singula.txt" "$STATIO/syms_singula.txt"
nm -gU "$STATIO/$TITULUS.o" | awk '{print $3}' | sort -u \
    > "$STATIO/syms_fasciculi.txt"
if ! diff "$STATIO/syms_singula.txt" "$STATIO/syms_fasciculi.txt" \
        > "$STATIO/syms_differentia.txt"; then
    echo "amalgama_ligare: FRACTA - symbola non aequalia:" >&2
    head -10 "$STATIO/syms_differentia.txt" >&2
    exit 1
fi

NUMERUS_C=$(wc -l < "$STATIO/capita_ordo.txt" | tr -d ' ')
NUMERUS_O=$(wc -l < "$STATIO/corpora.txt" | tr -d ' ')
MAGNITUDO_C=$(wc -c < "$STATIO/$TITULUS.c" | tr -d ' ')
echo "amalgama_ligare: VERIFICATUM - $STATIO/$TITULUS.{h,c}"
echo "  ($NUMERUS_C capita, $NUMERUS_O corpora, ${MAGNITUDO_C}B; portae: standalone + consumptor + nm-aequalitas)"
