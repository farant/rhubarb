#!/bin/bash
# tools/toml_oraculum.sh - ORACULUM cursoris toml (fabrica plan 2 T5):
# cursor VETUS (toml/compile_probationes.sh ad REF) contra NOVUM (arbor
# viva) - obiecta octetim, effusiones probationum linea per lineam.
#
# EADEM ARBOR, seriatim, uterque frigidus (toml/build vacuatum): -g
# directorium operis in obiectum infigit (spec 2 par. XII.2), ergo
# arbor alia (worktree) octetos aequales numquam daret. Cursor vetus
# ad toml/.oraculum_vetus.sh ponitur (vias ex sede sua computat),
# deinde deletur.
#
# Copia obiectorum: SOLUM IN NOVO = FRACTUM (novus compilat quod vetus
# non); SOLUM IN VETERE = laxitas indicis manu scripti, nominatur.
# Effusio: ab '=== probatio' usque ad finem, tempora dempta
# ('(N.NNs)', tabula 'Tardissimae', 'pars X ms ...' metrorum).
#
# CLANG NON DETERMINISTICUS (inventum 2026-10-02): Apple clang 16 fontem
# toml/fontes/toml_scalaris.c non deterministice compilat (IV obiecta in
# X cursibus nudis; -fno-vectorize id sanat - vectorizator ansarum).
# Discordia octetorum ergo iudicatur: fons idem per clang NUDUM V
# compilatur; obiecta plura = culpa compilatoris (nominata, non
# discordia thesauri); unum = DISCORDIA vera.
#
# -arbor REF: STATUS ARBORIS ad REF (worktree temporarium): cursor vetus
# = REF ipsius, novus = cursor ARBORIS VIVAE illuc copiatus; bin/aedilis
# et bin/compilator vivi nectuntur. Ambo in eadem arbore (via eadem).
#
# Usus: ./tools/toml_oraculum.sh [-ref REF | -arbor REF]   (ordinarium HEAD)
# Exitus: 0 consensus · 1 discordia · 2 nihil comparabile / usus
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
VIVA="$(pwd)"
REF=HEAD
ARBOR=""
case "${1:-}" in
    -ref)   REF="${2:?usus: -ref REF}" ;;
    -arbor) REF="${2:?usus: -arbor REF}"; ARBOR="$(mktemp -d)/arbor" ;;
esac
T="$(mktemp -d)"
VETUS="toml/.oraculum_vetus.sh"
NOVUS="toml/compile_probationes.sh"
if [ -n "$ARBOR" ]; then
    git worktree add --detach "$ARBOR" "$REF" > /dev/null 2>&1 \
        || { echo "oraculum toml: worktree ad $REF fieri nequit"; exit 2; }
    trap 'rm -rf "$T"; git -C "$VIVA" worktree remove --force "$ARBOR" > /dev/null 2>&1' EXIT
    [ -f "$ARBOR/toml/compile_probationes.sh" ] || {
        echo "oraculum toml: nihil comparabile ($REF sine toml/compile_probationes.sh)"; exit 2; }
    cp "$VIVA/toml/compile_probationes.sh" "$ARBOR/toml/.oraculum_novus.sh"
    mkdir -p "$ARBOR/bin"
    ln -sf "$VIVA/bin/aedilis" "$ARBOR/bin/aedilis"
    ln -sf "$VIVA/bin/compilator" "$ARBOR/bin/compilator"
    cd "$ARBOR" || exit 2
    cp toml/compile_probationes.sh "$VETUS"
    NOVUS="toml/.oraculum_novus.sh"
else
    trap 'rm -rf "$T" "$VETUS"' EXIT
    git show "$REF:toml/compile_probationes.sh" > "$VETUS" 2>/dev/null \
        || { echo "oraculum toml: $REF:toml/compile_probationes.sh legi nequit"; exit 2; }
fi
chmod +x "$VETUS" "$NOVUS"
if cmp -s "$VETUS" "$NOVUS"; then
    echo "oraculum toml: nihil comparabile (cursor novus idem ac vetus ad $REF)"
    exit 2
fi

currere () {   # $1 cursor, $2 sedes collectionis
    rm -rf toml/build
    mkdir -p "$T/$2/o"
    TOML_PROBATIONES_EFFUSIO=1 "$1" > "$T/$2/effusio" 2>&1
    echo $? > "$T/$2/rc"
    cp toml/build/*.o "$T/$2/o/" 2>/dev/null
    awk '/^=== probatio/ { on = 1 }
         /^Tardissimae:/ { tard = 1; next }
         tard && /^=+$/ { tard = 0 }
         tard { next }
         on { gsub(/ \([0-9.]+s(, exitus [0-9]+)?\)/, "");
              sub(/ +pars [0-9.]+ ms.*$/, ""); print }' \
        "$T/$2/effusio" > "$T/$2/probationes"
}

currere "./$VETUS" vetus
currere "./$NOVUS" novus

discordia=0
rc_v=$(cat "$T/vetus/rc"); rc_n=$(cat "$T/novus/rc")
if [ "$rc_v" != "$rc_n" ]; then
    echo "DISCORDIA exitus: vetus $rc_v, novus $rc_n"; discordia=1
fi
(cd "$T/vetus/o" && ls) | sort > "$T/v.lst"
(cd "$T/novus/o" && ls) | sort > "$T/n.lst"
solum_novo=$(comm -13 "$T/v.lst" "$T/n.lst")
solum_vetere=$(comm -23 "$T/v.lst" "$T/n.lst")
if [ -n "$solum_novo" ]; then
    echo "DISCORDIA obiecta SOLUM IN NOVO: $(echo $solum_novo)"; discordia=1
fi
[ -n "$solum_vetere" ] && echo "nota: obiecta solum in vetere (laxitas indicis): $(echo $solum_vetere)"
# fons obiecti ex clausuris cursoris novi (nomen basis + .c)
fons_obiecti () {
    cat toml/build/clausurae/*.lst 2>/dev/null | sort -u \
        | awk -v b="${1%.o}.c" '{ n = $0; sub(/.*\//, "", n); if (n == b) { print; exit } }'
}
clang_deterministicus () {   # $1 fons: V cursus clang nudi, vexilla cursoris
    local fons="$1" k
    source "$VIVA/tools/vexilla.sh"
    for k in 1 2 3 4 5; do
        clang "${VEXILLA_C89[@]}" "-I$PWD/toml/fontes" "-I$PWD/include" \
            "-I$PWD/materia/fontes" "-I$PWD/toml/probationes" \
            -c "$PWD/$fons" -o "$T/det.o" 2>/dev/null
        shasum -a 256 "$T/det.o" | cut -c1-64
    done | sort -u | wc -l | tr -d ' '
}
differentes=0
non_deterministica=""
for o in $(comm -12 "$T/v.lst" "$T/n.lst"); do
    if ! cmp -s "$T/vetus/o/$o" "$T/novus/o/$o"; then
        fons=$(fons_obiecti "$o")
        if [ -n "$fons" ] && [ "$(clang_deterministicus "$fons")" -gt 1 ]; then
            non_deterministica="$non_deterministica $o"
        else
            echo "DISCORDIA octeti: $o"; differentes=$((differentes + 1)); discordia=1
        fi
    fi
done
[ -n "$non_deterministica" ] && echo "nota: clang NON DETERMINISTICUS (culpa compilatoris, non thesauri):$non_deterministica"
communes=$(comm -12 "$T/v.lst" "$T/n.lst" | wc -l | tr -d ' ')
if ! diff -q "$T/vetus/probationes" "$T/novus/probationes" > /dev/null; then
    echo "DISCORDIA effusionis probationum:"
    diff "$T/vetus/probationes" "$T/novus/probationes" | head -20
    discordia=1
fi
lineae=$(wc -l < "$T/novus/probationes" | tr -d ' ')
# porta tacita = porta mortua: nihil collatum numquam 'consensus'
if [ "$communes" -eq 0 ] || [ "$lineae" -eq 0 ]; then
    echo "oraculum toml ($REF): nihil comparabile (obiecta communia $communes, lineae $lineae)"
    exit 2
fi
echo "oraculum toml ($REF${ARBOR:+, arbor}): obiecta communia $communes (differentia $differentes), lineae probationum $lineae"
if [ "$discordia" -ne 0 ]; then echo "oraculum toml: DISCORDIA"; exit 1; fi
echo "oraculum toml: consensus"
exit 0
