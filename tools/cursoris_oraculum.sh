#!/bin/bash
# tools/cursoris_oraculum.sh - ORACULUM cursoris probationum (fabrica
# plan 2; generale ex tools/toml_oraculum.sh, T5): cursor VETUS (idem
# scriptum ad REF) contra NOVUM (arbor viva) - obiecta <sub>/build
# octetim, effusiones probationum linea per lineam.
#
# EADEM ARBOR, seriatim, uterque frigidus (<sub>/build vacuatum): -g
# directorium operis in obiectum infigit (spec 2 par. XII.2), ergo
# arbor alia (worktree) octetos aequales numquam daret. Cursor vetus
# ad $SUB/.oraculum_vetus.sh ponitur (vias ex sede sua computat),
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
# MODUS RADICIS (cursor = compile_tests.sh, migratio W0ZBW gradus IV):
# build/ radicis acta, seras, binaria aliorum tenet - ergo NON vacuatur;
# deletur SOLUM quod cursor ipse struit (build/*.o, build/probationes/
# *.o), utrumque comparatur. Effusio ab 'Testing:' usque ad 'Slowest
# tests:' (colores dempti), sed VERDICTA sola comparantur (probationes
# radicis portus, tempora, inscriptiones imprimunt); involucrum tee per
# COMPILE_TESTS_EFFUSIO
# omittitur. -filtrum X probationes cursas angustat (cursor X accipit);
# bibliothecae tamen omnes compilantur, ergo obiecta tota comparantur.
#
# Usus: ./tools/cursoris_oraculum.sh <sub>/compile_probationes.sh
#         [-ref REF | -arbor REF]   (ordinarium HEAD)
#       ./tools/cursoris_oraculum.sh compile_tests.sh [-ref REF] [-filtrum X]
# Cursor novus mandata compilationis in <sub>/build/clausurae/mandata.tsv
# scribit (tools/cursor_communis.sh) - inde determinismus clang
# iudicatur; effusio per <SUB>_PROBATIONES_EFFUSIO (sine tee).
# Exitus: 0 consensus · 1 discordia · 2 nihil comparabile / usus
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
VIVA="$(pwd)"
CURSOR="${1:?usus: cursoris_oraculum.sh <sub>/compile_probationes.sh [-ref REF | -arbor REF]}"
shift
SUB="$(dirname "$CURSOR")"
[ -f "$CURSOR" ] || { echo "oraculum: cursor $CURSOR deest"; exit 2; }
EFFUSIO_VAR="$(echo "$SUB" | tr '[:lower:]/' '[:upper:]_')_PROBATIONES_EFFUSIO"
RADIX=0
if [ "$CURSOR" = "compile_tests.sh" ]; then
    RADIX=1
    EFFUSIO_VAR=COMPILE_TESTS_EFFUSIO
fi
REF=HEAD
ARBOR=""
FILTRUM=""
while [ "$#" -gt 0 ]; do
    case "$1" in
        -ref)     REF="${2:?usus: -ref REF}"; shift 2 ;;
        -arbor)   REF="${2:?usus: -arbor REF}"; ARBOR="$(mktemp -d)/arbor"; shift 2 ;;
        -filtrum) FILTRUM="${2:?usus: -filtrum X}"; shift 2 ;;
        *) echo "oraculum: argumentum ignotum $1"; exit 2 ;;
    esac
done
if [ -n "$FILTRUM" ] && [ "$RADIX" -eq 0 ]; then
    echo "oraculum: -filtrum solum cum compile_tests.sh"; exit 2
fi
if [ -n "$ARBOR" ] && [ "$RADIX" -eq 1 ]; then
    echo "oraculum: -arbor non cum compile_tests.sh (build/ radicis ignota)"; exit 2
fi
T="$(mktemp -d)"
VETUS="$SUB/.oraculum_vetus.sh"
NOVUS="$CURSOR"
if [ -n "$ARBOR" ]; then
    git worktree add --detach "$ARBOR" "$REF" > /dev/null 2>&1 \
        || { echo "oraculum $SUB: worktree ad $REF fieri nequit"; exit 2; }
    trap 'rm -rf "$T"; git -C "$VIVA" worktree remove --force "$ARBOR" > /dev/null 2>&1' EXIT
    [ -f "$ARBOR/$CURSOR" ] || {
        echo "oraculum $SUB: nihil comparabile ($REF sine $CURSOR)"; exit 2; }
    cp "$VIVA/$CURSOR" "$ARBOR/$SUB/.oraculum_novus.sh"
    mkdir -p "$ARBOR/bin"
    ln -sf "$VIVA/bin/aedilis" "$ARBOR/bin/aedilis"
    ln -sf "$VIVA/bin/compilator" "$ARBOR/bin/compilator"
    cd "$ARBOR" || exit 2
    cp $CURSOR "$VETUS"
    NOVUS="$SUB/.oraculum_novus.sh"
else
    trap 'rm -rf "$T" "$VETUS"' EXIT
    git show "$REF:$CURSOR" > "$VETUS" 2>/dev/null \
        || { echo "oraculum $SUB: $REF:$CURSOR legi nequit"; exit 2; }
fi
chmod +x "$VETUS" "$NOVUS"
if cmp -s "$VETUS" "$NOVUS"; then
    echo "oraculum $SUB: nihil comparabile (cursor novus idem ac vetus ad $REF)"
    exit 2
fi

currere () {   # $1 cursor, $2 sedes collectionis
    if [ "$RADIX" -eq 1 ]; then
        rm -f build/*.o build/probationes/*.o
    else
        rm -rf "$SUB/build"
    fi
    mkdir -p "$T/$2/o"
    # shellcheck disable=SC2086 (FILTRUM vacuum = nullum argumentum)
    env "$EFFUSIO_VAR=1" "$1" $FILTRUM > "$T/$2/effusio" 2>&1
    echo $? > "$T/$2/rc"
    cp "$SUB"/build/*.o "$T/$2/o/" 2>/dev/null
    [ "$RADIX" -eq 1 ] && cp build/probationes/*.o "$T/$2/o/" 2>/dev/null
    # LC_ALL=C: octeti non UTF-8 in effusione awk multibyte necabant
    # (towc, cursus radicis)
    LC_ALL=C awk -v radix="$RADIX" '
         { gsub(/\033\[[0-9;]*m/, "") }
         radix == 0 && /^=== probatio/ { on = 1 }
         radix == 1 && /Testing: / { on = 1 }
         radix == 1 && /^Slowest tests:/ { on = 0 }
         /^Total Time:/ { next }
         /^Tardissimae:/ { tard = 1; next }
         tard && /^=+$/ { tard = 0 }
         tard { next }
         on { gsub(/ \([0-9.]+s(, exitus [0-9]+)?\)/, "");
              sub(/ +pars [0-9.]+ ms.*$/, ""); print }' \
        "$T/$2/effusio" > "$T/$2/plena"
    # RADIX: probationes ipsae signa cursus-propria imprimunt (inscriptiones
    # ASLR, portus, pid, UUID, tempora, numeri callback - inventum
    # 2026-10-03, tres cursus) - ergo VERDICTA comparantur (linea per
    # probationem + summa), effusio plena solum numeratur (nota)
    if [ "$RADIX" -eq 1 ]; then
        grep -E 'TEST PASSED|TEST FAILED|COMPILATION FAILED|SINE VERDICTO|RETICULARIS EXCLUSUM|^Tests (Total|Passed|Failed):|^Failed:' \
            "$T/$2/plena" > "$T/$2/probationes"
    else
        cp "$T/$2/plena" "$T/$2/probationes"
    fi
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
# mandatum obiecti ex mandata.tsv cursoris novi (cursor_communis.sh)
mandatum_obiecti () {
    awk -F'\t' -v o="$1" '{ n = $1; sub(/.*\//, "", n); if (n == o) { print $2; exit } }' \
        "$SUB/build/clausurae/mandata.tsv" 2>/dev/null
}
clang_deterministicus () {   # $1 argumenta (sine -o): V cursus clang nudi
    local k
    for k in 1 2 3 4 5; do
        # shellcheck disable=SC2086 (argumenta consulto scissa)
        /usr/bin/clang $1 -o "$T/det.o" 2>/dev/null
        shasum -a 256 "$T/det.o" | cut -c1-64
    done | sort -u | wc -l | tr -d ' '
}
differentes=0
non_deterministica=""
for o in $(comm -12 "$T/v.lst" "$T/n.lst"); do
    if ! cmp -s "$T/vetus/o/$o" "$T/novus/o/$o"; then
        mandatum=$(mandatum_obiecti "$o")
        if [ -n "$mandatum" ] && [ "$(clang_deterministicus "$mandatum")" -gt 1 ]; then
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
if [ "$RADIX" -eq 1 ]; then
    plenae=$(diff "$T/vetus/plena" "$T/novus/plena" | grep -c '^>')
    echo "nota: effusio plena $(wc -l < "$T/novus/plena" | tr -d ' ') lineae, $plenae differunt (signa cursus-propria; verdicta comparata)"
fi
# porta tacita = porta mortua: nihil collatum numquam 'consensus'
if [ "$communes" -eq 0 ] || [ "$lineae" -eq 0 ]; then
    echo "oraculum $SUB ($REF): nihil comparabile (obiecta communia $communes, lineae $lineae)"
    exit 2
fi
echo "oraculum $SUB ($REF${ARBOR:+, arbor}): obiecta communia $communes (differentia $differentes), lineae probationum $lineae"
if [ "$discordia" -ne 0 ]; then echo "oraculum $SUB: DISCORDIA"; exit 1; fi
echo "oraculum $SUB: consensus"
exit 0
