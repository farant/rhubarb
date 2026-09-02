#!/bin/bash

# formator_intra_fumus.sh - porta natalis modi -intra (CLI).
#   I   -scribere -intra b: b formata (prototypum + definitio), a
#       octetim intacta; residuum intra ambitum nullum -> exitus 0
#   II  -intra ignota: stderr nomen dat, stdout RECUSATUM, exitus 2,
#       plagula intacta
#   III -intra a -machina: nullus ordo in extentis b (lineae >= 10)
#   IV  -intra a -intra b == -scribere sine ambitu (octetim)
# Machina ipsa in probatio_silva_formator probatur; hic CLI sola.
# Usage: ./silva/formator_intra_fumus.sh      exit 0 sanum | 1 fractum

set -u
RADIX="$(git rev-parse --show-toplevel)" || exit 2
cd "$RADIX" || exit 2
T=build/formator_intra_fumus
mkdir -p "$T"
fracta=0

printf 'vacuum a(vacuum);\nvacuum b(vacuum);\n\n/* a */\nvacuum\na(vacuum)\n{\n    redde;\n}\n\n\n\n/* b */\nvacuum\nb(vacuum)\n{\n    redde;\n}\n' > "$T/fons.c"

cp "$T/fons.c" "$T/b.c"
out=$(./silva/formator.sh "$T/b.c" -scribere -intra b 2>/dev/null); rc=$?
if [ "$rc" -eq 0 ] && grep -q '^a(vacuum)$' "$T/b.c" && grep -q '^vacuum a(vacuum);$' "$T/b.c" \
   && grep -q '^b (vacuum)$' "$T/b.c" && grep -q '^b (vacuum);$' "$T/b.c" \
   && [ "$(grep -c '^$' "$T/b.c")" -eq 2 ]; then echo "  I   -scribere -intra b: a intacta      OK"; else echo "  I   FRACTUM (rc=$rc)"; cat "$T/b.c"; fracta=1; fi

cp "$T/fons.c" "$T/ignota.c"
err=$(./silva/formator.sh "$T/ignota.c" -scribere -intra nemo 2>&1 >"$T/ignota.out"); rc=$?
if [ "$rc" -eq 2 ] && printf '%s\n' "$err" | grep -q 'functio ignota: nemo' && grep -q '^RECUSATUM$' "$T/ignota.out" \
   && cmp -s "$T/fons.c" "$T/ignota.c"; then echo "  II  -intra ignota: RECUSATUM, intacta   OK"; else echo "  II  FRACTUM (rc=$rc)"; printf '%s\n' "$err"; cat "$T/ignota.out"; fracta=1; fi

rows=$(./silva/formator.sh "$T/fons.c" -intra a -machina 2>/dev/null | awk -F'\t' '$1!~/^#/ {print $2}')
if [ -n "$rows" ] && ! printf '%s\n' "$rows" | awk '$1>=10 {f=1} END{exit !f}'; then echo "  III -intra a: nihil in extentis b       OK"; else echo "  III FRACTUM"; printf '%s\n' "$rows"; fracta=1; fi

cp "$T/fons.c" "$T/ambo.c"; cp "$T/fons.c" "$T/totum.c"
./silva/formator.sh "$T/ambo.c" -scribere -intra a -intra b >/dev/null 2>&1
./silva/formator.sh "$T/totum.c" -scribere >/dev/null 2>&1
if cmp -s "$T/ambo.c" "$T/totum.c" && ! cmp -s "$T/ambo.c" "$T/fons.c"; then echo "  IV  -intra a -intra b == totum          OK"; else echo "  IV  FRACTUM"; diff "$T/ambo.c" "$T/totum.c"; fracta=1; fi

[ "$fracta" -ne 0 ] && { echo "fumus intra: FRACTUM"; exit 1; }
echo "fumus intra: sanum (IV/IV)"; exit 0
