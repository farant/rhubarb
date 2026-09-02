#!/bin/bash

# fumus.sh - porta natalis unci pre-commit (examen): plantat REICE,
# exspectat obstare. Porta muta et porta mortua idem videntur - ergo
# culpa plantata ab ortu.
#
#   I   viae explicitae (UNCUS_VIAE): sanum -> 0, malum -> 1
#   II  via INDICIS (GIT_INDEX_FILE temporarius): malum additum ->
#       uncus obstat (1); index sine .c -> 0 cum 'nihil iudicatum'
#   VI-VIII scriptura automatica formae: functio appensa formatur et
#       index reponitur; commissio partialis intacta + monitum; plagula
#       nova tota formata (fixum tractum fumus_formae.c, checkout)
# Planta = declaratio in 'per' (C99): examen 'nodi erroris' REICE
# (mensuratum 2026-09-01; '//' et declaratio post sententiam ACCIPE -
# lacunae examinis, non plantae).
#
# Usage: ./tools/unci-git/fumus.sh     exit 0 sanum | 1 fractum

set -u
RADIX="$(git rev-parse --show-toplevel)" || exit 2
cd "$RADIX" || exit 2
UNCUS=tools/unci-git/pre-commit
T=build/uncus_fumus
mkdir -p "$T"
fracta=0

printf '#include "latina.h"\n\ninteger\nprincipale (vacuum)\n{\n    redde ZEPHYRUM;\n}\n' > "$T/sanum.c"
printf '#include "latina.h"\n\ninteger\nprincipale (vacuum)\n{\n    per (integer i = ZEPHYRUM; i < I; i++) { }\n    redde ZEPHYRUM;\n}\n' > "$T/malum.c"

# I - viae explicitae
UNCUS_VIAE="$T/sanum.c" "$UNCUS" > "$T/sanum.out" 2>&1; rc=$?
if [ "$rc" -eq 0 ]; then echo "  I.a sanum -> 0            OK"; else echo "  I.a sanum -> $rc  FRACTUM"; fracta=1; fi
UNCUS_VIAE="$T/malum.c" "$UNCUS" > "$T/malum.out" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q 'REICE' "$T/malum.out"; then echo "  I.b malum -> 1 (REICE)    OK"; else echo "  I.b malum -> $rc  FRACTUM"; cat "$T/malum.out"; fracta=1; fi

# II - via indicis temporarii (index verus intactus)
MALUM="tools/unci-git/.fumus_malum.c"
cp "$T/malum.c" "$MALUM"
export GIT_INDEX_FILE="$T/index"
rm -f "$GIT_INDEX_FILE"
git read-tree HEAD
"$UNCUS" > "$T/index_vacuus.out" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && grep -q 'nihil iudicatum' "$T/index_vacuus.out"; then echo "  II.a index sine .c -> 0    OK"; else echo "  II.a index sine .c -> $rc  FRACTUM"; cat "$T/index_vacuus.out"; fracta=1; fi
git add -f "$MALUM"
"$UNCUS" > "$T/index_malum.out" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q 'OBSTATA' "$T/index_malum.out"; then echo "  II.b index cum malo -> 1   OK"; else echo "  II.b index cum malo -> $rc  FRACTUM"; cat "$T/index_malum.out"; fracta=1; fi
unset GIT_INDEX_FILE
rm -f "$MALUM" "$T/index"

# V - formator -delta MONET (exitus 0 manet): plagula C89 sana cum
# divergentia formae nova (linea longa) -> monitum 'formator', non obstat
printf '#include "latina.h"\n\n/* commentarium valde longum quod limitem septuaginta duorum octetorum excedit */\ninteger\nprincipale (vacuum)\n{\n    redde ZEPHYRUM;\n}\n' > "$T/forma.c"
UNCUS_VIAE="$T/forma.c" "$UNCUS" > "$T/forma.out" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && grep -q 'formator (pre-commit)' "$T/forma.out" && grep -q 'longitudo-lxxii' "$T/forma.out"; then echo "  V   formator delta monet, non obstat OK"; else echo "  V   FRACTUM (rc=$rc)"; cat "$T/forma.out"; fracta=1; fi

# VI-VIII - SCRIPTURA AUTOMATICA formae (index temporarius, plagula
# fixa tracta tools/unci-git/fumus_formae.c; checkout restituit)
FIX="tools/unci-git/fumus_formae.c"
if git diff --quiet -- "$FIX" && git diff --cached --quiet -- "$FIX"; then
    export GIT_INDEX_FILE="$T/index2"
    rm -f "$GIT_INDEX_FILE"
    git read-tree HEAD
    # VI - functio male formata appensa, index = arbor -> formata + reposita
    printf '\nvacuum\nfumus_nova(vacuum)\n{\n    redde;\n}\n' >> "$FIX"
    git add -- "$FIX"
    "$UNCUS" > "$T/forma_auto.out" 2>&1; rc=$?
    if [ "$rc" -eq 0 ] && grep -q 'FORMATA' "$T/forma_auto.out" && grep -q '^fumus_nova (vacuum)$' "$FIX" \
       && git show ":$FIX" | grep -q '^fumus_nova (vacuum)$'; then echo "  VI  scriptura automatica: formata + index OK"; else echo "  VI  FRACTUM (rc=$rc)"; cat "$T/forma_auto.out"; fracta=1; fi
    git checkout -- "$FIX"
    # VII - commissio partialis (arbor != index): NON formata, monitum
    git read-tree HEAD
    printf '\nvacuum\nfumus_nova(vacuum)\n{\n    redde;\n}\n' >> "$FIX"
    git add -- "$FIX"
    printf '/* mutatio arboris non tradita */\n' >> "$FIX"
    "$UNCUS" > "$T/forma_part.out" 2>&1; rc=$?
    if [ "$rc" -eq 0 ] && grep -q 'NON formata' "$T/forma_part.out" && git show ":$FIX" | grep -q '^fumus_nova(vacuum)$'; then echo "  VII commissio partialis: intacta, monet  OK"; else echo "  VII FRACTUM (rc=$rc)"; cat "$T/forma_part.out"; fracta=1; fi
    git checkout -- "$FIX"
    # VIII - plagula nova: scriptura tota
    NOVA="tools/unci-git/.fumus_nova.c"
    printf '#include "latina.h"\n\ninterior vacuum\nf(vacuum)\n{\n    redde;\n}\n' > "$NOVA"
    git read-tree HEAD
    git add -f -- "$NOVA"
    "$UNCUS" > "$T/forma_nova.out" 2>&1; rc=$?
    if [ "$rc" -eq 0 ] && grep -q 'FORMATA' "$T/forma_nova.out" && git show ":$NOVA" | grep -q '^f (vacuum)$'; then echo "  VIII plagula nova: tota formata          OK"; else echo "  VIII FRACTUM (rc=$rc)"; cat "$T/forma_nova.out"; fracta=1; fi
    rm -f "$NOVA" "$T/index2"
    unset GIT_INDEX_FILE
else
    echo "  VI-VIII OMISSI: $FIX in arbore mutata"; fracta=1
fi

if [ "$fracta" -ne 0 ]; then echo "fumus unci: FRACTUM"; exit 1; fi
echo "fumus unci: sanum (VIII/VIII)"
exit 0
