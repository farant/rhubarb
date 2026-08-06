#!/bin/bash
# natura-custos.sh - PostToolUse (Write|Edit): iudicium bibliothecae
# semanticae in plagulam .genera modo scriptam - canalis PUSH
# (exemplar censor-custos.sh).
#
# Disciplina silentii: exitus purus = NIHIL in contextum;
# flagrationes solae additionalContext iniciunt (informatio, non
# obex - turnus pergit).
#
# MOMENTUM: bin/natura_examen exemplaria OMNIA onerat etiamsi unum
# iudicetur, quia regulae II/III/IV trans exemplaria transeunt -
# iudicium plagulae SOLIUS eas videre non potest. Filtrum post
# nexuram applicatur. Sumptus: XXIX millesima (natura_visus.sh
# XIV secunda - unde porta C, non shell).

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.genera) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */scratchpad/*|*/build/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PORTA="$RADIX/bin/natura_examen"

# porta absens = silentium, NON successus tacitus: si numquam
# structa est, dic id semel potius quam nihil agere fingens
if [ ! -x "$PORTA" ]; then
    jq -n '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:"NATURA: bin/natura_examen abest - porta .genera TACET. Strue: ./tools/natura_struere.sh"}}'
    exit 0
fi

OUT=$(cd "$RADIX" && "$PORTA" -plagula "$FILE" -machina 2>&1)
STATUS=$?

# exitus II = NIHIL ONERATUM (radix vacua/mota) - defectus portae
# ipsius, qui tacite ut sanitas legi NON debet
if [ "$STATUS" -eq 2 ]; then
    jq -n --arg o "$OUT" '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:("NATURA: porta NIHIL oneravit (exitus II) - exemplaria non inventa, iudicium NULLUM factum est:\n" + $o)}}'
    exit 0
fi

ROWS=$(printf '%s\n' "$OUT" | grep '^VULNUS' | head -10)
if [ -n "$ROWS" ]; then
    jq -n --arg r "$ROWS" '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:("NATURA (uncus post-editionem): contractus violatus in plagula modo scripta - campi: GRADUS\tREGULA\tEXEMPLAR\tENS\tNUNTIUS\n" + $r + "\nRegulae: natura/METAMODULUS.md par.8. Corpus totum: ./bin/natura_examen")}}'
    exit 0
fi

# VULNERA alibi (haec plagula sana, bibliotheca fracta) - dic,
# quia nexus fractus saepe ex hac editione natus est
if [ "$STATUS" -eq 1 ]; then
    ALIA=$(cd "$RADIX" && "$PORTA" -machina 2>/dev/null | grep -c '^VULNUS')
    jq -n --arg n "$ALIA" '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:("NATURA: haec plagula sana, sed bibliotheca " + $n + " VULNERA alibi fert (nexus trans-exemplaris fortasse ab hac editione fractus). Vide: ./bin/natura_examen")}}'
fi
exit 0
