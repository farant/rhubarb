#!/bin/bash
# <cursus titulus="tools/vexilla_fumus.sh" versio="I"/>
# CHASSIS CURSUUM (analytica T6): cursus in annalibus
source "$(dirname "${BASH_SOURCE[0]}")/cursus.sh"
# tools/vexilla_fumus.sh - porta natalis tools/vexilla.sh
#
# I.   tabula compilat C89 purum et REICIT C99 ('//', -pedantic -Werror
#      viva - tabula muta hic caperetur);
# II.  nullum scriptum tabulam suam declarat (declare -a GCC_FLAGS=( cum
#      lineis) - copiae nominantur;
# III. omne scriptum quod GCC_FLAGS adhibet vexilla.sh fontat.
# IV.  RADICES (cursores ad gradus T6): 'bin/aedilis --radices' ==
#      <inclusa> aedilis.stml (ordine, lectio altera independens);
# V.   vexilla_radices PRAEFIXUM -> RADICES_C89 ("-I" + praefixum +
#      radix), relativa et praefixa;
# VII. SIGILLUM (budget B2 J10, arbor ficta): fontatio prima sigillum
#      scribit et latina.h tangit; iterata non tangit; tabula MUTATA
#      cum VEXILLA_CKSUM tabulae veteris exportato TAMEN tangit (cache
#      cksum tabulam mutatam numquam celat); via relativa radicem
#      eandem dat.
# VI.  RATCHET radicum manu scriptarum: scripta cum '-I' manu ==
#      tools/radices_manu.tsv utrimque - scriptum NOVUM cum radicibus
#      manu RUBET; scriptum in tabula sine eis RUBET (linea delenda) -
#      tabula tantum decrescit.
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="vexilla" genus="praeiudicium" tegit="*.sh" legit="*unitas.stml include/unitates*.h"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
source tools/vexilla.sh
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT

[ "${#VEXILLA_C89[@]}" -ge 10 ]; credo $? "tabula VEXILLA_C89 (${#VEXILLA_C89[@]} vexilla)"
printf '#include "latina.h"\nvacuum f(vacuum);\nvacuum f(vacuum) { redde; }\n' > "$T/purum.c"
clang "${VEXILLA_C89[@]}" -Iinclude -c "$T/purum.c" -o "$T/purum.o" 2>/dev/null; credo $? "C89 purum compilat"
printf '#include "latina.h"\n// C99\nvacuum f(vacuum);\nvacuum f(vacuum) { redde; }\n' > "$T/c99.c"
clang "${VEXILLA_C89[@]}" -Iinclude -c "$T/c99.c" -o "$T/c99.o" 2>/dev/null; [ $? -ne 0 ]; credo $? "C99 '//' REICITUR (-pedantic -Werror vivus)"

copiae=$(grep -l '^declare -a GCC_FLAGS=($' $(git ls-files '*.sh') 2>/dev/null | grep -v '^tools/vexilla.sh$' || true)
[ -z "$copiae" ]; credo $? "nullum scriptum tabulam suam declarat"
[ -n "$copiae" ] && echo "$copiae" | sed 's/^/    copia: /'
sine=""
for s in $(git ls-files '*.sh'); do
    case "$s" in tools/vexilla.sh|tools/vexilla_fumus.sh) continue ;; esac
    if grep -q 'GCC_FLAGS' "$s" && ! grep -q 'tools/vexilla.sh\|/vexilla.sh' "$s"; then sine="$sine $s"; fi
done
[ -z "$sine" ]; credo $? "omne scriptum cum GCC_FLAGS vexilla.sh fontat"
[ -n "$sine" ] && echo "   sine fonte:$sine"
# IV. radices ex aedilis (lector verus) == <inclusa> (lectio altera)
exspectatae=$(sed -n '/<inclusa>/,/<\/inclusa>/p' aedilis.stml | sed -n 's/^ *<via (>\(.*\)$/\1/p')
datae=$(bin/aedilis --radices 2>/dev/null)
[ -n "$datae" ] && [ "$datae" = "$exspectatae" ]; credo $? "bin/aedilis --radices == <inclusa> aedilis.stml ($(echo "$datae" | grep -c .) radices)"

# V. vexilla_radices: relativa et praefixa
RADICES_C89=()
vexilla_radices "" 2>/dev/null
n_rel=${#RADICES_C89[@]}; prima_rel="${RADICES_C89[0]:-}"
vexilla_radices "/x/" 2>/dev/null
prima_abs="${RADICES_C89[0]:-}"
[ "$n_rel" -eq "$(echo "$exspectatae" | grep -c .)" ] && [ "$prima_rel" = "-Iinclude" ] && [ "$prima_abs" = "-I/x/include" ]; credo $? "vexilla_radices: $n_rel radices, '$prima_rel', '$prima_abs'"

# VI. RATCHET: scripta cum radicibus manu == tools/radices_manu.tsv
#     (scopus per iudicem 'vexilla_scriptorum' unitates.stml: fixturae
#     et archivata non iudicantur)
manu=""
for s in $(git ls-files '*.sh' | ./tools/iudex_unitatum.sh vexilla_scriptorum); do
    case "$s" in tools/vexilla.sh|tools/vexilla_fumus.sh) continue ;; esac
    grep -qE '(^|[ "(])-I["$a-zA-Z/.]|"-I' "$s" && manu="$manu
$s"
done
manu=$(echo "$manu" | grep . | sort)
tabula=$(grep -v '^#' tools/radices_manu.tsv 2>/dev/null | cut -f1 | grep . | sort)
nova=$(comm -23 <(echo "$manu") <(echo "$tabula"))
stalae=$(comm -13 <(echo "$manu") <(echo "$tabula"))
[ -n "$tabula" ] && [ -z "$nova" ]; credo $? "nullum scriptum NOVUM radices manu (tabula: $(echo "$tabula" | grep -c .) debita)"
[ -n "$nova" ] && echo "$nova" | sed 's/^/    radices manu, non in tabula (vexilla_radices adhibe): /'
[ -z "$stalae" ]; credo $? "tabula radices_manu sine lineis stalis"
[ -n "$stalae" ] && echo "$stalae" | sed 's/^/    sine radicibus manu iam - linea delenda: /'
# VII. sigillum et tactus latina.h (arbor ficta: copia vexilla.sh)
VT="$T/sigillum"
mkdir -p "$VT/tools" "$VT/include" || exit 2
cp tools/vexilla.sh "$VT/tools/vexilla.sh" || exit 2
: > "$VT/include/latina.h"
_tactus () {   # 0 si latina.h tacta (mtime ex praeterito mutatum)
    local m
    touch -t 202001010000 "$VT/include/latina.h"
    env -u VEXILLA_CKSUM ${1:+VEXILLA_CKSUM="$1"} bash -c "source '$VT/tools/vexilla.sh'" 2>/dev/null
    m=$(stat -f %m "$VT/include/latina.h")
    touch -t 202001010000 "$VT/praeteritum"
    [ "$m" != "$(stat -f %m "$VT/praeteritum")" ]
}
_tactus; credo $? "VII fontatio prima: latina.h tacta"
[ -s "$VT/build/vexilla.sigillum" ]; credo $? "VII sigillum scriptum"
_tactus; [ $? -ne 0 ]; credo $? "VII fontatio iterata: latina.h intacta"
notum=$(env -u VEXILLA_CKSUM bash -c "source '$VT/tools/vexilla.sh'; printf '%s' \"\$VEXILLA_CKSUM\"" 2>/dev/null)
sed -i '' 's/^    "-g"$/    "-g"\n    "-DVEXILLUM_FICTUM"/' "$VT/tools/vexilla.sh"
grep -q VEXILLUM_FICTUM "$VT/tools/vexilla.sh" && _tactus "$notum"
credo $? "VII tabula mutata cum cksum vetere exportato: latina.h TAMEN tacta"
radix_rel=$(cd "$VT/tools" && env -u VEXILLA_CKSUM bash -c 'source ./vexilla.sh; printf "%s" "$_vexilla_radix"' 2>/dev/null)
[ "$radix_rel" = "$(cd "$VT" && pwd)" ]; credo $? "VII via relativa: radix eadem ($radix_rel)"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus vexilla: sanum"; exit 0; fi
echo "fumus vexilla: FRACTUM ($fracta)"; exit 1
