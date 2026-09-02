#!/bin/bash
# tools/vexilla_fumus.sh - porta natalis tools/vexilla.sh
#
# I.   tabula compilat C89 purum et REICIT C99 ('//', -pedantic -Werror
#      viva - tabula muta hic caperetur);
# II.  nullum scriptum tabulam suam declarat (declare -a GCC_FLAGS=( cum
#      lineis) - copiae nominantur;
# III. omne scriptum quod GCC_FLAGS adhibet vexilla.sh fontat.
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
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
echo
if [ "$fracta" -eq 0 ]; then echo "fumus vexilla: sanum"; exit 0; fi
echo "fumus vexilla: FRACTUM ($fracta)"; exit 1
