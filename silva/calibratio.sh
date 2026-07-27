#!/bin/bash

# silva/calibratio.sh - cursor speciminum oraculi (01KXTY10JF)
#
# Lex calibrationis oraculi: specimina ANTE praedicatum. Fistula
# manu voluta quinquies (conversio-signi, fluxus-1) hic instrumentum
# facta est: per specimen x per copiam vexillorum clang -fsyntax-only
# curritur, tabula compendii imprimitur, acta plena servantur.
#
# Usage:
#   ./silva/calibratio.sh <directorium|specimen.c> [-vexilla "-W..."]...
#
# Specimina: *.c et fixturae examinis (.invalidum/.domesticum/
# .suspectum/.severum) sub directorio (non recursivum), aut plagula
# una directa. Copiae vexillorum: quaeque optio -vexilla copia una
# (chorda spatiis divisa); sine -vexilla copiae ordinariae infra.
# Basis = oraculum vectis (clang -x c -std=c89 -pedantic
# -Wno-long-long -fno-caret-diagnostics -fsyntax-only).
#
# Cella tabulae: numerus monitorum; '!' appensum = errores quoque.
# Lineae monitorum sub tabula per cellam non-vacuam; acta plena in
# silva/build/calibratio/<specimen>.<copia>.log
#
# Exit: 0 (instrumentum oculorum, non porta) | 2 usus pravus.
# NB: status clang INTUS captus, numquam per fistulam (laqueus
# codicis-exitus-per-fistulam semel pro semper hic sepultus).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
ACTA_DIR="$SILVA_DIR/build/calibratio"

FONS=""
declare -a COPIAE=()
while [ $# -gt 0 ]; do
    case "$1" in
        -vexilla)
            [ $# -lt 2 ] && { echo "calibratio: -vexilla sine argumento" >&2; exit 2; }
            COPIAE+=("$2"); shift 2 ;;
        -*)
            echo "calibratio: optio ignota $1" >&2; exit 2 ;;
        *)
            [ -n "$FONS" ] && { echo "calibratio: fons unus solum" >&2; exit 2; }
            FONS="$1"; shift ;;
    esac
done
[ -z "$FONS" ] && {
    echo "usus: calibratio.sh <directorium|specimen.c> [-vexilla \"-W...\"]..." >&2
    exit 2
}

if [ ${#COPIAE[@]} -eq 0 ]; then
    COPIAE=("-Wsign-conversion" "-Wtype-limits" "-Wtautological-compare" "-Wextra")
fi

declare -a BASIS=(
    clang -x c -std=c89 -pedantic -Wno-long-long
    -fno-caret-diagnostics -fsyntax-only
)

declare -a SPECIMINA=()
if [ -f "$FONS" ]; then
    SPECIMINA=("$FONS")
elif [ -d "$FONS" ]; then
    while IFS= read -r f; do
        SPECIMINA+=("$f")
    done < <(find "$FONS" -maxdepth 1 -type f \
        \( -name '*.c' -o -name '*.invalidum' -o -name '*.domesticum' \
           -o -name '*.suspectum' -o -name '*.severum' \) \
        | LC_ALL=C sort)
else
    echo "calibratio: $FONS nec plagula nec directorium" >&2
    exit 2
fi
[ ${#SPECIMINA[@]} -eq 0 ] && {
    echo "calibratio: nulla specimina sub $FONS" >&2
    exit 2
}

mkdir -p "$ACTA_DIR"
rm -f "$ACTA_DIR"/*.log 2>/dev/null

echo "CALIBRATIO  (specimina ${#SPECIMINA[@]} x copiae ${#COPIAE[@]})"
for i in "${!COPIAE[@]}"; do
    echo "  copia $((i + 1)): ${COPIAE[$i]}"
done
echo ""

# tabula: caput
printf '%-36s' "specimen"
for i in "${!COPIAE[@]}"; do
    printf ' %8s' "copia $((i + 1))"
done
printf '\n'

DETALIA=""
for f in "${SPECIMINA[@]}"; do
    basis_f="$(basename "$f")"
    printf '%-36s' "$basis_f"
    for i in "${!COPIAE[@]}"; do
        # shellcheck disable=SC2086 (copia consulto divisa)
        effusum="$("${BASIS[@]}" ${COPIAE[$i]} "$f" 2>&1)"
        status=$?
        log="$ACTA_DIR/${basis_f}.copia$((i + 1)).log"
        printf '%s\n' "$effusum" > "$log"
        n_mon="$(printf '%s\n' "$effusum" | grep -c 'warning:' || true)"
        n_err="$(printf '%s\n' "$effusum" | grep -c 'error:' || true)"
        cella="$n_mon"
        [ "$n_err" -gt 0 ] && cella="${n_mon}!"
        [ "$status" -ne 0 ] && [ "$n_err" -eq 0 ] && cella="${n_mon}?"
        printf ' %8s' "$cella"
        if [ "$n_mon" -gt 0 ]; then
            lineae="$(printf '%s\n' "$effusum" \
                | sed -n 's/^[^:]*:\([0-9]*\):[0-9]*: warning:.*/\1/p' \
                | paste -sd ',' -)"
            DETALIA="$DETALIA$basis_f copia $((i + 1)): lineae $lineae
"
        fi
    done
    printf '\n'
done

if [ -n "$DETALIA" ]; then
    echo ""
    echo "monita per cellam:"
    printf '%s' "$DETALIA" | sed 's/^/  /'
fi
echo ""
echo "acta plena: $ACTA_DIR/  ('!' = errores; '?' = exitus non-zerus sine errore)"
exit 0
