#!/bin/bash
# silva/scribe_fumus.sh - porta natalis scribe.sh
#
# Culpa plantata: commentum '//' (codex 89, C99-ismus) -> REICE exitu I;
# plagula munda -> ACCIPE exitu 0; .md -> scriptum, non iudicatum
# exitu IV; parens absens -> exitu II sine plagula. Effusus:
# 'fumus scribe: sanum' | 'fumus scribe: FRACTUM'. Exitus 0/1.
set -u
SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
D="$RADIX_DIR/silva/build/scribe_fumus"
rm -rf "$D"; mkdir -p "$D"
fracta=0
culpa () { echo "  FRACTUM: $1"; fracta=$((fracta + 1)); }
ok () { echo "  ok   $1"; }

cd "$D" || exit 1

# I. culpa plantata: '//' -> REICE (1)
out=$("$SILVA_DIR/scribe.sh" culpa.c <<'CEOF'
#include "latina.h"
// commentum C99 - examen codex LXXXIX
integer principale(vacuum) { redde ZEPHYRUM; }
CEOF
); rc=$?
if [ "$rc" -eq 1 ] && echo "$out" | grep -q 'REICE'; then ok "'//' plantatum -> REICE exitu I"; else culpa "'//' plantatum: exitus $rc, effusus: $(echo "$out" | tail -3 | tr '\n' ' ')"; fi
[ -f culpa.c ] && ok "plagula refutata scripta manet" || culpa "plagula refutata absens"

# II. munda -> ACCIPE (0)
out=$("$SILVA_DIR/scribe.sh" munda.c <<'CEOF'
/* munda.c - fumus scribe */
#include "latina.h"

integer principale(vacuum);

integer
principale (vacuum)
{
    redde ZEPHYRUM;
}
CEOF
); rc=$?
if [ "$rc" -eq 0 ] && echo "$out" | grep -q 'ACCIPE'; then ok "munda -> ACCIPE exitu 0"; else culpa "munda: exitus $rc, effusus: $(echo "$out" | tail -3 | tr '\n' ' ')"; fi
echo "$out" | grep -q 'scriptum: munda.c' && ok "scriptura nominata cum octetis" || culpa "linea 'scriptum' absens"

# III. .md -> scriptum, non iudicatum (4)
out=$(printf '# titulus\n' | "$SILVA_DIR/scribe.sh" nota.md); rc=$?
if [ "$rc" -eq 4 ] && echo "$out" | grep -q 'non iudicatum' && [ "$(cat nota.md)" = "# titulus" ]; then ok ".md -> scriptum, non iudicatum, exitu IV"; else culpa ".md: exitus $rc"; fi

# IV. parens absens -> 2, nihil scriptum
out=$(printf 'x' | "$SILVA_DIR/scribe.sh" absens/x.c 2>&1); rc=$?
if [ "$rc" -eq 2 ] && [ ! -e absens/x.c ]; then ok "parens absens -> exitu II, nihil scriptum"; else culpa "parens absens: exitus $rc"; fi

# V. via absoluta extra repositorium -> 4
out=$(printf '/* x */\n' | "$SILVA_DIR/scribe.sh" /tmp/scribe_fumus_extra.c); rc=$?
if [ "$rc" -eq 4 ] && echo "$out" | grep -q 'extra repositorium'; then ok "extra repositorium -> non iudicatum, exitu IV"; else culpa "extra repositorium: exitus $rc"; fi
rm -f /tmp/scribe_fumus_extra.c

if [ "$fracta" -eq 0 ]; then echo "fumus scribe: sanum"; exit 0; fi
echo "fumus scribe: FRACTUM ($fracta)"; exit 1
