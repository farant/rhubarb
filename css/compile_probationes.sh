#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/css.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
if [ -z "${CSS_PROBATIONES_EFFUSIO:-}" ]; then
    export CSS_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/css.log"
    exit "${PIPESTATUS[0]}"
fi

# materia/compile_probationes.sh - Probationes materiae
#
# Usage:
#   ./compile_probationes.sh            # omnes
#   ./compile_probationes.sh lexicon    # quarum titulus "lexicon" continet
#
# Exemplar: silva/compile_probationes.sh. Contractus exitus idem -
# 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum pravum).

set -u

CSS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MATERIA_DIR="$(cd "$CSS_DIR/../materia" && pwd)"
RADIX_DIR="$(cd "$CSS_DIR/.." && pwd)"
BUILD_DIR="$CSS_DIR/build"
# SERA cursoris (tools/sera.sh, 2026-09-07): cursus duo eiusdem suitae
# obiecta eadem scriberent, binarium currens obtererent (137);
# exspectatio ad SERA_TECTUM (DC s), deinde exitus 2 = NIHIL cucurrit
# (tenens nominatur). Involucra et pythonica reentrant (SERA_TENTA).
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$MATERIA_DIR/fontes"
    "-I$CSS_DIR/fontes"
    "-I$CSS_DIR/probationes"
)

FILTER="${1:-}"

if [ -n "$FILTER" ]; then
    FILTRUM_DATUM="$FILTER"
    FILTER="${FILTER##*/}"
    FILTER="${FILTER%.c}"
    if [ "$FILTER" != "$FILTRUM_DATUM" ]; then
        echo "compile_probationes: filtrum '$FILTRUM_DATUM' -> '$FILTER'" >&2
    fi
fi

# ---- FONTES ex clausuris (migratio ad compilatorem, cursor communis) ----
# Olim indices MANU scripti (XIX lib, XIII materia, css/fontes, adiumenta)
# et regula crassa 'caput recentissimum quodvis omne obiectum stalum facit'
# (mtime), deinde custodia excubitoris (mtime). Nunc tools/cursor_communis.sh:
# clausura cuiusque probationis ab aedile, unio per bin/compilator,
# probatio quaeque cum clausura SUA nexa (exemplar toml, T5).
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare || exit 1
cursor_clausuras_derivare "$CSS_DIR/probationes" || exit 1
cursor_fontes_compilare || exit 1
shopt -s nullglob

# metra suitae in volumen mensoris (tools/mensor_suitae.sh; praefixum
# "css." - silva.mensurae('css.', n) eas legit); numquam suitam frangit
source "$RADIX_DIR/tools/mensor_suitae.sh"
mensor_suitae_incipere "css."
total=0 ; passed=0 ; failed_names=""
for test_file in "$CSS_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    total=$((total + 1))
    bin="$BUILD_DIR/$name"
    echo ""
    echo "=== $name ==="
    t0=$(mensor_suitae_nunc)
    if ! cursor_probationem_struere "$test_file" "$bin"; then
        echo "FRACTA (compilatio): $name"
        failed_names="$failed_names $name"
        continue
    fi
    mensor_suitae_compilatio "$name" "$t0"
    t0=$(mensor_suitae_nunc)
    if RHUBARB_RADIX="$RADIX_DIR" "$bin"; then
        mensor_suitae_cursus "$name" "$t0"
        echo "--- $name praeteriit (${MSU_ULTIMA}s)"
        passed=$((passed + 1))
    else
        mensor_suitae_cursus "$name" "$t0"
        echo "--- $name FRACTA (${MSU_ULTIMA}s)"
        failed_names="$failed_names $name"
    fi
done
shopt -u nullglob

echo ""
mensor_suitae_tardissimae 5
echo "========================================"
echo "CSS PROBATIONES: $passed/$total praeteritae"
mensor_suitae_finire "" "$total" "$(echo $failed_names | wc -w | tr -d ' ')" 0
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi
if [ "$total" -eq 0 ]; then
    echo "NULLA PROBATIO CURSA (filtrum sine paribus?) - exitus II"
    exit 2
fi
exit 0
