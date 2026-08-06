#!/bin/bash
# censor-custos.sh - PostToolUse (Write|Edit): censor latinae in
# plagulam .c/.h modo scriptam - canalis PUSH (exemplar clangd).
#
# Disciplina silentii: exitus purus = NIHIL in contextum (FP
# censoris = 0.00 mensuratum, corpus 762 plagularum); flagrationes
# solae additionalContext iniciunt (informatio, non obex - turnus
# pergit, ut diagnostica clangd).
#
# Exclusa: corpus regressionis censoris (flagrationes consulto
# plantatae), scratchpad (probae consulto pravae), build/.

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.c|*.h) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */censoris/*|*/scratchpad/*|*/build/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
OUT=$("$RADIX/silva/censor.sh" "$FILE" 2>/dev/null)
if [ $? -eq 1 ]; then
    ROWS=$(printf '%s\n' "$OUT" | grep '\[CENSURA\]' | head -10)
    jq -n --arg r "CENSOR LATINAE (uncus post-editionem): macrum latinae in positione NOMINIS in plagula modo scripta - identificator alius quam scriptus erit:
$ROWS
Muta nomen (verba in latina.h #definita reservata sunt). Detail: silva/instrumenta/principalia/censor.worklog.md" \
        '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:$r}}'
fi
exit 0
