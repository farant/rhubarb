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
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89"
    "-pedantic"
    "-Wall"
    "-Wextra"
    "-Werror"
    "-Wconversion"
    "-Wsign-conversion"
    "-Wcast-qual"
    "-Wstrict-prototypes"
    "-Wmissing-prototypes"
    "-Wwrite-strings"
    "-Wno-long-long"
    "-Wno-overlength-strings"
)

declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$MATERIA_DIR/fontes"
    "-I$CSS_DIR/fontes"
    "-I$CSS_DIR/probationes"
)

# Fontes radicis quibus materia in evolutione nititur.
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "friatio"
    "tabula_dispersa"
    "internamentum"
    "css_lexema"
    "selectio"
    "stml"
    "stml_macros"
    "similitudo"
    "canon"
    "credo"
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

# ---- custodia capitum ----
# Mutatio capitis SOLA nihil recompilat si tantum mtimes .c
# spectantur - id est ipsum quod falsum viride 2026-08-27 in silva
# peperit (L/L contra obiecta sex horarum). Ergo caput
# recentissimum contra omne obiectum retiratur.
CAPUT_RECENS=""
while IFS= read -r caput_via; do
    if [ -z "$CAPUT_RECENS" ] || [ "$caput_via" -nt "$CAPUT_RECENS" ]; then
        CAPUT_RECENS="$caput_via"
    fi
done < <(find "$RADIX_DIR/include" "$CSS_DIR/fontes" "$CSS_DIR/probationes" \
             -name "*.h" 2>/dev/null)
if [ -z "$CAPUT_RECENS" ]; then
    echo "CAUTIO: nullum caput inventum (viae find pravae?) - custodia recompilationis capitum MORTUA" >&2
fi

# AEQUALITAS: mtimes secundo mensurantur, ergo fons et obiectum
# EODEM SECUNDO scripta '-nt' non satisfaciunt et recompilatio
# TACITE omittitur - suite contra obiectum PRIUS currit. Id
# 2026-08-27 in materia RUBRUM FALSUM peperit (vitium plantatum,
# restitutum, probatio adhuc rubra contra .o vetus; .c et .o ambo
# 21:27:07). Remedium ubique: in aequalitate RECOMPILA.
newest_header () {
    if [ -n "$CAPUT_RECENS" ] && ! [ "$1" -nt "$CAPUT_RECENS" ]; then
        echo "$CAPUT_RECENS"
    fi
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $f.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

shopt -s nullglob
# materia sub-fontes: css eam CONSUMIT, non continet
for m in materia_lexicon materia_token materia_nodus materia_scribere \
         materia_arbor materia_arbor_aequalitas; do
    src="$MATERIA_DIR/fontes/$m.c"
    obj="$BUILD_DIR/$m.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [materia] $m.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $m.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

for src in "$CSS_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [css] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# adiumenta probationum (probationes/*.c non probatio_*), e.g.
# lexicon_c89.c - descriptor GENERATUS, vehiculum portae phasis I
for src in "$CSS_DIR"/probationes/*.c; do
    base="$(basename "$src" .c)"
    case "$base" in probatio_*) continue ;; esac
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [adiumentum] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

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
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$test_file" $obj_files -o "$bin"; then
        echo "FRACTA (compilatio): $name"
        failed_names="$failed_names $name"
        continue
    fi
    if RHUBARB_RADIX="$RADIX_DIR" "$bin"; then
        passed=$((passed + 1))
    else
        failed_names="$failed_names $name"
    fi
done
shopt -u nullglob

echo ""
echo "========================================"
echo "CSS PROBATIONES: $passed/$total praeteritae"
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi
if [ "$total" -eq 0 ]; then
    echo "NULLA PROBATIO CURSA (filtrum sine paribus?) - exitus II"
    exit 2
fi
exit 0
