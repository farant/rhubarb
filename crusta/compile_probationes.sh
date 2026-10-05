#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/crusta.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
if [ -z "${CRUSTA_PROBATIONES_EFFUSIO:-}" ]; then
    export CRUSTA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/crusta.log"
    exit "${PIPESTATUS[0]}"
fi

# crusta/compile_probationes.sh - Probationes clientis crusta (materia)
#
# Usage:
#   ./compile_probationes.sh            # omnes
#   ./compile_probationes.sh lexicon    # quarum titulus "lexicon" continet
#
# Exemplar: silva/compile_probationes.sh. Contractus exitus idem -
# 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum pravum).

set -u

CRUSTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MATERIA_DIR="$(cd "$CRUSTA_DIR/../materia" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
BUILD_DIR="$CRUSTA_DIR/build"
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
    "-I$CRUSTA_DIR/fontes"
    "-I$CRUSTA_DIR/probationes"
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

# ---- custodia vetustatis ----
# '$src -nt $obj' AEQUALITATEM PERDIT: mtimes secundo mensurantur,
# ergo fons et obiectum in EODEM secundo scripta 'non novius'
# dant et recompilatio TACITE OMITTITUR. Id me momordit
# 2026-08-27: vitium plantatum, restitutum, et probatio adhuc
# rubra - contra obiectum vitiosum, quia .c et .o ambo 21:27:07
# ferebant. FALSUM RUBRUM, geminum falsi viridis quod eodem die
# silvam momordit, et peius: viride falsum te 'peractum' putare
# facit, rubrum falsum codici RECTO diffidere.
# Remedium: '! [ $obj -nt $src ]' - in aequalitate RECOMPILA.
# CAUTIO: silva/compile_probationes.sh idem exemplar '-nt' fert.

# ---- FONTES ex clausuris (migratio ad compilatorem, cursor communis) ----
# Olim indices MANU scripti (lib, XIII materia, crusta/fontes, adiumenta),
# regula crassa 'caput recentissimum' (mtime) et custodia excubitoris
# (mtime). Nunc tools/cursor_communis.sh: clausura cuiusque probationis
# ab aedile, unio per bin/compilator, probatio quaeque cum clausura SUA.
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare || exit 1
cursor_clausuras_derivare "$CRUSTA_DIR/probationes" || exit 1
cursor_fontes_compilare || exit 1
shopt -s nullglob

mkdir -p "$RADIX_DIR/build"
(cd "$RADIX_DIR" && git ls-files '*.sh') > "$RADIX_DIR/build/crusta_corpus.lst"

# metra suitae in volumen mensoris (tools/mensor_suitae.sh; praefixum
# "crusta." - silva.mensurae('crusta.', n) eas legit); numquam suitam frangit
source "$RADIX_DIR/tools/mensor_suitae.sh"
mensor_suitae_incipere "crusta."
total=0 ; passed=0 ; failed_names=""
for test_file in "$CRUSTA_DIR"/probationes/probatio_*.c; do
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
echo "CRUSTA PROBATIONES: $passed/$total praeteritae"
mensor_suitae_finire "" "$total" "$(echo $failed_names | wc -w | tr -d ' ')" 0
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi
if [ "$total" -eq 0 ]; then
    echo "NULLA PROBATIO CURSA (filtrum sine paribus?) - exitus II"
    exit 2
fi

# PORTA REGENS gradus II super corpore domus: extractor C contra AURUM
# quod extractor pythonicus ultimo dixit (vide
# probationes/fixa/exemplaria/README.md). HIC CURRIT, non manu: porta
# quam nemo currit porta non est, et haec per opera III manu sola
# cucurrit - quod ipsum est quod domus 'porta muta = porta mortua'
# nominat. Cum filtro omittitur (cursus angustus eam non poscit).
if [ -z "${FILTRUM_DATUM:-}" ]; then
    echo ""
    if ! "$CRUSTA_DIR/instrumenta/differentia_exemplariorum.sh"; then
        echo "CRUSTA: differentia exemplariorum FRACTA"
        exit 1
    fi
fi
exit 0
