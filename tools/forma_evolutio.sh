#!/bin/bash

# forma_evolutio.sh - evolutio formae domus per directorium (gradus II
# decreti 'forma non est res cui editor attendat', 2026-09-01).
#
# Usage:
#   ./tools/forma_evolutio.sh <dir>            census (lint, -machina)
#   ./tools/forma_evolutio.sh <dir> -scribere  scriptura + oraculum
#                                              differre per plagulam
# Plagulae: git ls-files <dir> *.c *.h, SINE fixa/ vendor/ build/
# amalgama/ scratchpad/ capsula_*.[ch]; exclusiones formatoris ipsae
# clamant (exempta). Capita: -capita pro quoque directorio capitum
# intra <dir> (typi visibiles - R7 sine eis exemptos facit).
# -scribere: post scripturam quaeque plagula mutata contra HEAD per
# ./silva/differre.sh iudicatur - substantiva/documentaria = FRACTUM
# (exitus 1), cosmetica omnia = sanum. Suites per manum, commissio per
# manum (--no-verify licet: oraculum = differre, non examen).
# Exitus: 0 sanum | 1 fractum | 2 usus

set -u
RADIX="$(git rev-parse --show-toplevel)" || exit 2
cd "$RADIX" || exit 2
DIR="${1:-}"; MODUS="${2:-}"
[ -z "$DIR" ] && { echo "usus: $0 <dir> [-scribere]" >&2; exit 2; }
[ -d "$DIR" ] || { echo "directorium absens: $DIR" >&2; exit 2; }
T=build/forma_evolutio; mkdir -p "$T"

FILES=$(git ls-files "$DIR" | grep -E '\.(c|h)$' \
    | grep -vE '(^|/)(fixa|vendor|build|amalgama|scratchpad|archivum)/' \
    | grep -vE '(^|/)capsula_[a-z_]+\.(c|h)$')
[ -z "$FILES" ] && { echo "nulla plagula in $DIR" >&2; exit 2; }
N=$(printf '%s\n' "$FILES" | wc -l | tr -d ' ')
CAPITA=$(printf '%s\n' "$FILES" | grep '\.h$' | xargs -n1 dirname 2>/dev/null | sort -u | sed 's/^/-capita /' | tr '\n' ' ')

if [ "$MODUS" != "-scribere" ]; then
    ./silva/formator.sh $FILES $CAPITA -machina 2>"$T/census.err" > "$T/census.tsv"; rc=$?
    if [ "$rc" -eq 2 ] || ! grep -q '^#' "$T/census.tsv"; then
        echo "evolutio $DIR: APPARATUS FRACTUS (formator rc=$rc, nullus ordo titularis) - nihil censum" >&2
        tail -5 "$T/census.err" >&2; exit 2
    fi
    echo "evolutio $DIR: $N plagulae; exemptae $(grep -c 'exempta' "$T/census.err" || true)"
    awk -F'\t' '$1!~/^#/ && NF>=7 {n[$4]++; s++} END {for (r in n) printf "  %6d %s\n", n[r], r; printf "  summa %d\n", s+0}' "$T/census.tsv" | sort -rn
    exit 0
fi

./silva/formator.sh $FILES $CAPITA -scribere 2>"$T/scriptura.err" >"$T/scriptura.out"; rc=$?
if [ "$rc" -eq 2 ]; then
    echo "evolutio $DIR: APPARATUS FRACTUS aut recusatio (formator rc=2) - vide $T/scriptura.err" >&2
    tail -8 "$T/scriptura.err" >&2; exit 2
fi
echo "evolutio $DIR: scriptae $(grep -c 'formator: scriptum' "$T/scriptura.err" || true) / $N; recusatae $(grep -c 'recusat' "$T/scriptura.err" || true); exemptae $(grep -c 'exempta' "$T/scriptura.err" || true)"
grep 'recusat' "$T/scriptura.err" | head -5 >&2 || true

fracta=0; mutatae=0; cosm=0
for f in $(git diff --name-only -- $FILES 2>/dev/null); do
    mutatae=$((mutatae + 1))
    git show "HEAD:$f" > "$T/basis.c" 2>/dev/null || { echo "  $f: basis absens (nova?)"; continue; }
    d=$(./silva/differre.sh "$T/basis.c" "$f" 2>/dev/null)
    # tituli classium sunt '[substantiva]' - verbum nudum in corpore
    # diff apparet (silva_differre.c ipsa 'redde "substantiva"' fert)
    sub=$(printf '%s\n' "$d" | grep -c '\[substantiva\]' || true)
    doc=$(printf '%s\n' "$d" | grep -c '\[documentaria\]' || true)
    if [ "$sub" -ne 0 ] || [ "$doc" -ne 0 ]; then
        echo "  FRACTUM $f: substantiva $sub, documentaria $doc" >&2; fracta=1
    else
        cosm=$((cosm + 1))
    fi
done
echo "differre: $mutatae mutatae, $cosm cosmetica omnia"
if [ "$fracta" -ne 0 ]; then echo "evolutio $DIR: FRACTUM (differre non cosmetica)"; exit 1; fi
echo "evolutio $DIR: sanum - suites deinde, commissio per manum"
exit 0
