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

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

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

# ---- 3. probationes: PARALLELAE (FILA operarii), collectio ordinata ----
#
# 2026-09-02: suita seriatim ~240 s, quinque probationes corporis
# XCVII% temporis - parallelae, tempus muri = probatio tardissima.
# Operarius (probatio_una, exportatus) compilat + nectit + currit
# probationem UNAM, effusum in singulae/<nomen>.log, verdictum et
# tempora in <nomen>.res; parens post omnes acta ORDINE nominum
# imprimit (acta legibilia, deterministica) et tabulas mensoris implet.
# Nihil inter operarios commune: obiecta bibliothecae iam recentia
# (gradus 1-2b supra, seriatim), binarium et .o per nomen propria.
# PROBATIONES_FILA=N (ordinarius: nuclei PERFORMANTES - mensura
# 2026-09-02: VIII fila (IV perf + IV eff) murus 142 s, IV fila 96 s;
# probationes corporis in nucleis efficientibus bis tardiores et
# latitudo memoriae contenditur); =1 seriatim.
FILA="${PROBATIONES_FILA:-$(sysctl -n hw.perflevel0.physicalcpu 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)}"
SINGULAE="$BUILD_DIR/singulae"
rm -rf "$SINGULAE"; mkdir -p "$SINGULAE"
OBIECTA="$BUILD_DIR/obiecta_probationum.txt"
echo "$obj_files" > "$OBIECTA"
export SILVA_DIR RADIX_DIR BUILD_DIR SINGULAE OBIECTA

probatio_una () {   # <nomen> - in operario (bash filio); ambitus exportatus
    local name="$1"
    local test_file="$SILVA_DIR/probationes/$1.c"
    local bin="$BUILD_DIR/$1"
    local t0 t1 tc tr rc obj_files
    local -a INC
    source "$RADIX_DIR/tools/vexilla.sh"
    INC=("-I$RADIX_DIR/include" "-I$SILVA_DIR/fontes" "-I$SILVA_DIR/instrumenta" "-I$SILVA_DIR/probationes")
    obj_files="$(cat "$OBIECTA")"
    t0=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    # compilatio et nexus seorsum (dsymutil vitatur - vide radicem)
    if ! clang "${VEXILLA_C89[@]}" "${INC[@]}" -c "$test_file" -o "$bin.o" > "$SINGULAE/$name.log" 2>&1 \
       || ! clang "${VEXILLA_C89[@]}" "$bin.o" $obj_files -o "$bin" >> "$SINGULAE/$name.log" 2>&1; then
        echo "FRACTA (compilatio): $name" >> "$SINGULAE/$name.log"
        echo "2 0 0" > "$SINGULAE/$name.res"
        return 0
    fi
    t1=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    tc=$(echo "$t1 - $t0" | bc); case "$tc" in .*) tc="0$tc" ;; esac
    t0=$t1
    RHUBARB_RADIX="$RADIX_DIR" "$bin" >> "$SINGULAE/$name.log" 2>&1
    rc=$?
    t1=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    tr=$(echo "$t1 - $t0" | bc); case "$tr" in .*) tr="0$tr" ;; esac
    echo "$rc $tc $tr" > "$SINGULAE/$name.res"
    return 0
}
export -f probatio_una

lista=""
for test_file in "$SILVA_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    lista="$lista $name"
done
shopt -u nullglob
total=0 ; passed=0 ; failed_names="" ; MURUS=""
if [ -n "${lista// /}" ]; then
    echo ""
    echo "probationes parallelae: $(echo $lista | wc -w | tr -d ' ') per $FILA fila"
    MURUS_T0=$(mensor_suitae_nunc)
    printf '%s\n' $lista | xargs -P "$FILA" -n 1 -I{} bash -c 'probatio_una "$1"' _ {}
    _mensor_suitae_duratio "$MURUS_T0"; MURUS="$MSU_ULTIMA"
    echo "murus fascis: ${MURUS}s"
fi
for name in $lista; do
    total=$((total + 1))
    echo ""
    echo "=== $name ==="
    cat "$SINGULAE/$name.log" 2>/dev/null
    if [ ! -f "$SINGULAE/$name.res" ]; then
        echo "--- $name SINE VERDICTO (operarius periit?)"
        failed_names="$failed_names $name"
        continue
    fi
    read -r rc tc tr < "$SINGULAE/$name.res"
    if [ "$rc" = "2" ] && [ "$tc" = "0" ]; then
        failed_names="$failed_names $name"
        continue
    fi
    mensor_suitae_compilatio_secunda "$name" "$tc"
    mensor_suitae_cursus_secunda "$name" "$tr"
    if [ "$rc" = "0" ]; then
        echo "--- $name praeteriit (${tr}s)"
        passed=$((passed + 1))
    else
        echo "--- $name FRACTA (${tr}s, exitus $rc)"
        failed_names="$failed_names $name"
    fi
done

echo ""
mensor_suitae_tardissimae 5
echo "========================================"
echo "SILVA PROBATIONES: $passed/$total praeteritae"
if [ "$total" -gt 0 ]; then
    fractae=$(echo $failed_names | wc -w | tr -d ' ')
    mensor_suitae_finire "" "$total" "$fractae" "$RECOMPILATAE" "$FILA" "$MURUS"
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
