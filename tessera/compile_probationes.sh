#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/tessera.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS
# servatus). Ratio plena: compile_tests.sh radicis.
# <porta titulus="tessera" tegit="tessera/*_fontes_generata.sh tessera/fontes/* tessera/instrumenta/*"/>
if [ -z "${TESSERA_PROBATIONES_EFFUSIO:-}" ]; then
    export TESSERA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/tessera.log"
    exit "${PIPESTATUS[0]}"
fi

# tessera/compile_probationes.sh - Probationes tesserae compilare et currere
#
# Usage:
#   ./compile_probationes.sh            # omnes probationes
#   ./compile_probationes.sh cellula    # probationes quarum titulus "cellula" continet
#
# Silva's template (glob discovery, incremental objects, substring filter),
# self-contained to tessera/, no Cocoa/Security frameworks — tessera is a
# pure library. System headers (termios etc.) live ONLY in the posix pons
# (fontes/tessera_pons_posix.c); the suite itself never needs a tty (the
# memoria pons is the test seam).

set -u

TESSERA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$TESSERA_DIR/.." && pwd)"
BUILD_DIR="$TESSERA_DIR/build"
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
    "-I$TESSERA_DIR/fontes"
    "-I$TESSERA_DIR/probationes"
    "-I$TESSERA_DIR/instrumenta/folium"
    "-I$TESSERA_DIR/instrumenta/effigies"
    "-I$TESSERA_DIR/instrumenta/musivum"
)

# Rhubarb lib sources tessera depends on during development (vendoring
# happens only at amalgamation; the vendored set is these MINUS credo —
# spec-v2 §1.2: latina+piscina+chorda+chorda_aedificator+utf8, closed graph).
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "utf8"
    # eventus B1b: lector initus super lexematorem; B3a: pipeline in
    # rivo, lector = proiectio Eventus
    "series_terminalis"
    "eventus_cauda"
    "claves_physicae"
    "interpres_terminalis"
    "rivus_terminalis"
    # modulus 004: emissio SGR per codicem stili
    "stilus_terminalis"
    "runae"
    "runae_tabulae"
    "quadrans"
    "dithering"
    "imago"
    "imago_opus"
    "credo"
    "memoria_processus_macos"   # credo memoria T2 (2026-10-19)
    "lectiones"   # probatio folium per librum scribit (fabrica-8, 2026-10-27)
    # tessellatio T5 (musivum): clausura lib/tessellatio per aedilis
    # mensa 2026-10-01 - sine Cocoa (tabula_pixelorum.h, T4a)
    "color"
    "delineare"
    "exemplaria"
    "delineare_mandata"
    "fenestra_textus"
    "fons"
    "glyphae_ductae"   # fenestra_textus: glyphae ductae ut terminale
    "friatio"
    "internamentum"
    "mandatum"
    "modulus"
    "selectio"
    "stml"
    "tabula_dispersa"
    "tessellatio"
    "thema"
    "xar"
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

# ---- 1-2. obiecta per compilatorem (budget B2 E3/J9) ----
# Olim clang per obiectum sub regula 'caput recentissimum': caput tactum
# sine mutatione omnia (XLIV) recompilabat (48 s), caput mutatum quod
# regula non videbat obiectum stalum relinquebat. Nunc bin/compilator
# --plures (thesaurus contenti: clavis = cwd + argumenta + octeti fontis
# et capitum depfile; obiectum identicum non rescribitur), processu uno
# (tools/cursor_communis.sh, exemplar ludus_tessera 765f1f2e).
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 1
source "$RADIX_DIR/tools/nexus_probationis.sh"
cursor_instrumenta_parare || exit 1
: > "$CLAUSURAE_DIR/mandata.tsv"
VOCATIONES="$BUILD_DIR/obiecta.vocationes"
: > "$VOCATIONES"
obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    _cursor_vocationem_addere "$VOCATIONES" "$RADIX_DIR/lib/$f.c" \
        "$BUILD_DIR/$f.o"
    obj_files="$obj_files $BUILD_DIR/$f.o"
done
shopt -s nullglob
# instrumenta/folium: pars PURA instrumenti folium (runae U7) - extra
# bibliothecam et amalgama, sed probationibus eius nexa
for src in "$TESSERA_DIR"/fontes/*.c "$TESSERA_DIR"/instrumenta/folium/*.c \
           "$TESSERA_DIR"/instrumenta/effigies/*.c \
           "$TESSERA_DIR"/instrumenta/musivum/*.c; do
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

# ---- 3. discover, compile, run probationes ----
# Probatio per compilatorem (obiectum in build/probationes/), nexus SOLUM
# si signum nexus mutatum (tools/nexus_probationis.sh): olim omnis
# probatio omni cursu compilata et nexa.
total=0 ; passed=0 ; failed_names=""
for test_file in "$TESSERA_DIR"/probationes/probatio_*.c; do
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
echo "TESSERA PROBATIONES: $passed/$total praeteritae"
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
# 'STALA' falso clamaret), ut ludus_tessera et compile_tests.sh
exit 0
