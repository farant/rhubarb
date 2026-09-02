#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/silva.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ratio plena: compile_tests.sh radicis.
if [ -z "${SILVA_PROBATIONES_EFFUSIO:-}" ]; then
    export SILVA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/silva.log"
    exit "${PIPESTATUS[0]}"
fi

# silva/compile_probationes.sh - Probationes silvae compilare et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh lexema     # probationes quarum titulus "lexema" continet
#
# Mirrors the repo-root compile_tests.sh pattern (glob discovery, incremental
# objects, substring filter) but self-contained to silva/ and without the
# Cocoa/Security frameworks — silva is a pure library.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

# METRA SUITAE (tools/mensor_suitae.sh, 2026-09-02): forma eadem ac
# radix, tituli "silva." praefixati - ante hoc suita silvae nihil in
# mensorem scribebat, et quinque probationes corporis (XCVIII% temporis,
# ~XIII min) historiam nullam habebant. Numquam suitam frangit.
source "$RADIX_DIR/tools/mensor_suitae.sh"
mensor_suitae_incipere "silva."
RECOMPILATAE=0

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
    "-fbracket-depth=512"
)

declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
    "-I$SILVA_DIR/instrumenta"
    "-I$SILVA_DIR/probationes"
)

# Rhubarb lib sources silva depends on during development (vendoring happens
# only at amalgamation). credo is the test framework. stml + internamentum
# are ENGINE deps since silva_annotationes (2026-07-21, vendored at
# amalgamation like the rest); the generator (instrumenta) also uses stml.
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "tabula_dispersa"
    "friatio"
    "internamentum"
    "selectio"
    "stml"
    # stml_macros: expansio templorum in silva_arbor_legere_parsuram
    # (macros v1, 2026-08-26 - lector visionem contenti legit)
    "stml_macros"
    "credo"
    "differentia"
    "sigillum"
    # canon + similitudo: probatio_silva_canon canonem proiectum
    # (silva/c89.canon) contra canon.canon iudicat - schema
    # schematum schema generatum iudicans (arbor M2.3 / T5)
    "canon"
    "similitudo"
)

FILTER="${1:-}"

# NORMALIZATIO FILTRI (exemplar radicis): formae quas homo revera
# scribit ('silva/probationes/probatio_x.c' e completione TAB,
# '.c' appensum) ad substringam rediguntur - CLAMAT, non tacet.
if [ -n "$FILTER" ]; then
    FILTRUM_DATUM="$FILTER"
    FILTER="${FILTER##*/}"
    FILTER="${FILTER%.c}"
    if [ "$FILTER" != "$FILTRUM_DATUM" ]; then
        echo "compile_probationes: filtrum '$FILTRUM_DATUM' -> '$FILTER'" >&2
    fi
fi

# ---- 1. compile dependency objects (incremental) ----
# CAPUT RECENTISSIMUM SEMEL (exemplar radicis 2026-08-25): find
# per obiectum retiratum - aequivalentia: aliquod caput recentius
# obiecto <=> recentissimum recentius obiecto. Custos: nihil
# inventum = viae find pravae = custodia capitum MORTUA - CLAMAT.
CAPUT_RECENS=""
while IFS= read -r caput_via; do
    if [ -z "$CAPUT_RECENS" ] || [ "$caput_via" -nt "$CAPUT_RECENS" ]; then
        CAPUT_RECENS="$caput_via"
    fi
done < <(find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' 2>/dev/null)
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
        echo "  [dep] $f.c"; RECOMPILATAE=1
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $f.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 2. compile silva's own sources (fontes/*.c + instrumenta/*.c) ----
shopt -s nullglob
for src in "$SILVA_DIR"/fontes/*.c "$SILVA_DIR"/instrumenta/*.c; do
    base="$(basename "$src" .c)"
    # nexus_ordines est mundi AMALGAMATIS (silva.h includit; communis
    # cum officina/legatus) — contra capita fontium non compilat.
    if [ "$base" = "nexus_ordines" ]; then continue; fi
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c"; RECOMPILATAE=1
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 2b. adiumenta probationum (probationes/*.c non probatio_*) ----
# Porta apparatus BIS incendit et utroque numerum peperit qui datum
# simulabat; exemplar eius SECUNDUM in probatione altera tertiam
# occasionem erroris INDEPENDENTIS daret. Ergo unum exemplar hic
# compilatur et in probationes omnes nectitur.
for src in "$SILVA_DIR"/probationes/*.c; do
    base="$(basename "$src" .c)"
    case "$base" in probatio_*) continue ;; esac
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [adiumentum] $base.c"; RECOMPILATAE=1
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 3. discover, compile, run probationes ----
total=0 ; passed=0 ; failed_names=""
for test_file in "$SILVA_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    total=$((total + 1))
    bin="$BUILD_DIR/$name"
    echo ""
    echo "=== $name ==="
    t0=$(mensor_suitae_nunc)
    if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$test_file" $obj_files -o "$bin"; then
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
echo "SILVA PROBATIONES: $passed/$total praeteritae"
if [ "$total" -gt 0 ]; then
    fractae=$(echo $failed_names | wc -w | tr -d ' ')
    mensor_suitae_finire "" "$total" "$fractae" "$RECOMPILATAE"
fi
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi

# NULLA probatio cursa = error operantis (filtrum male
# scriptum), NON successus - exitus II ut radix (01M0XDNTN6;
# contractus 0/1/2, vide compile_tests.sh radicis).
if [ "$total" -eq 0 ]; then
    echo "NULLA PROBATIO CURSA (filtrum sine paribus?) - exitus II"
    exit 2
fi

# canalis excubitoris: verdictum post-constructionem (tacet nisi stala)
"$RADIX_DIR/excubitor.sh" -tacitus "silva/build/" >&2 || true
exit 0
