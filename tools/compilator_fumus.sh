#!/bin/bash
# tools/compilator_fumus.sh - porta natalis bin/compilator (fabrica plan
# 2 T4): 'clang -c' per thesaurum. Thesaurus et fixa temporaria sub
# build/compilator_fumus/ (arbor fontium numquam tangitur); clang per
# involucrum NUMERANS (FABRICA_CLANG) - 'hit' = numerus immotus.
#
#   I    frigidus: lib/chorda.c -> obiectum octetim idem ac clang -c
#        nudum (vexilla eadem, directorium idem)
#   II   iterum: hit (clang non vocatus), obiectum idem
#   III  fons tactus solum (octeti idem): hit
#   IV   caput in depfile mutatum: miss; restitutum: hit (clavis
#        plena vetus servata)
#   V    OBUMBRATIO (Review Focus 2): caput eiusdem nominis in radice
#        -I priore creatum -> miss, obiectum mutatum; ablatum -> miss
#        iterum, obiectum octetim originale
#   VI   COMPILATOR alius (Review Focus 4): involucrum octetis aliis ->
#        miss semel, deinde hit
#   VII  compilatio fracta: exitus clang (1) et stderr transeunt;
#        nihil conditum (iterum: clang iterum vocatur)
#   VIII destinatio identica non rescribitur (inode idem)
#   IX   liber lectionum PRAECISUS (fabrica plan 5 T2): nulla
#        enumeratio radicis (D); caput in radice priore quaesitum (A) -
#        obumbratio per NOMEN, non per directorium totum
#   X    caput ALIENUM in radice additum: hit (clavis nomina radicis
#        non sigillat; verdicta caput novum non tangit)
#
# Exitus: 0 sanum · 1 fractum · 2 bin/compilator deest.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
C="$RADIX/bin/compilator"
[ -x "$C" ] || { echo "fumus compilatoris: bin/compilator deest (./tools/compilator_struere.sh)"; exit 2; }
source tools/vexilla.sh
B="build/compilator_fumus"
rm -rf "$B"; mkdir -p "$B/fixa/radix_prior" "$B/fixa/radix_posterior" "$B/o"
trap 'rm -rf "$B"' EXIT
export FABRICA_THESAURUS="$B/thesaurus"
N="$B/numerus"
involucrum () {   # $1 via, $2 nota (octeti diversi)
    printf '#!/bin/sh\n# %s\necho x >> "%s"\nexec /usr/bin/clang "$@"\n' "$2" "$RADIX/$N" > "$1"
    chmod +x "$1"
}
involucrum "$B/clang_a" "primus"
involucrum "$B/clang_b" "alter"
export FABRICA_CLANG="$RADIX/$B/clang_a"
numerus () { [ -f "$N" ] && wc -l < "$N" | tr -d ' ' || echo 0; }
fracta=0
V=("${VEXILLA_C89[@]}" -Iinclude)

# I: frigidus contra clang nudum
/usr/bin/clang "${V[@]}" -c lib/chorda.c -o "$B/o/nudum.o" 2> "$B/err0"; rc0=$?
"$C" "${V[@]}" -c lib/chorda.c -o "$B/o/chorda.o" 2> "$B/err"; rc=$?
n1=$(numerus)
if [ "$rc0" -eq 0 ] && [ "$rc" -eq 0 ] && [ "$n1" -eq 1 ] && cmp -s "$B/o/nudum.o" "$B/o/chorda.o"; then echo "  I    frigidus: octetim idem ac clang  OK"; else echo "  I    FRACTUM (rc=$rc0 $rc, clang $n1)"; cat "$B/err0" "$B/err"; fracta=1; fi

# II: hit
rm -f "$B/o/chorda.o"
"$C" "${V[@]}" -c lib/chorda.c -o "$B/o/chorda.o" 2> "$B/err"; rc=$?
if [ "$rc" -eq 0 ] && [ "$(numerus)" -eq 1 ] && cmp -s "$B/o/nudum.o" "$B/o/chorda.o"; then echo "  II   iterum: hit, sine clang         OK"; else echo "  II   FRACTUM (rc=$rc, clang $(numerus))"; cat "$B/err"; fracta=1; fi

# fixa: fons cum capite quoto in radice posteriore
printf '#include "caput.h"\nint functio(void) { return VALOR; }\n' > "$B/fixa/fons.c"
printf '#define VALOR 1\n' > "$B/fixa/radix_posterior/caput.h"
VF=(-Wall -O2 -I"$B/fixa/radix_prior" -I"$B/fixa/radix_posterior")
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc_f=$?
cp "$B/o/fons.o" "$B/o/fons_originale.o"
n0=$(numerus)

# III: tactus solum
sleep 1; touch "$B/fixa/fons.c"
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc=$?
if [ "$rc_f" -eq 0 ] && [ "$rc" -eq 0 ] && [ "$(numerus)" -eq "$n0" ]; then echo "  III  fons tactus solum: hit          OK"; else echo "  III  FRACTUM (rc=$rc_f $rc, clang $n0 -> $(numerus))"; cat "$B/err"; fracta=1; fi

# IV: caput mutatum -> miss; restitutum -> hit
printf '#define VALOR 2\n' > "$B/fixa/radix_posterior/caput.h"
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc1=$?; n_m=$(numerus)
printf '#define VALOR 1\n' > "$B/fixa/radix_posterior/caput.h"
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc2=$?; n_r=$(numerus)
if [ "$rc1" -eq 0 ] && [ "$n_m" -eq $((n0 + 1)) ] && [ "$rc2" -eq 0 ] && [ "$n_r" -eq "$n_m" ] && cmp -s "$B/o/fons.o" "$B/o/fons_originale.o"; then echo "  IV   caput mutatum: miss; restitutum: hit OK"; else echo "  IV   FRACTUM (rc=$rc1 $rc2, clang $n0 $n_m $n_r)"; cat "$B/err"; fracta=1; fi

# V: obumbratio per radicem -I priorem
printf '#define VALOR 3\n' > "$B/fixa/radix_prior/caput.h"
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc1=$?; n_u=$(numerus)
cmp -s "$B/o/fons.o" "$B/o/fons_originale.o"; mutatum=$?
rm -f "$B/fixa/radix_prior/caput.h"
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc2=$?; n_v=$(numerus)
if [ "$rc1" -eq 0 ] && [ "$n_u" -eq $((n_r + 1)) ] && [ "$mutatum" -ne 0 ] && [ "$rc2" -eq 0 ] && [ "$n_v" -ge "$n_u" ] && cmp -s "$B/o/fons.o" "$B/o/fons_originale.o"; then echo "  V    obumbratio: miss, deinde originale OK"; else echo "  V    FRACTUM (rc=$rc1 $rc2, clang $n_r $n_u $n_v, mutatum=$mutatum)"; cat "$B/err"; fracta=1; fi

# VI: compilator alius -> miss semel, deinde hit
export FABRICA_CLANG="$RADIX/$B/clang_b"
n_a=$(numerus)
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc1=$?; n_b=$(numerus)
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc2=$?; n_c=$(numerus)
if [ "$rc1" -eq 0 ] && [ "$n_b" -eq $((n_a + 1)) ] && [ "$rc2" -eq 0 ] && [ "$n_c" -eq "$n_b" ]; then echo "  VI   compilator alius: miss, hit    OK"; else echo "  VI   FRACTUM (rc=$rc1 $rc2, clang $n_a $n_b $n_c)"; cat "$B/err"; fracta=1; fi

# VII: compilatio fracta transit, nihil conditum
printf 'int fracta(void) { return ; \n' > "$B/fixa/fractus.c"
n_d=$(numerus)
"$C" "${VF[@]}" -c "$B/fixa/fractus.c" -o "$B/o/fractus.o" 2> "$B/err"; rc1=$?
"$C" "${VF[@]}" -c "$B/fixa/fractus.c" -o "$B/o/fractus.o" 2> "$B/err2"; rc2=$?
if [ "$rc1" -eq 1 ] && [ "$rc2" -eq 1 ] && grep -q 'error' "$B/err" && [ "$(numerus)" -eq $((n_d + 2)) ] && [ ! -e "$B/o/fractus.o" ]; then echo "  VII  compilatio fracta: exitus 1, nihil conditum OK"; else echo "  VII  FRACTUM (rc=$rc1 $rc2, clang $n_d -> $(numerus))"; cat "$B/err"; fracta=1; fi

# VIII: destinatio identica non rescribitur
ino1=$(stat -f '%i' "$B/o/fons.o")
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc=$?
ino2=$(stat -f '%i' "$B/o/fons.o")
if [ "$rc" -eq 0 ] && [ "$ino1" = "$ino2" ]; then echo "  VIII destinatio identica non rescripta OK"; else echo "  VIII FRACTUM (rc=$rc, inode $ino1 -> $ino2)"; cat "$B/err"; fracta=1; fi

# IX: liber lectionum praecisus
L="$B/liber"; rm -f "$L"
FABRICA_LECTIONES="$RADIX/$L" "$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc=$?
if [ "$rc" -eq 0 ] && ! grep -q $'^D\t' "$L" && grep -q $'^A\t'"$B/fixa/radix_prior/caput.h\$" "$L"; then echo "  IX   liber: sine D, obumbratio per nomen OK"; else echo "  IX   FRACTUM (rc=$rc)"; grep -E $'^(D|A)\t' "$L" | head -5; fracta=1; fi

# X: caput alienum additum -> hit
n_x=$(numerus)
printf '#define ALIENUM 1\n' > "$B/fixa/radix_posterior/alienum.h"
"$C" "${VF[@]}" -c "$B/fixa/fons.c" -o "$B/o/fons.o" 2> "$B/err"; rc=$?
if [ "$rc" -eq 0 ] && [ "$(numerus)" -eq "$n_x" ]; then echo "  X    caput alienum additum: hit      OK"; else echo "  X    FRACTUM (rc=$rc, clang $n_x -> $(numerus))"; cat "$B/err"; fracta=1; fi
rm -f "$B/fixa/radix_posterior/alienum.h"

if [ "$fracta" -ne 0 ]; then echo "fumus compilatoris: FRACTUM"; exit 1; fi
echo "fumus compilatoris: sanum (X/X)"
exit 0
