#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/briar.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="briar" tegit="briar/facies/* briar/fontes/* briar/grammatica/* briar/probationes/* materia/fontes/* md/fontes/*" legit="md/html/md-html.stml silva/fontes/systema_c89.h silva/fontes/systema_posix.h"/>
if [ -z "${BRIAR_PROBATIONES_EFFUSIO:-}" ]; then
    export BRIAR_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/briar.log"
    exit "${PIPESTATUS[0]}"
fi
# initium scripti (2026-10-22): mensor_suitae 'totum' hinc metitur, ut
# praevolatus (totum - compilatio - cursus) PRAEPARATIONEM obiectorum
# ante ansam probationum nominet (olim ab ansa ipsa - invisa)
SUITA_INITIUM="$(perl -MTime::HiRes -e 'print Time::HiRes::time')"

# briar/compile_probationes.sh - Probationes thistle (cliens materiae; md consumit)
#
# Usage:
#   ./compile_probationes.sh            # omnes
#   ./compile_probationes.sh lexema     # quarum titulus "lexema" continet
#
# Exemplar: silva/compile_probationes.sh. Contractus exitus idem -
# 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum pravum).

set -u

BRIAR_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MD_DIR="$(cd "$BRIAR_DIR/../md" && pwd)"
MATERIA_DIR="$(cd "$BRIAR_DIR/../materia" && pwd)"
RADIX_DIR="$(cd "$BRIAR_DIR/.." && pwd)"
BUILD_DIR="$BRIAR_DIR/build"
# SERA cursoris (tools/sera.sh, 2026-09-07): cursus duo eiusdem suitae
# obiecta eadem scriberent, binarium currens obtererent (137);
# exspectatio ad SERA_TECTUM (DC s), deinde exitus 2 = NIHIL cucurrit
# (tenens nominatur). Involucra et pythonica reentrant (SERA_TENTA).
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
mkdir -p "$BUILD_DIR"

# tabula symbolum -> caput (corpus.symbola.tsv): nexus derivationis
# inclusionum eam e fonte disci legit - hic genita si vetus
# --obiecta (actio obiecta_briar): tabula et capsula probationibus solis
# serviunt - omittuntur (actiones corpus_silicis, capsula_facies_briar
# eas producunt; 1b T6)
if [ "${1:-}" != "--obiecta" ]; then
    source "$RADIX_DIR/tools/corpus_infixum.sh"
    ( cd "$RADIX_DIR" && corpus_symbola_generare ) || { echo "FRACTA: corpus.symbola.tsv" ; exit 1; }

    # vestis faciei infixa: porta spectatoris eam contra plagulas discI
    # confert (capsula stala = vitium verum, non molestia)
    source "$RADIX_DIR/tools/briar_facies_capsula.sh"
    ( cd "$RADIX_DIR" && briar_facies_capsula_regenerare ) || { echo "FRACTA: capsula faciei" ; exit 1; }
fi

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$MATERIA_DIR/fontes"
    "-I$MD_DIR/fontes"
    "-I$BRIAR_DIR/fontes"
    "-I$BRIAR_DIR/probationes"
    "-I$RADIX_DIR/silva/amalgama"
    # compendium capitis (communis cum legato; -functiones)
    "-I$RADIX_DIR/officina/instrumenta"
    "-I$RADIX_DIR/silva/instrumenta"
)

# Fontes radicis quibus materia in evolutione nititur.
declare -a RADIX_FONTES=(
    "piscina"
    "sors"
    "chorda"
    "chorda_aedificator"
    "xar"
    "friatio"
    "tabula_dispersa"
    "internamentum"
    "selectio"
    "stml"
    "stml_macros"
    "stml_html"
    "html_lexema"
    "entitates_html"
    "entitates_html_tabula"
    "similitudo"
    "canon"
    "credo"
    "memoria_processus_macos"   # credo memoria T2 (2026-10-19)
    # clausura silicis (bin/aedilis lib/silex.c --partes, 2026-09-05):
    # nexus regiones C per silvam parsat capitibus e FONTE silicis
    "filum"
    "via"
    "lectiones"
    "processus_posix"
    "iter_directoria"
    "sigillum"
    "json"
    "utf8"         # json lexema UTF-8 validat (2026-09-22)
    "capsula"
    "differentia"
    "scrinium"
    "flatura"
    "moneta"
    "fasti"
    "volumen"
    "hora_hybrida"   # volumen D2: identitas actuum
    "machina_posix"
    "silex"
    # briar -app (spec par. 4.8): fasciculum et quae trahit
    # (bin/aedilis lib/fasciculum.c --partes, 2026-09-14)
    "xml"
    "base64"
    "plist"
    "fasciculum"
    # icones (opus IV): imago NON additur - nm: icones, imago_opus,
    # imago_png nullum symbolum ex imago.o poscunt (stb_image extra
    # omnem portam briar manet); aedilis imago.c per imago.h nominat
    "imago_opus"
    "imago_png"
    "icones"
)

# --obiecta (fabrica 1b T5): SOLA obiecta briar/build/*.o (cum
# adiumentis) struuntur, exitus 0 - nulla probatio. Actio 'obiecta_briar'
# (ignota) praecondicio est briar et spectatoris, qui ea CAECE nectunt.
OBIECTA=0
if [ "${1:-}" = "--obiecta" ]; then
    OBIECTA=1
    shift
fi
FILTER="${1:-}"

if [ -n "$FILTER" ]; then
    FILTRUM_DATUM="$FILTER"
    FILTER="${FILTER##*/}"
    FILTER="${FILTER%.c}"
    if [ "$FILTER" != "$FILTRUM_DATUM" ]; then
        echo "compile_probationes: filtrum '$FILTRUM_DATUM' -> '$FILTER'" >&2
    fi
fi

# ---- obiecta per compilatorem (budget B2 E3/J9) ----
# Olim clang per obiectum sub custodiis mtime manu scriptis (aequalitas
# secundi, caput recentissimum, capsula regula fontis sola): caput tactum
# sine mutatione omnia recompilabat, aequalitas secundi rubrum falsum
# pepererat (2026-08-27). Nunc bin/compilator --plures processu uno
# (clavis = cwd + argumenta + octeti fontis et omnium capitum depfile;
# obiectum identicum non rescribitur) - custodiae superfluae. Exemplar
# tessera 1b7f5edc, officina 85629289.
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 1
source "$RADIX_DIR/tools/nexus_probationis.sh"
cursor_instrumenta_parare || exit 1
: > "$CLAUSURAE_DIR/mandata.tsv"
VOCATIONES="$BUILD_DIR/obiecta.vocationes"
: > "$VOCATIONES"
obj_files=""
# vocatio cum radicibus PROPRIIS (silva_lexicon fontes silvae poscit)
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

# venditorius: sqlite3 (volumen -> silex); vexilla compile_tests.sh
declare -a VENDOR_FLAGS=("-O2" "-DSQLITE_ENABLE_FTS5" "-DSQLITE_THREADSAFE=0"
    "-DSQLITE_DQS=0" "-DSQLITE_DEFAULT_MEMSTATUS=0" "-DSQLITE_OMIT_LOAD_EXTENSION"
    "-DSQLITE_OMIT_DEPRECATED" "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1")
src="$RADIX_DIR/vendor/sqlite3.c"
obj="$BUILD_DIR/sqlite3.o"
if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
    echo "  [vendor] sqlite3.c"
    if ! _vendicatum_compilare "$obj" "${VENDOR_FLAGS[@]}" -c vendor/sqlite3.c; then
        echo "FRACTA: sqlite3.c" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# silva: amalgama ut obiectum UNUM (officina exemplar) - fontes silvae
# absunt de industria; regiones C thistle per silvam parsantur
src="$RADIX_DIR/silva/amalgama/silva.c"
obj="$BUILD_DIR/amalgama_silva.o"
if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
    echo "  [amalgama] silva.c"
    if ! _vendicatum_compilare "$obj" "${GCC_FLAGS[@]}" -c silva/amalgama/silva.c; then
        echo "FRACTA: amalgama silva" ; exit 1
    fi
fi
obj_files="$obj_files $obj"

# compositio lexici systematis (silva-migratio T16b): modulus textualis
# silvae (latina + piscina + silva_limes_posix.h solum) - transitus
# silvae briar lexicon systematis ut examen accipit
_vocatio_addere "$RADIX_DIR/silva/instrumenta/silva_lexicon.c" \
    "$BUILD_DIR/silva_lexicon.o" "${INCLUDE_FLAGS[@]}" \
    "-I$RADIX_DIR/silva/fontes"
_vocatio_addere "$RADIX_DIR/silva/instrumenta/silva_conflatio.c" \
    "$BUILD_DIR/silva_conflatio.o" "${INCLUDE_FLAGS[@]}"
_vocatio_addere "$RADIX_DIR/officina/instrumenta/compendium.c" \
    "$BUILD_DIR/compendium.o" "${INCLUDE_FLAGS[@]}"
shopt -s nullglob
for m in materia_lexicon materia_token materia_nodus materia_scribere \
         materia_arbor materia_arbor_aequalitas; do
    _vocatio_addere "$MATERIA_DIR/fontes/$m.c" "$BUILD_DIR/$m.o" \
        "${INCLUDE_FLAGS[@]}"
done
for base in md_registrum_coctum md_lexicon md_lexema md_arbor md_inlinea \
            md_decoctum md_stml md_html; do
    _vocatio_addere "$MD_DIR/fontes/$base.c" "$BUILD_DIR/$base.o" \
        "${INCLUDE_FLAGS[@]}"
done
# capsula vestis (generata in build/ radicis)
_vocatio_addere "$RADIX_DIR/build/capsula_facies_briar.c" \
    "$BUILD_DIR/capsula_facies_briar.o" "${INCLUDE_FLAGS[@]}"
for src in "$BRIAR_DIR"/fontes/*.c; do
    base="${src##*/}"; base="${base%.c}"
    _vocatio_addere "$src" "$BUILD_DIR/$base.o" "${INCLUDE_FLAGS[@]}"
done
# adiumenta probationum (probationes/*.c non probatio_*)
for src in "$BRIAR_DIR"/probationes/*.c; do
    base="${src##*/}"; base="${base%.c}"
    case "$base" in probatio_*) continue ;; esac
    _vocatio_addere "$src" "$BUILD_DIR/$base.o" "${INCLUDE_FLAGS[@]}"
done
ante="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
if ! _cursor_plures_currere "$VOCATIONES"; then
    echo "FRACTA: obiecta (fons a clang supra nominatus)" ; exit 1
fi
post="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
echo "  obiecta: $(grep -c . "$VOCATIONES") per bin/compilator, $(comm -13 <(echo "$ante" | sort) <(echo "$post" | sort) | grep -c .) mutata"

# metra suitae in volumen mensoris (tools/mensor_suitae.sh; praefixum
# "md." - silva.mensurae('md.', n) eas legit); numquam suitam frangit
source "$RADIX_DIR/tools/mensor_suitae.sh"
mensor_suitae_incipere "briar." "$SUITA_INITIUM"
total=0 ; passed=0 ; failed_names=""
if [ "$OBIECTA" -eq 1 ]; then
    echo "obiecta briar parata"
    exit 0
fi

for test_file in "$BRIAR_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    total=$((total + 1))
    bin="$BUILD_DIR/$name"
    echo ""
    echo "=== $name ==="
    t0=$(mensor_suitae_nunc)
    # probatio per compilatorem, nexus SOLUM si signum nexus mutatum
    # (tools/nexus_probationis.sh): olim omnis probatio omni cursu
    # compilata et nexa
    obj="$BUILD_DIR/probationes/$name.o"
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
echo "BRIAR PROBATIONES: $passed/$total praeteritae"
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
