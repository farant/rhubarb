#!/bin/bash
# canon-custos.sh - PostToolUse (Write|Edit): iudicium canonicum in
# plagulam .canon aut .stml modo scriptam - canalis PUSH (exemplar
# natura-custos.sh).
#
# .genera NON hic: natura-custos eas fert, et natura_examen post
# migrationem (2026-08-06) canonem IPSUM intus adhibet (regula
# VIII per lib/canon.c) - iudex unus, uncus unus, iudicium unum.
#
# Disciplina silentii: sanum = NIHIL in contextum; inventa et
# absentiae solae loquuntur. Dialectus sine linea registri notitiam
# unam lineam accipit (decretum Fran 2026-08-06): linea absens =
# dialectus nondum descriptus, NON dialectus sanus.

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.canon|*.stml) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */scratchpad/*|*/build/*|*/probationes/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PORTA="$RADIX/bin/canon_examen"

# porta absens = silentium non est successus tacitus
if [ ! -x "$PORTA" ]; then
    jq -n '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:"CANON: bin/canon_examen abest - porta .canon/.stml TACET. Strue: ./tools/canon_struere.sh"}}'
    exit 0
fi

# porta stala = iudicium vetus se recens fingens (lex -o /dev/null)
for f in lib/canon.c include/canon.h tools/canon_examen.c lib/stml.c; do
    if [ "$RADIX/$f" -nt "$PORTA" ]; then
        jq -n '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:"CANON: bin/canon_examen STALUS (fontes recentiores binario) - iudicium non currit. Strue: ./tools/canon_struere.sh"}}'
        exit 0
    fi
done

OUT=$(cd "$RADIX" && "$PORTA" "$FILE" 2>&1)
STATUS=$?

# exitus II = NIHIL iudicatum: aut dialectus sine canone (notitia)
# aut defectus portae (loquere totum)
if [ "$STATUS" -eq 2 ]; then
    if printf '%s' "$OUT" | grep -q "canonem in"; then
        jq -n --arg f "$FILE" '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:("CANON: dialectus nondum descriptus - " + $f + " canonem in canones.registrum non habet. Absentia non est sanitas; canon scribendus cum dialectus stabilis erit.")}}'
    else
        jq -n --arg o "$OUT" '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:("CANON: porta NIHIL iudicavit (exitus II) - defectus portae, non sanitas:\n" + $o)}}'
    fi
    exit 0
fi

if [ "$STATUS" -eq 1 ]; then
    ROWS=$(printf '%s\n' "$OUT" | head -12)
    jq -n --arg r "$ROWS" '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:("CANON (uncus post-editionem): contractus violatus in plagula modo scripta:\n" + $r + "\nCanones: canones.registrum; totum: bin/canon_examen <via>")}}'
fi
exit 0
