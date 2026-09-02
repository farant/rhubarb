#!/bin/bash
# formator-custos.sh - PostToolUse (Write|Edit): lint formae domus
# in plagulam .c/.h modo scriptam - RELATIO SOLA (numquam scribit;
# emendatio manu: ./silva/formator.sh <via> -scribere).
#
# Disciplina silentii: regulae FIXABILES solae referuntur -
# longitudo/vexilla/ordo (gradus lint-perpetuus, a -scribere non
# sanabiles) TACENT, aliter residuum censuum quamque editionem
# strepitaret et negligere doceret. Exclusiones pinnatae ab ipso
# CLI tacentur (exempta -> stderr, ordines nulli).

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.c|*.h) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */scratchpad/*|*/build/*|*/fixa/*|*/amalgama/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

case "$FILE" in
    "$RADIX"/*) REL="${FILE#"$RADIX"/}" ;;
    *) exit 0 ;;   # extra repositorium
esac

OUT=$(cd "$RADIX" && ./silva/formator.sh "$REL" -machina 2>/dev/null)

# regulae fixabiles solae (gradus lint-perpetuus omissus; longitudo
# FRANGIBILIS fixabilis est - nuntius eam signat, 2026-09-01)
ROWS=$(printf '%s\n' "$OUT" | awk -F'\t' \
    '$1!~/^#/ && NF>=7 \
     && ($4!="longitudo-lxxii" || $7 ~ /frangibilis/) \
     && $4!="vexillum-quinquaginta" \
     && $4!="ordo-inclusionum" {
        print $2":"$3" ["$4"] "$7" ("$5" pro "$6")"
    }')
[ -z "$ROWS" ] && exit 0

N=$(printf '%s\n' "$ROWS" | wc -l | tr -d ' ')
CAPUT=$(printf '%s\n' "$ROWS" | head -6)

jq -n --arg r "FORMATOR (uncus post-editionem, relatio sola): $N divergentiae fixabiles in $REL:
$CAPUT
Emendatio: ./silva/formator.sh $REL -scribere (regulae: project-specs/formator-regulae.md)" \
    '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:$r}}'
exit 0
