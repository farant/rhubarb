#!/bin/bash
# tools/sera_fumus.sh - porta natalis tools/sera.sh (sera cursorum)
#
# I.    capere: directorium cum radix (haec arbor) et pid ($$);
#       dimittere tollit, SERA_TENTA abit
# II.   tenta a tenente VIVO: capere tectum II -> 1 intra V s,
#       nuntius pid tenentis nominat, sera intacta
# III.  vetus per pid mortuum -> capta statim (crusta filia), et
#       trap EXIT eius seram liberam relinquit
# IV.   vetus per radicem alienam (tenens vivus) -> capta statim
# V.    reentrantia: SERA_TENTA == via -> 0 statim, dimittere filii
#       seram avi non tollit
# VI.   crusta quae seram capit et 'exit 1' facit: sera libera post
# VII.  via sine suffixo .sera refutata (nihil creatum)
# VIII. cursores omnes (compile_tests.sh, */compile_probationes.sh,
#       oratio/*.sh, silva/nexus.sh) sera.sh fontant et sera_capere
#       vocant
# IX.   integratio VIVA: sera orationis tenta -> oratio/vocabula.sh
#       -nova (SERA_TECTUM=1) exitus 2 cum 'sera' in effusu; radix:
#       compile_tests.sh item (involucrum tee praeteritum ne acta
#       cursus ultimi obruantur)
# X.    speculum Pythonicum: silva.sera(via) tenet -> sera_capere
#       crustae 1 (radix eadem, pid vivus: NON furata); intra 'with'
#       filius crustae cum SERA_TENTA hereditario reentrat; post
#       'with' sera abiit
# XI.   subshelli ($(...), ( ), ( ) &) seram NON dimittunt - trap EXIT
#       in crusta principali sola (bash 3.2 mensuratum 2026-09-07)
# XII.  involucra orationis seram in exitu dimittunt: nullum 'exec
#       "$BIN"' (exec crustam substituit, trap EXIT numquam currit -
#       sera cum pid mortuo relicta: inventum eodem die), verba.sh
#       vivum cursum sine reliquia
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
source tools/sera.sh
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
T=$(mktemp -d)
HOLD=""
mundare () { [ -n "$HOLD" ] && kill "$HOLD" 2>/dev/null; rm -rf "$T"; }
trap mundare EXIT
S="$T/probatio.sera"

echo "--- I. capere / dimittere ---"
sera_capere "$S" 1; credo $? "capere: 0"
[ -d "$S" ] && [ "$(cat "$S/radix")" = "$RADIX" ] && [ "$(cat "$S/pid")" = "$$" ]; credo $? "sera: radix haec arbor, pid \$\$"
[ "${SERA_TENTA:-}" = "$S" ]; credo $? "SERA_TENTA exportata"
sera_dimittere; [ ! -d "$S" ] && [ -z "${SERA_TENTA:-}" ]; credo $? "dimittere: sera abiit, SERA_TENTA abiit"

echo "--- II. tenta a tenente vivo ---"
mkdir "$S"; echo "$RADIX" > "$S/radix"; sleep 60 & HOLD=$!; echo "$HOLD" > "$S/pid"
t0=$(date +%s)
out=$( (sera_capere "$S" 2) 2>&1 ); rc=$?
t1=$(date +%s)
[ "$rc" -eq 1 ]; credo $? "capere tenta: 1 (rc $rc)"
[ $((t1 - t0)) -le 5 ]; credo $? "capere tenta: intra V s ($((t1 - t0)) s)"
echo "$out" | grep -q "pid $HOLD"; credo $? "nuntius tenentem nominat (pid $HOLD)"
echo "$out" | grep -q "NIHIL cursum"; credo $? "nuntius 'NIHIL cursum'"
[ -d "$S" ] && [ "$(cat "$S/pid")" = "$HOLD" ]; credo $? "sera tenentis intacta"
kill "$HOLD" 2>/dev/null; wait "$HOLD" 2>/dev/null; HOLD=""; rm -rf "$S"

echo "--- III. vetus per pid mortuum; trap EXIT ---"
mkdir "$S"; echo "$RADIX" > "$S/radix"; echo 999999 > "$S/pid"
out=$(bash -c 'source tools/sera.sh; sera_capere "$1" 2 || exit 9; [ "$(cat "$1/pid")" = "$$" ] && echo PID_MEUS; exit 0' _ "$S" 2>&1); rc=$?
[ "$rc" -eq 0 ] && echo "$out" | grep -q PID_MEUS; credo $? "sera vetus (pid mortuus) capta a filio, pid eius scriptus"
echo "$out" | grep -q "vetus"; credo $? "derelictio nuntiata"
[ ! -d "$S" ]; credo $? "trap EXIT filii seram liberam reliquit"

echo "--- IV. vetus per radicem alienam ---"
mkdir "$S"; echo "/nusquam/arbor/alia" > "$S/radix"; sleep 60 & HOLD=$!; echo "$HOLD" > "$S/pid"
out=$(bash -c 'source tools/sera.sh; sera_capere "$1" 2 || exit 9; exit 0' _ "$S" 2>&1); rc=$?
[ "$rc" -eq 0 ] && echo "$out" | grep -q "vetus"; credo $? "sera arboris alienae (tenens vivus) derelicta et capta"
kill "$HOLD" 2>/dev/null; wait "$HOLD" 2>/dev/null; HOLD=""; rm -rf "$S"

echo "--- V. reentrantia ---"
mkdir "$S"; echo "$RADIX" > "$S/radix"; echo "$$" > "$S/pid"
out=$(SERA_TENTA="$S" bash -c 'source tools/sera.sh; sera_capere "$1" 0 && echo REENTRATA; sera_dimittere; exit 0' _ "$S" 2>&1); rc=$?
[ "$rc" -eq 0 ] && echo "$out" | grep -q REENTRATA; credo $? "filius cum SERA_TENTA reentrat statim (tectum 0)"
[ -d "$S" ] && [ "$(cat "$S/pid")" = "$$" ]; credo $? "dimittere filii seram avi non tollit"
rm -rf "$S"

echo "--- VI. exit 1 post capere ---"
bash -c 'source tools/sera.sh; sera_capere "$1" 1 || exit 9; exit 1' _ "$S" 2>/dev/null; rc=$?
[ "$rc" -eq 1 ] && [ ! -d "$S" ]; credo $? "crusta exiens 1 seram liberam relinquit"

echo "--- VII. via sine .sera ---"
out=$( (sera_capere "$T/nonsera" 1) 2>&1 ); rc=$?
[ "$rc" -eq 1 ] && [ ! -e "$T/nonsera" ] && echo "$out" | grep -q "refutata"; credo $? "via sine suffixo refutata, nihil creatum"

echo "--- VIII. cursores omnes fontant ---"
sine=""
for s in compile_tests.sh $(git ls-files '*/compile_probationes.sh' 'oratio/*.sh' 'silva/nexus.sh'); do
    if ! grep -q 'tools/sera.sh' "$s" || ! grep -q 'sera_capere' "$s"; then sine="$sine $s"; fi
done
[ -z "$sine" ]; credo $? "cursores et involucra sera.sh fontant et sera_capere vocant"
[ -n "$sine" ] && echo "   sine sera:$sine"

echo "--- IX. integratio viva ---"
SO="$RADIX/oratio/build/cursor.sera"
mkdir -p "$RADIX/oratio/build"
[ -d "$SO" ] && sera_vetus "$SO" && rm -rf "$SO"     # reliquia (pid mortuus) tollitur
if mkdir "$SO" 2>/dev/null; then
    echo "$RADIX" > "$SO/radix"; sleep 60 & HOLD=$!; echo "$HOLD" > "$SO/pid"
    out=$(SERA_TECTUM=1 ./oratio/vocabula.sh -nova 2>&1); rc=$?
    [ "$rc" -eq 2 ] && echo "$out" | grep -q "sera.*pid $HOLD"; credo $? "oratio/vocabula.sh -nova sub sera tenta: exitus 2 (rc $rc), tenens nominatus"
    kill "$HOLD" 2>/dev/null; wait "$HOLD" 2>/dev/null; HOLD=""; rm -rf "$SO"
else
    credo 1 "sera orationis iam tenta ab alio ($SO) - integratio non probata"
fi
SR="$RADIX/build/cursor.sera"
mkdir -p "$RADIX/build"
[ -d "$SR" ] && sera_vetus "$SR" && rm -rf "$SR"
if mkdir "$SR" 2>/dev/null; then
    echo "$RADIX" > "$SR/radix"; sleep 60 & HOLD=$!; echo "$HOLD" > "$SR/pid"
    out=$(COMPILE_TESTS_EFFUSIO=1 SERA_TECTUM=1 ./compile_tests.sh nemo_probatio_xyz 2>&1); rc=$?
    [ "$rc" -eq 2 ] && echo "$out" | grep -q "sera.*pid $HOLD"; credo $? "compile_tests.sh sub sera tenta: exitus 2 (rc $rc), tenens nominatus"
    kill "$HOLD" 2>/dev/null; wait "$HOLD" 2>/dev/null; HOLD=""; rm -rf "$SR"
else
    credo 1 "sera radicis iam tenta ab alio ($SR) - integratio non probata"
fi

echo "--- X. speculum Pythonicum ---"
out=$(python3 - "$S" <<'PY' 2>&1
import os, subprocess, sys
sys.path.insert(0, 'pythonica')
import silva
via = sys.argv[1]
with silva.sera(via, tectum=1):
    assert os.path.isdir(via) and open(via + '/pid').read().strip() == str(os.getpid())
    assert open(via + '/radix').read().strip() == silva.RADIX
    r = subprocess.run(['bash', '-c', 'unset SERA_TENTA; source tools/sera.sh; sera_capere "$1" 1', '_', via],
                       capture_output=True, text=True)
    print('CRUSTA_RC', r.returncode, 'NOMINAT' if ('pid %d' % os.getpid()) in r.stderr else 'TACET')
    r2 = subprocess.run(['bash', '-c', 'source tools/sera.sh; sera_capere "$1" 0 && echo REENTRATA; sera_dimittere', '_', via],
                        capture_output=True, text=True)
    print('FILIUS', r2.stdout.strip(), 'SERA_MANET' if os.path.isdir(via) else 'SERA_ABIIT')
print('POST', 'ABIIT' if not os.path.exists(via) else 'MANET', 'TENTA_ABIIT' if 'SERA_TENTA' not in os.environ else 'TENTA_MANET')
PY
); rc=$?
[ "$rc" -eq 0 ] && echo "$out" | grep -q "CRUSTA_RC 1 NOMINAT"; credo $? "silva.sera tenet: sera_capere crustae 1, pid Pythonis nominatus"
echo "$out" | grep -q "FILIUS REENTRATA SERA_MANET"; credo $? "filius crustae cum SERA_TENTA Pythonis reentrat, sera manet"
echo "$out" | grep -q "POST ABIIT TENTA_ABIIT"; credo $? "post 'with': sera abiit, SERA_TENTA abiit"
[ "$rc" -eq 0 ] || echo "$out" | tail -5

echo "--- XI. subshelli seram non dimittunt (bash 3.2: trap EXIT in crusta principali sola) ---"
out=$(bash -c 'source tools/sera.sh; sera_capere "$1" 1 || exit 9; x=$(echo sub); (true); (sleep 0.1) & wait; [ -d "$1" ] && echo MANET; exit 0' _ "$S" 2>&1); rc=$?
[ "$rc" -eq 0 ] && echo "$out" | grep -q MANET && [ ! -d "$S" ]; credo $? "sera post \$(...), ( ) et ( ) & manet, post exitum principalis abiit"

echo "--- XII. involucra seram in exitu dimittunt (exec trap EXIT praeterit) ---"
cum_exec=$(grep -l '^[^#]*exec "\$BIN"' $(git ls-files 'oratio/*.sh') 2>/dev/null || true)
[ -z "$cum_exec" ]; credo $? "nullum involucrum orationis 'exec \$BIN' (trap EXIT viveret)"
[ -n "$cum_exec" ] && echo "   cum exec: $cum_exec"
printf 'Puella amat.\n' > "$T/verba.txt"
./oratio/verba.sh "$T/verba.txt" > /dev/null 2>&1; rc=$?
[ "$rc" -eq 0 ] && [ ! -d "$SO" ]; credo $? "oratio/verba.sh cucurrit (rc $rc) et seram dimisit"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus sera: sanum"; exit 0; fi
echo "fumus sera: FRACTUM ($fracta)"; exit 1
