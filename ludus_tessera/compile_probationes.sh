#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/ludus_tessera.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# Ratio plena: compile_tests.sh radicis.
if [ -z "${LUDUS_TESSERA_PROBATIONES_EFFUSIO:-}" ]; then
    export LUDUS_TESSERA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/ludus_tessera.log"
    exit "${PIPESTATUS[0]}"
fi

# ludus_tessera/compile_probationes.sh - Probationes glutini tesserae
# (modulus 013; project-specs/ludus-tessera-plan.md) compilare et
# currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh fumus      # substring filter
#
# Exemplar saltuarii, sed tessera per FONTES (A2, 2026-10-03), ut
# musivum, folium, effigies: ludus_tessera cliens monorepositorii est,
# non hospes amalgamatis (saltuarius id probat). Pars pura musivi
# (musivum_pingere: cellulae tessellationis -> tessera) iterum
# adhibetur. API publica tesserae = mos, non clavus. ludus
# (dispensator, figurae, tessellatio, rivus, terminalis) ex lib/.
# Suita sine capite: pons memoriae; terminalis verus solum aspectu
# Frani probatur.

set -u

LT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$LT_DIR/.." && pwd)"
BUILD_DIR="$LT_DIR/build"
# SERA cursoris (tools/sera.sh): cursus duo eiusdem suitae obiecta
# eadem scriberent; exspectatio ad SERA_TECTUM, deinde exitus 2.
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2

# vexilla: tools/vexilla.sh (una sedes)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$RADIX_DIR/tessera/fontes"
    "-I$RADIX_DIR/tessera/instrumenta/musivum"
    "-I$LT_DIR/fontes"
    "-I$LT_DIR/probationes"
    # A3: ludus_toy.h (applicatio ludicra probationum radicis)
    "-I$RADIX_DIR/probationes"
)

# Bibliothecae radicis (lista manualis ut tessera et saltuarius):
# clausura tesserae (fontes) + tessellatio + terminalis (pons, A1)
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "credo"
    "xar"
    "utf8"
    "runae"
    "runae_tabulae"
    "series_terminalis"
    "eventus_cauda"
    "claves_physicae"
    "interpres_terminalis"
    "rivus_terminalis"
    "stilus_terminalis"
    "terminalis_posix"
    "quadrans"
    "dithering"
    "imago"
    "imago_opus"
    "color"
    "delineare"
    "exemplaria"
    "delineare_mandata"
    "fenestra_textus"
    "fons"
    "friatio"
    "internamentum"
    "mandatum"
    "modulus"
    "selectio"
    "stml"
    "tabula_dispersa"
    "tessellatio"
    "thema"
    # A3: ludus (dispensator et clausura eius) + horologium
    "actio"
    "componens"
    "dispensator"
    "figura"
    "insula"
    "motus"
    "derivare"
    "destinatio"
    "fenestra_tempus_macos"
    "canon"
    "stml_macros"
    "similitudo"
    # A4: pictor (compositio communis + documentum in volumine) et
    # iteratio per terminalem (codificator, manus)
    "pictor_applicatio"
    "pictor_actiones"
    "pictor_componentia"
    "pictor_documentum"
    "flatura"          # pictor-strata L2: checkpoints deflati
    # scriba H2: cauda documenti per historia
    "historia"
    "pictor_figurae"
    # B3: pictor_componere disponit per dispositio
    "dispositio"
    # scriba S3: applicatio scribae (pagina vim, documentum textus)
    "scriba_applicatio"
    "scriba_actiones"
    "scriba_componentia"
    "scriba_documentum"
    # vicus-latera S2b: liber paginarum scribae
    "scriba_liber"
    "iussum"
    "scriba_figurae"
    "vim"
    "tabula_characterum"
    "volumen"
    "filum"
    "sigillum"
    "lectiones"
    "json"
    "fasti"
    "moneta"
    "scrinium"
    "codificator_terminalis"
    "manus_ludus"
    # insula-rami T4: hospes (vicus) et compositio eius
    "vicus"
    "vicus_applicatio"
    # aemulator A3: reditus (tessera -> octeti -> aemulator)
    "aemulator"
    "sors"
    # vicus-latera S1c: terminale genus vici (tabula t1 ordinaria)
    "terminale"
    "aemulator_hospes"
    "pseudoterminale"
    "pseudoterminale_posix"
    "glyphae_ductae"
)

FILTER="${1:-}"

# NORMALIZATIO FILTRI (exemplar radicis): 'x/probatio_y.c' -> 'probatio_y'
if [ -n "$FILTER" ]; then
    FILTRUM_DATUM="$FILTER"
    FILTER="${FILTER##*/}"
    FILTER="${FILTER%.c}"
    if [ "$FILTER" != "$FILTRUM_DATUM" ]; then
        echo "compile_probationes: filtrum '$FILTRUM_DATUM' -> '$FILTER'" >&2
    fi
fi

# CAPUT RECENTISSIMUM SEMEL (exemplar radicis): custos - nihil inventum
# = viae find pravae = custodia capitum MORTUA - CLAMAT.
CAPUT_RECENS=""
while IFS= read -r caput_via; do
    # <tolera codex="lint:nt-aequalitas" (>maximum quaeritur, non comparatio duorum: in aequalitate utrumlibet aeque valet
    if [ -z "$CAPUT_RECENS" ] || [ "$caput_via" -nt "$CAPUT_RECENS" ]; then
        CAPUT_RECENS="$caput_via"
    fi
done < <(find "$RADIX_DIR/include" "$LT_DIR/fontes" "$RADIX_DIR/tessera/fontes" "$RADIX_DIR/tessera/instrumenta/musivum" -name '*.h' 2>/dev/null)
if [ -z "$CAPUT_RECENS" ]; then
    echo "CAUTIO: nullum caput inventum (viae find pravae?) - custodia recompilationis capitum MORTUA" >&2
fi

newest_header () {
    if [ -n "$CAPUT_RECENS" ] && ! [ "$1" -nt "$CAPUT_RECENS" ]; then
        echo "$CAPUT_RECENS"
    fi
}

# ---- 1. dependency objects (incremental) ----
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

# ---- 2. tessera (fontes) + pars pura musivi ----
for src in "$RADIX_DIR"/tessera/fontes/*.c \
           "$RADIX_DIR/tessera/instrumenta/musivum/musivum_pictura.c"; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/tessera_$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [tessera] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 2b. vendicata (A4: volumen -> scrinium -> sqlite): obiectum
#          RELAXATUM, vexilla eadem ac compile_tests.sh radicis ----
src="$RADIX_DIR/vendor/sqlite3.c"
obj="$BUILD_DIR/vendor_sqlite3.o"
if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
    echo "  [vendicatum] sqlite3.c"
    if ! clang -O2 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0 \
            -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 \
            -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_OMIT_DEPRECATED \
            -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1 -c "$src" -o "$obj"; then
        echo "FRACTA: sqlite3.c" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# ---- 3. fontes ludus_tessera ----
shopt -s nullglob
for src in "$LT_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [ludus_tessera] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# ---- 4. discover, compile, run probationes ----
total=0 ; passed=0 ; failed_names=""
for test_file in "$LT_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    total=$((total + 1))
    bin="$BUILD_DIR/$name"
    echo ""
    echo "=== $name ==="
    # shellcheck disable=SC2086
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
echo "LUDUS_TESSERA PROBATIONES: $passed/$total praeteritae"
if [ -n "$failed_names" ]; then
    echo "FRACTAE:$failed_names"
    exit 1
fi

# NULLA probatio cursa = error operantis, NON successus - exitus II
# (contractus 0/1/2, vide compile_tests.sh radicis).
if [ "$total" -eq 0 ]; then
    echo "NULLA PROBATIO CURSA (filtrum sine paribus?) - exitus II"
    exit 2
fi

# canalis excubitoris: verdictum post-constructionem (tacet nisi stala)
"$RADIX_DIR/excubitor.sh" -tacitus "ludus_tessera/build/" >&2 || true
exit 0
