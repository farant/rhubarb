#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/oratio.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="oratio" gradus="fusio" tegit="materia/fontes/* md/fontes/* oratio/*.stml oratio/census/* oratio/fontes/* oratio/grammatica/* oratio/probationes/* oratio/vocabularium/* silva/fontes/*" legit="*.md *.c *.h *.stml"/>
if [ -z "${ORATIO_PROBATIONES_EFFUSIO:-}" ]; then
    export ORATIO_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/oratio.log"
    exit "${PIPESTATUS[0]}"
fi
# initium scripti (2026-10-22): mensor_suitae 'totum' hinc metitur, ut
# praevolatus (totum - compilatio - cursus) PRAEPARATIONEM obiectorum
# ante ansam probationum nominet (olim ab ansa ipsa - invisa)
SUITA_INITIUM="$(perl -MTime::HiRes -e 'print Time::HiRes::time')"

# oratio/compile_probationes.sh - Probationes orationis (cliens quartus materiae)
#
# Usage:
#   ./compile_probationes.sh            # omnes
#   ./compile_probationes.sh lexema     # quarum titulus "lexema" continet
#
# Exemplar: silva/compile_probationes.sh. Contractus exitus idem -
# 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum pravum).

set -u

ORATIO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MATERIA_DIR="$(cd "$ORATIO_DIR/../materia" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
BUILD_DIR="$ORATIO_DIR/build"
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
    "-I$ORATIO_DIR/fontes"
    "-I$ORATIO_DIR/probationes"
    "-I$RADIX_DIR/md/fontes"
    "-I$RADIX_DIR/silva/fontes"
)

# Fontes radicis quibus materia in evolutione nititur.
declare -a RADIX_FONTES=(
    "piscina"
    "sors"
    "entitates_html"
    "entitates_html_tabula"
    "chorda"
    "chorda_aedificator"
    "numerus_romanus"
    "xar"
    "dependentia"   # conllu.h includit (D130, 2026-10-17)
    "conllu"        # lector CoNLL-U, ex oratio/fontes translatus
    "friatio"
    "tabula_dispersa"
    "internamentum"
    "selectio"
    "stml"
    "stml_macros"
    "stml_html"
    "html_lexema"
    "similitudo"
    "sententia_fissio"
    "nuntium"
    "utf8"
    "canon"
    "credo"
    "memoria_processus_macos"   # credo memoria T2 (2026-10-19)
    "lectiones"   # materia_coctor lectiones crudas notat (fabrica spec 3 T3)
    "partitio"
    "partitio_aestimatio"
    # ambitus lintus Latini = iudex lint_latinus (unitates.stml; spec
    # unitates-codicis U4): probatio pinnae unitates per arborem veram
    # legit (git ls-files + filum)
    "unitates"
    "unitates_arboris"
    "filum"
    "processus_posix"
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

# ---- obiecta per compilatorem (budget B2 E3/J9) ----
# Olim clang per obiectum sub custodiis mtime manu scriptis (aequalitas
# secundi, caput recentissimum ex indice MANU scripto, find capitum
# silvae) et excubitor POST constructionem ut custodia indicis (index
# caput ignorans: materia/fontes 2026-09-17, silva/fontes 2026-09-25).
# Nunc bin/compilator --plures processu uno (clavis = cwd + argumenta +
# octeti fontis et OMNIUM capitum quae depfile nominat; obiectum
# identicum non rescribitur): index capitum nullus, ergo nec custodia
# eius - excubitor remotus (mtime iudicans post caput tactum obiecta
# identica non rescripta 'STALA' falso clamaret et suitam exitu II
# sisteret). Exemplar tessera 1b7f5edc, briar c6ab2020.
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 1
source "$RADIX_DIR/tools/nexus_probationis.sh"
cursor_instrumenta_parare || exit 1
: > "$CLAUSURAE_DIR/mandata.tsv"
VOCATIONES="$BUILD_DIR/obiecta.vocationes"
: > "$VOCATIONES"
obj_files=""
_obiectum_addere () {   # <fons> <obiectum>
    _cursor_vocationem_addere "$VOCATIONES" "$1" "$2"
    obj_files="$obj_files $2"
}

for f in "${RADIX_FONTES[@]}"; do
    _obiectum_addere "$RADIX_DIR/lib/$f.c" "$BUILD_DIR/$f.o"
done
shopt -s nullglob
# materia sub-fontes: md eam CONSUMIT, non continet
for m in materia_lexicon materia_token materia_nodus materia_scribere \
         materia_arbor materia_arbor_aequalitas materia_coctor \
         materia_sedes; do
    _obiectum_addere "$MATERIA_DIR/fontes/$m.c" "$BUILD_DIR/$m.o"
done
# silva sub-fontes: lexator totalis pro commentariis (T10 vocabula) -
# lexema et token solum, non arbor
for m in silva_token silva_lexema; do
    _obiectum_addere "$RADIX_DIR/silva/fontes/$m.c" "$BUILD_DIR/$m.o"
done
# md sub-fontes: porta corporis paragraphos markdown per arborem md
# legit (T3)
for src in "$RADIX_DIR"/md/fontes/*.c; do
    base="${src##*/}"; base="${base%.c}"
    _obiectum_addere "$src" "$BUILD_DIR/$base.o"
done
for src in "$ORATIO_DIR"/fontes/*.c; do
    base="${src##*/}"; base="${base%.c}"
    _obiectum_addere "$src" "$BUILD_DIR/$base.o"
done
# adiumenta probationum (probationes/*.c non probatio_*)
for src in "$ORATIO_DIR"/probationes/*.c; do
    base="${src##*/}"; base="${base%.c}"
    case "$base" in probatio_*) continue ;; esac
    _obiectum_addere "$src" "$BUILD_DIR/$base.o"
done
ante="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
if ! _cursor_plures_currere "$VOCATIONES"; then
    echo "FRACTA: obiecta (fons a clang supra nominatus)" ; exit 1
fi
post="$(stat -f '%i %N' "$BUILD_DIR"/*.o 2>/dev/null)"
echo "  obiecta: $(grep -c . "$VOCATIONES") per bin/compilator, $(comm -13 <(echo "$ante" | sort) <(echo "$post" | sort) | grep -c .) mutata"

# INDEX SYMBOLORUM (build/nexus.tsv) SANATUS ante portas (2026-09-07):
# percursus incrementalis ~1 s nulla mutatione, plenus (~70 s) instrumento
# novo. Olim cursor orationis numquam renovabat (excubitor solum post
# suitas silvae/officinae/radicis) - verba plagularum novarum pinnam UNA
# COMMISSIONE POST feriebant, saepe in sessione aliena.
if ! "$RADIX_DIR/silva/nexus.sh" -renovare > "$BUILD_DIR/nexus_renovatio.log" 2>&1; then
    echo "CAUTIO: index nexus non renovatus (oratio/build/nexus_renovatio.log) - porta vocabulorum contra tabulam veterem currit" >&2
fi
# corpus markdown (paragraphi eius = corpus orationis, T3) = plagulae a
# git TRACTATAE ('git ls-files' - ambulatio
# directoriorum MMMCX invenit, plerasque scripta .superpowers); porta
# corporis eam legit et absentem CLAMAT (numquam tacite praeterit)
git -C "$RADIX_DIR" ls-files '*.md' > "$BUILD_DIR/corpus_md.txt" || {
    echo "CAUTIO: corpus_md.txt scribi non potuit - porta corporis rubebit" >&2
}
# viae TRACTATAE (.c/.h) pro lintu identificatorum (2026-09-07): index
# nexus arborem totam ambulat, porta vias git-tractatas solas numerat
git -C "$RADIX_DIR" ls-files '*.c' '*.h' > "$BUILD_DIR/corpus_tractatae.txt" || {
    echo "CAUTIO: corpus_tractatae.txt scribi non potuit - porta vocabulorum rubebit" >&2
}
# corpus C commentariorum (T10 vocabula): lib/*.c et silva/fontes/*.c tracti
git -C "$RADIX_DIR" ls-files 'lib/*.c' 'silva/fontes/*.c' > "$BUILD_DIR/corpus_c.txt" || {
    echo "CAUTIO: corpus_c.txt scribi non potuit - porta vocabulorum rubebit" >&2
}

# metra suitae in volumen mensoris (tools/mensor_suitae.sh; praefixum
# "md." - silva.mensurae('md.', n) eas legit); numquam suitam frangit
source "$RADIX_DIR/tools/mensor_suitae.sh"
mensor_suitae_incipere "oratio." "$SUITA_INITIUM"
total=0 ; passed=0 ; failed_names=""
for test_file in "$ORATIO_DIR"/probationes/probatio_*.c; do
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
echo "ORATIO PROBATIONES: $passed/$total praeteritae"
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
