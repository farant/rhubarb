#!/bin/bash
# fabrica-celer.sh - SessionStart: bin/fabrica iudicare (celer, ~I,IV s)
# nominat quae artificia ex fontibus hodiernis NON facta sunt - binaria
# installata stala ("rebake post lib/"), generata commissa stala - cum
# ordine sanationis (fabrica plan 1a T8, gradus II).
#
# Disciplina silentii: omnia recentia = NIHIL. Lineae semper praesentes
# (ORPHANUM, 'IGNOTUM: N binaria in bin/ sine declaratione') non
# referuntur - aliter uncus numquam taceret. bin/fabrica deest =
# monitum unum (iudex ipse deest, id sciendum est).

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$RADIX" || exit 0

if [ ! -x bin/fabrica ]; then
    jq -n --arg r "FABRICA (uncus sessionis): bin/fabrica deest - iudex aedificationis non currit. Strue: ./tools/fabrica_struere.sh" \
        '{hookSpecificOutput:{hookEventName:"SessionStart",additionalContext:$r}}'
    exit 0
fi

EFFUSIO="$(bin/fabrica iudicare 2>&1)"
# verdicta non-recentia (non 'IGNOTUM:' numeri Q36) + ordo sanationis
LINEAE="$(printf '%s\n' "$EFFUSIO" \
    | grep -E '^(STALUM|IGNOTUM) [^:]|^SANATIO:|^  ')"
[ -z "$LINEAE" ] && exit 0

jq -n --arg r "FABRICA (uncus sessionis, bin/fabrica iudicare celer): artificia NON ex fontibus hodiernis facta - sana ante laborem aut dic Frano:
$LINEAE" \
    '{hookSpecificOutput:{hookEventName:"SessionStart",additionalContext:$r}}'
exit 0
