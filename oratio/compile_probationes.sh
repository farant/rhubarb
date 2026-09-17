#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/oratio.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
if [ -z "${ORATIO_PROBATIONES_EFFUSIO:-}" ]; then
    export ORATIO_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/oratio.log"
    exit "${PIPESTATUS[0]}"
fi

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
    "entitates_html"
    "entitates_html_tabula"
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
    "similitudo"
    "sententia_fissio"
    "nuntium"
    "utf8"
    "canon"
    "credo"
    "partitio"
    "partitio_aestimatio"
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
    if [ -z "$CAPUT_RECENS" ] || [ "$caput_via" -nt "$CAPUT_RECENS" ]; then
        CAPUT_RECENS="$caput_via"
    fi
done < <(find "$RADIX_DIR/include" "$MATERIA_DIR/fontes" "$RADIX_DIR/md/fontes" \
             "$ORATIO_DIR/fontes" "$ORATIO_DIR/probationes" \
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
# materia sub-fontes: md eam CONSUMIT, non continet
for m in materia_lexicon materia_token materia_nodus materia_scribere \
         materia_arbor materia_arbor_aequalitas materia_coctor \
         materia_sedes; do
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

# silva sub-fontes: lexator totalis pro commentariis (T10 vocabula) - lexema
# et token solum, non arbor
for m in silva_token silva_lexema; do
    src="$RADIX_DIR/silva/fontes/$m.c"
    obj="$BUILD_DIR/$m.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(find "$RADIX_DIR/silva/fontes" -name '*.h' -newer "$obj" 2>/dev/null | head -1)" ]; then
        echo "  [silva] $m.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || {
            echo "FRACTA: $m.c" ; exit 1
        }
    fi
    obj_files="$obj_files $obj"
done

# md sub-fontes: porta corporis paragraphos markdown per arborem md legit (T3)
for src in "$RADIX_DIR"/md/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [md] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

for src in "$ORATIO_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [oratio] $base.c"
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then
            echo "FRACTA: $base.c" ; exit 1
        fi
    fi
    obj_files="$obj_files $obj"
done

# adiumenta probationum (probationes/*.c non probatio_*), e.g.
# lexicon_c89.c - descriptor GENERATUS, vehiculum portae phasis I
for src in "$ORATIO_DIR"/probationes/*.c; do
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
mensor_suitae_incipere "oratio."
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
