#!/bin/bash

# silva/formator.sh - lint formae domus (regulae formandi, G1)
#
# Usage:
#   ./silva/formator.sh <via.c> [viae ...] [-machina]
#   ./silva/formator.sh <via.c> [viae ...] -delta [ref]     (ref = HEAD)
#   ./silva/formator.sh <via.c> -contra <basis.c>           (basis explicita)
# Exit:  0 conformis | 1 divergentiae | 2 usus / plagula illegibilis
#
# -delta = LEX BASIS ('mensura baselineam propriam, adde zephyrum'):
#   divergentiae NOVAE contra versionem ref (git show) - per REGULAM +
#   CONTENTUM LINEAE, numquam per numerum lineae (numeri labuntur cum
#   editione; tally classium permutationem celat: +I -I = zephyrum
#   mendax). Effusum: sedes novae + summa DELTA +N -M. Exitus 0 = nihil
#   novum, 1 = novum adest. Plagula extra ref (nova) = basis vacua -
#   omnia nova, '-scribere' eam. -contra = basis ex disco (probationi
#   et comparationibus manualibus).
#
# Regulae: project-specs/formator-regulae.md (nomina stabilia in
# fructu - divergentiae se nominant). -machina = TSV cum linea
# titulari '#' praefixa (consumptores awk '$1!~/^#/' filtrant).
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
    "-I$SILVA_DIR/instrumenta"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/formator_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" "$SILVA_DIR/instrumenta" -name '*.h' -newer "$1" 2>/dev/null | head -1
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

# moduli instrumentorum communes (machina lint)
for base in silva_formator; do
    src="$SILVA_DIR/instrumenta/$base.c"
    obj="$BUILD_DIR/instr_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [instr] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

FORMATOR_SRC="$SILVA_DIR/instrumenta/principalia/formator.c"
FORMATOR_BIN="$BUILD_DIR/formator"
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$FORMATOR_SRC" $obj_files \
    -o "$FORMATOR_BIN" || exit 1

cd "$RADIX_DIR"
# ---------------- -delta / -contra ----------------
DELTA_REF=""
CONTRA=""
declare -a VIAE=()
MACHINA=""
argv=("$@")
i=0
while [ $i -lt ${#argv[@]} ]; do
    a="${argv[$i]}"
    case "$a" in
        -delta)
            DELTA_REF="HEAD"
            j=$((i + 1))
            if [ $j -lt ${#argv[@]} ] && [[ "${argv[$j]}" != -* ]] && [ ! -e "${argv[$j]}" ]; then
                DELTA_REF="${argv[$j]}"; i=$j
            fi ;;
        -contra)
            i=$((i + 1)); CONTRA="${argv[$i]:-}" ;;
        -machina) MACHINA=1 ;;
        -*) ;;
        *) VIAE+=("$a") ;;
    esac
    i=$((i + 1))
done
if [ -z "$DELTA_REF" ] && [ -z "$CONTRA" ]; then
    exec "$FORMATOR_BIN" "$@"
fi
if [ ${#VIAE[@]} -eq 0 ]; then
    echo "usus: ./silva/formator.sh <via.c> -delta [ref] | -contra <basis>" >&2
    exit 2
fi
if [ -n "$CONTRA" ] && { [ ${#VIAE[@]} -ne 1 ] || [ ! -f "$CONTRA" ]; }; then
    echo "usus: -contra basim unam legibilem et viam unam poscit" >&2
    exit 2
fi
cd "$RADIX_DIR" || exit 2
mkdir -p "$BUILD_DIR/delta"

# claves: regula<TAB>contentum lineae (ex plagula ipsa), cum numero
_claves () {   # $1 = plagula (via ad formator), $2 = fons linearum
    "$FORMATOR_BIN" "$1" -machina 2>/dev/null | awk -F'\t' -v src="$2" '
        BEGIN { while ((getline l < src) > 0) { n++; L[n] = l } }
        $1 !~ /^#/ && $2 ~ /^[0-9]+$/ { print $4 "\t" L[$2] }'
}
novae_summa=0
for via in "${VIAE[@]}"; do
    rel="${via#"$RADIX_DIR"/}"
    rel="${rel#./}"
    if [ ! -f "$rel" ]; then
        echo "apparatus: plagula non exstat: $rel" >&2
        exit 2
    fi
    basis="$BUILD_DIR/delta/basis_$(basename "$rel")"
    nota_basis=""
    if [ -n "$CONTRA" ]; then
        cp "$CONTRA" "$basis"
    elif ! git show "$DELTA_REF:$rel" > "$basis" 2>/dev/null; then
        : > "$basis"
        nota_basis="(basis absens in $DELTA_REF - plagula nova: omnia nova; -scribere eam)"
    fi
    _claves "$basis" "$basis" | sort > "$BUILD_DIR/delta/b.txt"
    _claves "$rel" "$rel"     | sort > "$BUILD_DIR/delta/w.txt"
    n_basis=$(wc -l < "$BUILD_DIR/delta/b.txt" | tr -d ' ')
    n_work=$(wc -l < "$BUILD_DIR/delta/w.txt" | tr -d ' ')
    # multiset differentia per claves
    novae=$(comm -13 "$BUILD_DIR/delta/b.txt" "$BUILD_DIR/delta/w.txt")
    evanidae=$(comm -23 "$BUILD_DIR/delta/b.txt" "$BUILD_DIR/delta/w.txt")
    n_novae=$(printf '%s' "$novae" | grep -c . || true)
    n_evan=$(printf '%s' "$evanidae" | grep -c . || true)
    # awk BSD lineas novas in -v recusat ('newline in string') - per plagulam
    printf '%s\n' "$novae" > "$BUILD_DIR/delta/novae.txt"
    if [ -n "$MACHINA" ]; then
        # sedes novarum: lineae plagulae operis quarum clavis nova est
        "$FORMATOR_BIN" "$rel" -machina 2>/dev/null | awk -F'\t' -v src="$rel" -v nf="$BUILD_DIR/delta/novae.txt" '
            BEGIN { while ((getline l < src) > 0) { n++; L[n] = l }
                    while ((getline k < nf) > 0) if (k != "") NOVA[k] = 1 }
            $1 !~ /^#/ && $2 ~ /^[0-9]+$/ && (($4 "\t" L[$2]) in NOVA) { print $1 "\t" $2 "\t" $4 "\t" $7 }'
        printf 'DELTA\t%s\t+%s\t-%s\t%s\t%s\n' "$rel" "$n_novae" "$n_evan" "$n_basis" "$n_work"
    else
        echo "delta $rel: basis $n_basis -> nunc $n_work  (+$n_novae novae, -$n_evan evanidae) $nota_basis"
        if [ "$n_novae" -gt 0 ]; then
            "$FORMATOR_BIN" "$rel" -machina 2>/dev/null | awk -F'\t' -v src="$rel" -v nf="$BUILD_DIR/delta/novae.txt" '
                BEGIN { while ((getline l < src) > 0) { n++; L[n] = l }
                        while ((getline k < nf) > 0) if (k != "") NOVA[k] = 1 }
                $1 !~ /^#/ && $2 ~ /^[0-9]+$/ && (($4 "\t" L[$2]) in NOVA) { printf "  %s:%s: [%s] %s\n", $1, $2, $4, $7 }'
        fi
    fi
    novae_summa=$((novae_summa + n_novae))
done
[ "$novae_summa" -gt 0 ] && exit 1
exit 0
