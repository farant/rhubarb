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

# Fontes radicis quibus materia in evolutione nititur.
declare -a RADIX_FONTES=(
    "piscina"
    "chorda"
    "chorda_aedificator"
    "xar"
    "friatio"
    "tabula_dispersa"
    "internamentum"
    "stml"
    "stml_macros"
    "excerptum"
    "utf8"
    "iter_directoria"
    "via"
    "selectio"
    "similitudo"
    "canon"
    "credo"
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
# materia/fontes (et md/fontes ubi md compilatur) additi 2026-09-17:
# substratum quod cliens compilat, olim ab indice absens.
CAPUT_RECENS=""
while IFS= read -r caput_via; do
    # <tolera codex="lint:nt-aequalitas" (>maximum quaeritur, non comparatio duorum: in aequalitate utrumlibet aeque valet
    if [ -z "$CAPUT_RECENS" ] || [ "$caput_via" -nt "$CAPUT_RECENS" ]; then
        CAPUT_RECENS="$caput_via"
    fi
done < <(find "$RADIX_DIR/include" "$MATERIA_DIR/fontes" \
             "$CRUSTA_DIR/fontes" "$CRUSTA_DIR/probationes" \
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
# materia sub-fontes: crusta eam CONSUMIT, non continet
for m in materia_lexicon materia_token materia_nodus materia_scribere \
         materia_arbor materia_arbor_aequalitas materia_coctor \
         materia_sedes materia_diagnostica materia_annotationes \
         materia_excusatio materia_exemplaria materia_pictor; do
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

for src in "$CRUSTA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [crusta] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# adiumenta probationum (probationes/*.c non probatio_*), e.g.
# lexicon_c89.c - descriptor GENERATUS, vehiculum portae phasis I
for src in "$CRUSTA_DIR"/probationes/*.c; do
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

# ---- custodia post constructionem (2026-09-17) ----
# Custodia capitum supra per indicem MANU scriptum iudicat; excubitor
# graphum inclusionum DERIVATUM legit (build/inclusiones.tsv). Obiectum
# post constructionem stalum = index supra caput ignoravit: olim
# materia/fontes deerat, et mutatio MateriaArborConsilium (A1 plani
# materia-sedes, campus ultimus additus) obiecta vetera cum structura
# minore in acervo reliquit - probationes et instrumenta contra ea
# currebant. Stala = nihil curritur (exitus II); graphus absens aut
# excubitor fractus = CAUTIO clamata, numquam tacita.
excubitor_exitus=0
excubitor_relatio="$("$RADIX_DIR/excubitor.sh" "${BUILD_DIR#"$RADIX_DIR"/}/" -tacitus 2>&1)" \
    || excubitor_exitus=$?
if [ "$excubitor_exitus" -eq 1 ]; then
    echo "$excubitor_relatio"
    echo "FRACTA: obiecta stala post constructionem - custodia capitum caput ignoravit; NIHIL CURSUM (exitus II)"
    exit 2
elif [ "$excubitor_exitus" -ne 0 ]; then
    echo "$excubitor_relatio" >&2
    echo "CAUTIO: excubitor exitus $excubitor_exitus - custodia post constructionem NON iudicavit" >&2
fi

# corpus domus: omnis .sh tracta (viae relativae radici) pro porta
# corporis (probatio_crusta_corpus legit build/crusta_corpus.lst
# radicis; lista absens = CREDO_CULPA)
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
