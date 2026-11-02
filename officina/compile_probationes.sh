#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/officina.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ratio plena: compile_tests.sh radicis.
# <porta titulus="officina" gradus="fusio" tegit="officina/*_fontes_generata.sh officina/fontes/* officina/instrumenta/* officina/probationes/* silva/fontes/* silva/instrumenta/*"/>
if [ -z "${OFFICINA_PROBATIONES_EFFUSIO:-}" ]; then
    export OFFICINA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/officina.log"
    exit "${PIPESTATUS[0]}"
fi

# officina/compile_probationes.sh - Probationes officinae compilare
# et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh medulla    # substring filter
#
# Saltuarius's template. THE AMALGAM BOUNDARY IS THE API: silva
# enters this build ONLY as amalgama/silva.c object with the
# amalgama include dir — silva/fontes/ is deliberately absent from
# INCLUDE_FLAGS (a fontes include is a compile error here; that is
# the pin working). Medulla itself is silva-free (SilvaNodus tag
# forward-declared); demissio (M1b) is the real silva consumer.
# (Sole exception: the silva_lexicon.o step passes a private
# -Ifontes on its own clang line - that unit is silva-side code
# needing SILVA_LIMES_POSIX_TITULUS, not officina code.)

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
BUILD_DIR="$OFF_DIR/build"
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
    "-I$RADIX_DIR/silva/amalgama"
    "-I$RADIX_DIR/silva/instrumenta"
    "-I$RADIX_DIR/tessera/amalgama"
    "-I$OFF_DIR/fontes"
    "-I$OFF_DIR/instrumenta"
    "-I$OFF_DIR/probationes"
)

# Rhubarb libs officina consumes as a monorepo citizen
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "tabula_dispersa"
    "friatio"
    "credo"
    "memoria_processus_macos"   # credo memoria T2 (2026-10-19)
    "internamentum"
    "json"
    "utf8"         # json lexema UTF-8 validat (2026-09-22)
    "tabellarius"
    "similitudo"
    "sigillum"
    "vigilia"
    "lectiones"    # vigilia per librum legit (fabrica plan 5 T5a)
    "filum"        # probationes per filum delent (fabrica-8, 2026-10-27)
    "processus_posix"
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

# ---- 1-3. obiecta per compilatorem (budget B2 E3/J9) ----
# Olim clang per obiectum sub regulis mtime manu scriptis (caput
# recentissimum, capita singula nominata, amalgama tesserae SINE capitibus):
# caput tactum sine mutatione omnia recompilabat, caput non nominatum
# obiectum stalum relinquebat (silva.h forma mutata -> corruptio ABI).
# Nunc bin/compilator --plures processu uno (clavis = cwd + argumenta +
# octeti fontis et OMNIUM capitum depfile; obiectum identicum non
# rescribitur) - exemplar tessera 1b7f5edc, gesta f3a54629.
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 1
source "$RADIX_DIR/tools/nexus_probationis.sh"
cursor_instrumenta_parare || exit 1
: > "$CLAUSURAE_DIR/mandata.tsv"
VOCATIONES="$BUILD_DIR/obiecta.vocationes"
: > "$VOCATIONES"
obj_files=""
# vocatio cum radicibus PROPRIIS (obiecta officinae radices varias
# poscunt: amalgama tesserae sola include/, silva_lexicon fontes silvae)
_vocatio_addere () {   # <fons> <obiectum> <vexilla radicum...>
    local fons="$1" obj="$2"
    shift 2
    printf '%s\t%s\n' "$obj" "${GCC_FLAGS[*]} $* -c $fons" \
        >> "$CLAUSURAE_DIR/mandata.tsv"
    echo "${GCC_FLAGS[*]} $* -c $fons -o $obj" >> "$VOCATIONES"
    obj_files="$obj_files $obj"
}

for f in "${RADIX_FONTES[@]}"; do
    _vocatio_addere "$RADIX_DIR/lib/$f.c" "$BUILD_DIR/$f.o" \
        "${INCLUDE_FLAGS[@]}"
done

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

# amalgama silvae ut obiectum (substratum demissionis) - via vendicata
src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
    echo "  [amalgama] silva.c"
    if ! _vendicatum_compilare "$obj" "${GCC_FLAGS[@]}" -c silva/amalgama/silva.c; then
        echo "FRACTA: amalgama silva" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# amalgama tesserae + vindex visum (M3 chunk 6: probationes cellularum
# sine capite; obiecta inutilia innocua)
_vocatio_addere "$RADIX_DIR/tessera/amalgama/tessera.c" \
    "$BUILD_DIR/amalgama_tessera.o" "-I$RADIX_DIR/include"
_vocatio_addere "$OFF_DIR/instrumenta/vindex_visum.c" \
    "$BUILD_DIR/vindex_visum.o" "${INCLUDE_FLAGS[@]}" \
    "-I$RADIX_DIR/tessera/amalgama" "-I$OFF_DIR/instrumenta"
# nexus_ordines (unitas ordinum communis - legatus)
_vocatio_addere "$RADIX_DIR/silva/instrumenta/nexus_ordines.c" \
    "$BUILD_DIR/nexus_ordines.o" "${INCLUDE_FLAGS[@]}"
# silva_lexicon (compositio systematis - praeparator): unitas silvae,
# radices fontium silvae et materiae PROPRIAE (vide supra)
_vocatio_addere "$RADIX_DIR/silva/instrumenta/silva_lexicon.c" \
    "$BUILD_DIR/silva_lexicon.o" "${INCLUDE_FLAGS[@]}" \
    "-I$RADIX_DIR/silva/fontes" "-I$RADIX_DIR/materia/fontes"
# praeparator (unitas oneratorum communis), compendium capitis (communis
# legato et briar), sessio, legatus
for m in praeparator compendium sessio legatus; do
    _vocatio_addere "$OFF_DIR/instrumenta/$m.c" "$BUILD_DIR/$m.o" \
        "${INCLUDE_FLAGS[@]}"
done

# fontes officinae
shopt -s nullglob
for src in "$OFF_DIR"/fontes/*.c; do
    base="${src##*/}"; base="${base%.c}"
    _vocatio_addere "$src" "$BUILD_DIR/$base.o" "${INCLUDE_FLAGS[@]}"
done
ante="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
if ! _cursor_plures_currere "$VOCATIONES"; then
    echo "FRACTA: obiecta (fons a clang supra nominatus)" ; exit 1
fi
post="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
echo "  obiecta: $(grep -c . "$VOCATIONES") per bin/compilator, $(comm -13 <(echo "$ante" | sort) <(echo "$post" | sort) | grep -c .) mutata"

# ---- 4. discover, compile, run probationes ----
# Probatio per compilatorem (obiectum in build/probationes/), nexus SOLUM
# si signum nexus mutatum (tools/nexus_probationis.sh): olim omnis
# probatio omni cursu compilata et nexa.
total=0 ; passed=0 ; failed_names=""
for test_file in "$OFF_DIR"/probationes/probatio_*.c; do
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
echo "OFFICINA PROBATIONES: $passed/$total praeteritae"
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
# 'STALA' falso clamaret), ut tessera, gesta et compile_tests.sh
exit 0
