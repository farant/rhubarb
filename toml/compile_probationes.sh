#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/toml.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
if [ -z "${TOML_PROBATIONES_EFFUSIO:-}" ]; then
    export TOML_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/toml.log"
    exit "${PIPESTATUS[0]}"
fi

# toml/compile_probationes.sh - Probationes clientis toml (materia)
#
# Usage:
#   ./compile_probationes.sh            # omnes
#   ./compile_probationes.sh lexicon    # quarum titulus "lexicon" continet
#
# Exemplar: silva/compile_probationes.sh. Contractus exitus idem -
# 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum pravum).

set -u

TOML_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MATERIA_DIR="$(cd "$TOML_DIR/../materia" && pwd)"
RADIX_DIR="$(cd "$TOML_DIR/.." && pwd)"
BUILD_DIR="$TOML_DIR/build"
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

# toml/fontes ANTE include: "toml.h" novum (Q10) nomen commune habet cum
# include/toml.h vetere (lib/toml.c) usque ad Q12, quae vetus delet.
# Numquam in binario uno: cursor hic lib/toml.c non compilat.
declare -a INCLUDE_FLAGS=(
    "-I$TOML_DIR/fontes"
    "-I$RADIX_DIR/include"
    "-I$MATERIA_DIR/fontes"
    "-I$TOML_DIR/probationes"
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

# ---- FONTES ex clausuris (fabrica plan 2 T5) ----
# Olim indices MANU scripti (XXIV lib, XIII materia, toml/fontes,
# adiumenta) et regula crassa 'caput recentissimum quodvis omne obiectum
# stalum facit' (mtime, aequalitas secundi). Nunc: clausura cuiusque
# probationis ab aedile (bin/aedilis --enumerare, idem ac ceterae suitae),
# unio compilatur per bin/compilator ('clang -c' per thesaurum: clavis
# ex octetis fontis et capitum depfile, radicibus -I, identitate clang -
# nulla mtime). Obiectum identicum non rescribitur; ideo custodia
# excubitoris post constructionem (mtime) hic non iam valet et deleta
# est (plan 2 T7 excubitorem ipsum emeritum facit).
[ -x "$RADIX_DIR/bin/aedilis" ] || "$RADIX_DIR/tools/aedilis_struere.sh" >&2 \
    || { echo "FRACTA: bin/aedilis struere nequit"; exit 1; }
[ -x "$RADIX_DIR/bin/compilator" ] || "$RADIX_DIR/tools/compilator_struere.sh" >&2 \
    || { echo "FRACTA: bin/compilator struere nequit"; exit 1; }
COMPILATOR="$RADIX_DIR/bin/compilator"
# thesaurus unus pro aedile (recorda extractionis) et compilatore
# (obiecta): FABRICA_THESAURUS, ordinarie build/aedilis/obiecta. Sine eo
# aedilis XIII clausuras omni cursu de novo parsat (6.8 s contra 0.26 s)
THESAURUS="${FABRICA_THESAURUS:-$RADIX_DIR/build/aedilis/obiecta}"
export FABRICA_THESAURUS="$THESAURUS"
CLAUSURAE_DIR="$BUILD_DIR/clausurae"
mkdir -p "$CLAUSURAE_DIR" "$BUILD_DIR/probationes"
shopt -s nullglob
for test_file in "$TOML_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if ! (cd "$RADIX_DIR" && bin/aedilis "${test_file#"$RADIX_DIR"/}" --enumerare \
            --thesaurus "$THESAURUS") \
            > "$CLAUSURAE_DIR/$name.lst"; then
        echo "FRACTA (clausura): $name"; exit 1
    fi
done
shopt -u nullglob
numerus_fontium=0
while IFS= read -r fons; do
    obj="$BUILD_DIR/$(basename "$fons" .c).o"
    # via ABSOLUTA et cwd vocantis, ut olim: -g utramque infigit -
    # obiecta cursori veteri octetim aequalia (oraculum)
    if ! "$COMPILATOR" "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
            -c "$RADIX_DIR/$fons" -o "$obj"; then
        echo "FRACTA: $fons" ; exit 1
    fi
    numerus_fontium=$((numerus_fontium + 1))
done < <(cat "$CLAUSURAE_DIR"/*.lst | sort -u)
echo "  fontes clausurarum: $numerus_fontium (per bin/compilator)"

# corpus domus: omnis .toml tracta extra fixa clientis (viae relativae
# radici) pro porta corporis (probatio_toml_corpus legit
# build/toml_corpus.lst; lista absens = CREDO_CULPA)
mkdir -p "$RADIX_DIR/build"
(cd "$RADIX_DIR" && git ls-files '*.toml' | grep -v '^toml/probationes/fixa/') \
    > "$RADIX_DIR/build/toml_corpus.lst"

# aurum silvestre tomllib (Q9): numquam commissum (contentum alienum);
# regeneratur si abest aut manifesto vetustius. Python absens = CAUTIO
# clamata, et porta differentiae tunc rubet (numquam tacite viridis).
AURUM_SILVESTRE="$BUILD_DIR/aurum_silvestre.txt"
MANIFESTUM="$TOML_DIR/probationes/fixa/silvestria.manifestum"
# <tolera codex="lint:nt-aequalitas" (manifestum in aequalitate non mutatum: aurum eodem secundo scriptum recens est)
if [ ! -f "$AURUM_SILVESTRE" ] || [ "$MANIFESTUM" -nt "$AURUM_SILVESTRE" ]; then
    echo "  [aurum] tomllib silvestre regeneratur"
    if ! "$TOML_DIR/tomllib_aurum.sh" -silvestre > /dev/null; then
        echo "CAUTIO: toml/tomllib_aurum.sh -silvestre fractum - porta differentiae rubebit" >&2
    fi
fi

# metra suitae in volumen mensoris (tools/mensor_suitae.sh; praefixum
# "toml." - silva.mensurae('toml.', n) eas legit); numquam suitam frangit
source "$RADIX_DIR/tools/mensor_suitae.sh"
mensor_suitae_incipere "toml."
total=0 ; passed=0 ; failed_names=""
for test_file in "$TOML_DIR"/probationes/probatio_*.c; do
    name="$(basename "$test_file" .c)"
    if [ -n "$FILTER" ] && [[ "$name" != *"$FILTER"* ]]; then
        continue
    fi
    total=$((total + 1))
    bin="$BUILD_DIR/$name"
    echo ""
    echo "=== $name ==="
    t0=$(mensor_suitae_nunc)
    # probatio ipsa per compilatorem; nexus cum obiectis clausurae SUAE
    obj_probationis="$BUILD_DIR/probationes/$name.o"
    obj_files=""
    while IFS= read -r fons; do
        obj_files="$obj_files $BUILD_DIR/$(basename "$fons" .c).o"
    done < "$CLAUSURAE_DIR/$name.lst"
    if ! "$COMPILATOR" "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
            -c "$test_file" -o "$obj_probationis" \
        || ! clang "${GCC_FLAGS[@]}" "$obj_probationis" $obj_files -o "$bin"; then
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
echo "TOML PROBATIONES: $passed/$total praeteritae"
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
