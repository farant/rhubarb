#!/bin/bash

# silva/differentia.sh - iudicium nostrum contra oraculum clang,
# una plagula (instrumentum OCULORUM pro gradibus oraculo-comparibus,
# non porta - portae = examen_vectis.sh)
#
# Natum ex arcu conversionis signi 2026-07-16: circulus
# examen-contra-clang quinquies manu constructus, semel PRAVE
# (stderr oraculi in /dev/null missum -> tabula zerorum confidens).
# Hic scriptus redirectionem SEMEL recte fert.
#
# Usage:
#   ./silva/differentia.sh <plagula.c> [-posix]
# Effusum: per gradum tres situlae -
#   CONSENSUS (ambo flagrant) / NOS SOLI (falsa positiva suspecta) /
#   ORACULUM SOLUM (omissiones nostrae aut limites nominati)
# Exit: 0 consensus plenus | 1 discrepantia | 2 apparatus

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
cd "$RADIX_DIR"

if [ $# -lt 1 ]; then
    echo "usus: ./silva/differentia.sh <plagula.c> [-posix]" >&2
    exit 2
fi
PLAGULA="$1"
shift
POSIX=""
[ "${1:-}" = "-posix" ] && POSIX="-posix"

if [ ! -f "$PLAGULA" ]; then
    echo "apparatus: plagula non exstat: $PLAGULA" >&2
    exit 2
fi

# tabula graduum oraculo-comparium: codex <TAB> vexilla clang
# <TAB> titulus vexilli (exemplar in effuso oraculi "[-W...]").
# Gradus novus oraculo-compar: ordinem adde.
GRADUS_TABULA="54	-Wsign-conversion	-Wsign-conversion
57	-Wsign-compare	-Wsign-compare"

# ---- latus nostrum (per involucrum - SEMPER recens; binarium
# directum numquam: decipula binarii vetusti) ----
effusum="$(./silva/examen.sh "$PLAGULA" -machina $POSIX 2>/dev/null)"
if [ -z "$effusum" ]; then
    echo "apparatus: examen nihil effudit (aedificatio fracta?)" >&2
    ./silva/examen.sh "$PLAGULA" -machina $POSIX 2>&1 >/dev/null \
        | tail -5 >&2
    exit 2
fi

# ---- latus oraculi (NB: monita clang in STDERR - 2>&1!) ----
declare -a ORACULUM_BASIS=(
    clang -x c -std=c89 -pedantic
    -Wno-long-long -Wno-overlength-strings
    -fno-caret-diagnostics -fsyntax-only
    -I include -I lib -I silva/fontes
    -I officina/fontes -I officina/instrumenta
)
vexilla_omnia="$(printf '%s\n' "$GRADUS_TABULA" | cut -f2 | tr '\n' ' ')"
# shellcheck disable=SC2086
monita="$("${ORACULUM_BASIS[@]}" $vexilla_omnia "$PLAGULA" 2>&1)"
if printf '%s\n' "$monita" | grep -q "error:"; then
    echo "CAUTIO: oraculum errores compilationis dedit (verdictum" \
         "eius incertum):" >&2
    printf '%s\n' "$monita" | grep "error:" | head -3 >&2
fi

echo "differentia: $PLAGULA"
echo "oraculum:    $(clang --version | head -1)"
discrepantiae=0

while IFS=$'\t' read -r codex vexillum titulus; do
    [ -z "$codex" ] && continue
    echo ""
    echo "--- gradus codex $codex ($vexillum) ---"

    nos_lineae="$(printf '%s\n' "$effusum" | awk -F'\t' -v c="$codex" \
        '$5==c {print $2}' | sort -un)"
    orc_lineae="$(printf '%s\n' "$monita" | awk -F: -v p="$PLAGULA" \
        -v t="[$titulus]" 'index($0, t) && $1==p {print $2}' \
        | sort -un)"

    consensus="$(comm -12 <(printf '%s\n' "$nos_lineae" | grep . || true) \
                          <(printf '%s\n' "$orc_lineae" | grep . || true))"
    nos_soli="$(comm -23 <(printf '%s\n' "$nos_lineae" | grep . || true) \
                         <(printf '%s\n' "$orc_lineae" | grep . || true))"
    orc_solum="$(comm -13 <(printf '%s\n' "$nos_lineae" | grep . || true) \
                          <(printf '%s\n' "$orc_lineae" | grep . || true))"

    for l in $consensus; do
        causa="$(printf '%s\n' "$effusum" | awk -F'\t' -v c="$codex" \
            -v l="$l" '$5==c && $2==l {print $7; exit}')"
        echo "  CONSENSUS       $PLAGULA:$l  $causa"
    done
    for l in $nos_soli; do
        causa="$(printf '%s\n' "$effusum" | awk -F'\t' -v c="$codex" \
            -v l="$l" '$5==c && $2==l {print $7; exit}')"
        echo "  NOS SOLI        $PLAGULA:$l  $causa   <- falsum positivum suspectum"
        discrepantiae=$((discrepantiae + 1))
    done
    for l in $orc_solum; do
        nuntius="$(printf '%s\n' "$monita" | awk -F: -v p="$PLAGULA" \
            -v l="$l" '$1==p && $2==l {
                i = index($0, "warning: ");
                print (i > 0) ? substr($0, i + 9) : $0; exit}')"
        echo "  ORACULUM SOLUM  $PLAGULA:$l  $nuntius   <- omissio aut limes nominatus"
        discrepantiae=$((discrepantiae + 1))
    done
    n_c="$(printf '%s' "$consensus" | grep -c . || true)"
    echo "  (consensus $n_c)"
done <<< "$GRADUS_TABULA"

echo ""
if [ "$discrepantiae" -eq 0 ]; then
    echo "DIFFERENTIA: consensus plenus"
    exit 0
fi
echo "DIFFERENTIA: $discrepantiae discrepantiae"
exit 1
