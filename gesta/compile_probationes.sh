#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/gesta.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ratio plena: compile_tests.sh radicis.
# <porta titulus="gesta" tegit="gesta/*_fontes_generata.sh gesta/fontes/*"/>
if [ -z "${GESTA_PROBATIONES_EFFUSIO:-}" ]; then
    export GESTA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/gesta.log"
    exit "${PIPESTATUS[0]}"
fi

# gesta/compile_probationes.sh - Probationes gestae compilare et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh gesta      # filtrum substringae
#
# Tessera's template. SQLite VENDICATUM compilatur LAXUM in obiecto
# proprio (compile_tests.sh exemplar - numquam sub vexillis domus);
# obiectum in gesta/build/ cachetur.

set -u

GESTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$GESTA_DIR/.." && pwd)"
BUILD_DIR="$GESTA_DIR/build"
# SERA cursoris (tools/sera.sh, 2026-09-07): cursus duo eiusdem suitae
# obiecta eadem scriberent, binarium currens obtererent (137);
# exspectatio ad SERA_TECTUM (DC s), deinde exitus 2 = NIHIL cucurrit
# (tenens nominatur). Involucra et pythonica reentrant (SERA_TENTA).
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
mkdir -p "$BUILD_DIR"

# ANCORA: fixturae probationum et scripta daemonum viis radicis
# relativis utuntur - cursus e directorio alio OMNIA a salutatione
# frangit (aspectus catastrophae, causa CWD; mensuratum 2026-07-30)
cd "$RADIX_DIR" || exit 1

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$GESTA_DIR/fontes"
    "-I$GESTA_DIR/probationes"
)

# Fontes bibliothecarum radicis quibus gesta nititur - GENERATUM
# AB AEDILE (gesta/fontes_generare.sh regenerat; lista manualis
# prima retirata, aedilis Phasis C 2026-07-20)
source "$GESTA_DIR/fontes_generata.sh"

# SQLite vendicatum - LAXUM, obiectum proprium (exemplar
# compile_tests.sh VENDOR_FLAGS)
declare -a VENDOR_FLAGS=(
    "-O2"
    "-DSQLITE_ENABLE_FTS5"
    "-DSQLITE_THREADSAFE=0"
    "-DSQLITE_DQS=0"
    "-DSQLITE_DEFAULT_MEMSTATUS=0"
    "-DSQLITE_OMIT_LOAD_EXTENSION"
    "-DSQLITE_OMIT_DEPRECATED"
    "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"
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

# VENDICATA PER THESAURUM (budget B1 E7, 2026-10-28): fontes magni
# communes (vendor/sqlite3.c ~IX.V MB, silva/amalgama/silva.c ~V MB) per
# bin/compilator EX RADICE, argumentis iisdem ac compile_tests.sh: clavis
# (cwd + argumenta sine -o + octeti fontis et capitum) congruit, ergo
# obiectum radicis (aut suitae prioris) ex thesauro sumitur - in frigida
# compilatio una pro IV. Compilator absens -> clang ipse.
_vendicatum_compilare () {   # <obiectum> <argumenta clang, fons RADICI relativus>
    local obiectum="$1"
    shift
    if [ -x "$RADIX_DIR/bin/compilator" ]; then
        ( cd "$RADIX_DIR" && bin/compilator "$@" -o "$obiectum" )
    else
        ( cd "$RADIX_DIR" && clang "$@" -o "$obiectum" )
    fi
}

# ---- 0. vendor sqlite3.o (laxum; source-newer solum) ----
sqlite_obj="$BUILD_DIR/sqlite3.o"
if [ ! -f "$sqlite_obj" ] || ! [ "$sqlite_obj" -nt "$RADIX_DIR/vendor/sqlite3.c" ]; then
    echo "  [vendor] sqlite3.c (laxum)"
    if ! _vendicatum_compilare "$sqlite_obj" "${VENDOR_FLAGS[@]}" -c vendor/sqlite3.c; then
        echo "FRACTA: vendor/sqlite3.c" ; exit 1
    fi
fi
obj_files="$sqlite_obj"

# ---- 1-2. obiecta per compilatorem (budget B2 E3/J9) ----
# Olim clang per obiectum sub regula 'caput recentissimum' (caput tactum
# sine mutatione omnia recompilabat; caput mutatum quod regula non
# videbat obiectum stalum relinquebat). Nunc bin/compilator --plures
# processu uno (tools/cursor_communis.sh; clavis contenti, obiectum
# identicum non rescribitur) - exemplar tessera 1b7f5edc.
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 1
source "$RADIX_DIR/tools/nexus_probationis.sh"
cursor_instrumenta_parare || exit 1
: > "$CLAUSURAE_DIR/mandata.tsv"
VOCATIONES="$BUILD_DIR/obiecta.vocationes"
: > "$VOCATIONES"
for f in "${RADIX_FONTES[@]}"; do
    _cursor_vocationem_addere "$VOCATIONES" "$RADIX_DIR/lib/$f.c" \
        "$BUILD_DIR/$f.o"
    obj_files="$obj_files $BUILD_DIR/$f.o"
done
shopt -s nullglob
for src in "$GESTA_DIR"/fontes/*.c; do
    base="${src##*/}"; base="${base%.c}"
    _cursor_vocationem_addere "$VOCATIONES" "$src" "$BUILD_DIR/$base.o"
    obj_files="$obj_files $BUILD_DIR/$base.o"
done
ante="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
if ! _cursor_plures_currere "$VOCATIONES"; then
    echo "FRACTA: obiecta (fons a clang supra nominatus)" ; exit 1
fi
post="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
echo "  obiecta: $(grep -c . "$VOCATIONES") per bin/compilator, $(comm -13 <(echo "$ante" | sort) <(echo "$post" | sort) | grep -c .) mutata"

# ---- 3. probationes invenire, compilare, currere ----
# Probatio per compilatorem (obiectum in build/probationes/), nexus SOLUM
# si signum nexus mutatum (tools/nexus_probationis.sh): olim omnis
# probatio omni cursu compilata et nexa.
total=0 ; passed=0 ; failed_names=""
for test_file in "$GESTA_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    total=$((total + 1))
    bin="$BUILD_DIR/$name"
    obj="$BUILD_DIR/probationes/$name.o"
    echo ""
    echo "=== $name ==="
    if ! _cursor_compilare "$test_file" "$obj"; then
        echo "FRACTA (compilatio): $name"
        failed_names="$failed_names $name"
        continue
    fi
    # shellcheck disable=SC2086 (obj_files: verba consulto scissa)
    if ! probationem_nectere "$bin" "${GCC_FLAGS[*]}" "$obj" $obj_files; then
        echo "FRACTA (nexus): $name"
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
echo "GESTA PROBATIONES: $passed/$total praeteritae"
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

# canalis excubitoris remotus (budget B2 E3/J9): obiecta per
# compilatorem (clavis contenti) - mtime iudicium excubitoris superfluum
# et mendax (obiectum identicum non rescribitur: post caput tactum
# 'STALA' falso clamaret), ut tessera et compile_tests.sh
exit 0
