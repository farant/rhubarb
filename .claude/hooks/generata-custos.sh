#!/bin/bash
# generata-custos.sh - PreToolUse (Write|Edit): vetat editiones
# manuales plagularum GENERATARUM.
#
# MECHANICUM, non lista viarum: plagula generata se ipsa signat
# (linea prima "GENERATUM" + "NE MANU EDITES" / "NOLI MANU
# MUTARE") - plagulae generatae futurae se sponte protegunt.
# Generatores per Bash scribunt, non per Write/Edit - regeneratio
# ergo libera manet.

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')
[ -z "$FILE" ] && exit 0
[ -f "$FILE" ] || exit 0

if head -3 "$FILE" | grep -q "GENERATUM" \
   && head -3 "$FILE" | grep -qE "NE MANU EDITES|NOLI MANU MUTARE"; then
    jq -n --arg r "Plagula GENERATA: $FILE (signum 'NE MANU EDITES' in capite). Noli manu mutare - muta FONTEM et regenera (silva/generare.sh pro tabulis, */amalgamare.sh pro amalgamis). Si editio manualis vere necessaria est, id cum Frane confirma." \
        '{hookSpecificOutput:{hookEventName:"PreToolUse",permissionDecision:"deny",permissionDecisionReason:$r}}'
    exit 0
fi
exit 0
