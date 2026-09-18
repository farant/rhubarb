#!/bin/bash

# fumus.sh - porta natalis unci pre-commit (examen): plantat REICE,
# exspectat obstare. Porta muta et porta mortua idem videntur - ergo
# culpa plantata ab ortu.
#
#   I   viae explicitae (UNCUS_VIAE): sanum -> 0, malum -> 1
#   II  via INDICIS (GIT_INDEX_FILE temporarius): malum additum ->
#       uncus obstat (1); index sine .c -> 0 cum 'nihil iudicatum'
#   IX-XI lint Latinus identificatorum (2026-09-07): verbum ignotum
#       in plagula nova tracta -> obstat cum exitibus; Latina sola -> 0;
#       pre-merge-commit item obstat
#   XV-XVI gradus II (regulae lintris) ad commissionem (2026-09-18):
#     '-nt' nudum OBSTAT, idem annotatus TRANSIT;
#   XII-XIV diagnostica materiae (.sh .css, 2026-09-18): plagula .sh
#       malformata OBSTAT; sana transit; FIXTURA PATHOLOGICA
#       (probationes/fixa/) NON obstat - per INDICEM probata, quia
#       UNCUS_VIAE consulto 'sine filtro' est
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
    git checkout HEAD -- "$FIX"   # HEAD, non index temporarius (qui appensum fert)
    # VII - commissio partialis (arbor != index): NON formata, monitum
    git read-tree HEAD
    printf '\nvacuum\nfumus_nova(vacuum)\n{\n    redde;\n}\n' >> "$FIX"
    git add -- "$FIX"
    printf '/* mutatio arboris non tradita */\n' >> "$FIX"
    "$UNCUS" > "$T/forma_part.out" 2>&1; rc=$?
    if [ "$rc" -eq 0 ] && grep -q 'NON formata' "$T/forma_part.out" && git show ":$FIX" | grep -q '^fumus_nova(vacuum)$'; then echo "  VII commissio partialis: intacta, monet  OK"; else echo "  VII FRACTUM (rc=$rc)"; cat "$T/forma_part.out"; fracta=1; fi
    git checkout HEAD -- "$FIX"   # HEAD, non index temporarius (qui appensum fert)
    # VIII - plagula nova: scriptura tota
    NOVA="tools/unci-git/.fumus_nova.c"
    printf '#include "latina.h"\n\ninterior vacuum\nf(vacuum)\n{\n    redde;\n}\n' > "$NOVA"
    git read-tree HEAD
    git add -f -- "$NOVA"
    "$UNCUS" > "$T/forma_nova.out" 2>&1; rc=$?
    if [ "$rc" -eq 0 ] && grep -q 'FORMATA' "$T/forma_nova.out" && git show ":$NOVA" | grep -q '^f (vacuum)$'; then echo "  VIII plagula nova: tota formata          OK"; else echo "  VIII FRACTUM (rc=$rc)"; cat "$T/forma_nova.out"; fracta=1; fi
    rm -f "$NOVA" "$T/index2"
    unset GIT_INDEX_FILE
    git checkout HEAD -- "$FIX"
else
    echo "  VI-VIII OMISSI: $FIX in arbore mutata"; fracta=1
fi

# IX-XI - LINT LATINUS (2026-09-07): plagula NOVA tracta in indice
# temporario cum verbo ignoto 'xyzzyquux' -> uncus obstat (1) cum
# 'IGNOTA NOVA' et exitibus; plagula Latina sola -> 0 'nihil novi';
# pre-merge-commit idem obstat. Per indicem VERUM symbolorum (nexus
# incrementalis: plagula in tools/unci-git/ ambulatur - nomen sine
# puncto initiali, quod percursus praeterit) et vias tractatas (git
# ls-files indicis temporarii).
LINTF="tools/unci-git/fumus_verba_ignota.c"
export GIT_INDEX_FILE="$T/index3"
rm -f "$GIT_INDEX_FILE"
git read-tree HEAD
printf '#include "latina.h"\n\ninterior integer\nxyzzyquux_fumus (vacuum)\n{\n    redde ZEPHYRUM;\n}\n' > "$LINTF"
git add -f -- "$LINTF"
"$UNCUS" > "$T/lint_malum.out" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q 'IGNOTA NOVA' "$T/lint_malum.out" && grep -q 'xyzzyquux' "$T/lint_malum.out" && grep -q 'EXITUS' "$T/lint_malum.out"; then echo "  IX  lint: verbum ignotum obstat + exitus  OK"; else echo "  IX  FRACTUM (rc=$rc)"; tail -20 "$T/lint_malum.out"; fracta=1; fi
tools/unci-git/pre-merge-commit > "$T/lint_fusio.out" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q 'IGNOTA NOVA' "$T/lint_fusio.out"; then echo "  X   lint ad fusionem: obstat            OK"; else echo "  X   FRACTUM (rc=$rc)"; tail -12 "$T/lint_fusio.out"; fracta=1; fi
printf '#include "latina.h"\n\ninterior integer\nfumus_latinus (vacuum)\n{\n    redde ZEPHYRUM;\n}\n' > "$LINTF"
git add -f -- "$LINTF"
"$UNCUS" > "$T/lint_sanum.out" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && grep -q 'nihil novi' "$T/lint_sanum.out"; then echo "  XI  lint: plagula Latina -> 0            OK"; else echo "  XI  FRACTUM (rc=$rc)"; tail -12 "$T/lint_sanum.out"; fracta=1; fi
rm -f "$LINTF" "$T/index3"
unset GIT_INDEX_FILE
# XII-XIV - DIAGNOSTICA MATERIAE (.sh .css, 2026-09-18)
printf '#!/bin/bash\necho ok\n' > "$T/sana.sh"
printf '#!/bin/bash\n{ echo a\n' > "$T/mala.sh"

UNCUS_VIAE="$T/sana.sh" "$UNCUS" > "$T/sana.out" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && grep -q 'plagulae .sh/.css sanae' "$T/sana.out"; then echo "  XII  .sh sana -> 0                       OK"; else echo "  XII  FRACTUM (rc=$rc)"; cat "$T/sana.out"; fracta=1; fi

UNCUS_VIAE="$T/mala.sh" "$UNCUS" > "$T/mala.out" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q 'DIAGNOSTICA MATERIAE' "$T/mala.out"; then echo "  XIII .sh mala -> 1 (OBSTAT)              OK"; else echo "  XIII FRACTUM (rc=$rc)"; cat "$T/mala.out"; fracta=1; fi

# XIV - FIXTURA PATHOLOGICA per INDICEM: probationes/fixa/ consulto mala
# est. Sine exclusione corpus fixturarum totum incommissibile fieret.
# Per indicem probatur, NON per UNCUS_VIAE: ille modus 'sine filtro' est.
# Plagula NOVA sub via excepta: fixtura EXSTANS ab HEAD non differt,
# ergo in 'diff --cached' NUMQUAM apparet et porta nihil probaret
# (mensuratum 2026-09-18: planta exclusionem removens VIRIDIS mansit).
FIXA_MALA="probationes/fixa/crusta/.fumus_mala.sh"
printf '#!/bin/bash\n{ echo a\n' > "$FIXA_MALA"
export GIT_INDEX_FILE="$T/index_fixa"
rm -f "$GIT_INDEX_FILE"
git read-tree HEAD
git add -f -- "$FIXA_MALA" 2>/dev/null
"$UNCUS" > "$T/fixa.out" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && ! grep -q 'DIAGNOSTICA MATERIAE' "$T/fixa.out"; then echo "  XIV  fixtura pathologica excepta -> 0    OK"; else echo "  XIV  FRACTUM (rc=$rc)"; cat "$T/fixa.out"; fracta=1; fi
unset GIT_INDEX_FILE
rm -f "$T/index_fixa" "$FIXA_MALA"

# XV-XVI - GRADUS II (regulae lintris) AD COMMISSIONEM, 2026-09-18.
# Ante hunc arcum regula lintris nusquam automatice currebat: catena
# eius per pythonica ibat et via commissionis Pythone carere debet.
# ASYMMETRIA EST PORTA: eadem plagula, annotatione sola differens -
# aliter porta 'obstat semper' a 'obstat recte' non distingueretur.
printf '#!/bin/bash\n[ $a -nt $b ]\n' > "$T/lint_mala.sh"
printf '#!/bin/bash\n# <tolera codex="lint:nt-aequalitas" (>consulto\n[ $a -nt $b ]\n' \
    > "$T/lint_sana.sh"

UNCUS_VIAE="$T/lint_mala.sh" "$UNCUS" > "$T/lint_mala.out" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q 'lint:nt-aequalitas' "$T/lint_mala.out"; then echo "  XV   .sh '-nt' nudum -> 1 (OBSTAT)       OK"; else echo "  XV   FRACTUM (rc=$rc)"; cat "$T/lint_mala.out"; fracta=1; fi

UNCUS_VIAE="$T/lint_sana.sh" "$UNCUS" > "$T/lint_sana.out" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && ! grep -q 'lint:nt-aequalitas' "$T/lint_sana.out"; then echo "  XVI  .sh '-nt' excusatum -> 0            OK"; else echo "  XVI  FRACTUM (rc=$rc)"; cat "$T/lint_sana.out"; fracta=1; fi

if [ "$fracta" -ne 0 ]; then echo "fumus unci: FRACTUM"; exit 1; fi
echo "fumus unci: sanum (XVI/XVI)"
exit 0
