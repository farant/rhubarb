#!/bin/bash
# examen-custos.sh - PostToolUse (Write|Edit): iudicium C89 in
# plagulam .c modo scriptam - canalis PUSH (praecedens
# censor-custos; dum clangd quoque currit = differentialis VIVUS
# in sessionibus veris, calibratio ante substitutionem lsp).
#
# Disciplina silentii: nulla diagnostica (praeter infra) = NIHIL
# in contextum. Exclusiones lexici (exclusiones.txt pinnatae)
# TACENT - strepitus deorsum-lexici doceret negligere. Capita .h
# non iudicantur sola (per includentes iudicantur); scratchpad/
# build/fixa exclusa (probae consulto pravae).

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.c) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */scratchpad/*|*/build/*|*/fixa/*|*/amalgama/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

case "$FILE" in
    "$RADIX"/*) REL="./${FILE#"$RADIX"/}" ;;
    *) exit 0 ;;   # extra repositorium
esac
EXCL="$RADIX/silva/probationes/fixa/examinis/exclusiones.txt"
[ -f "$EXCL" ] && grep -qxF "$REL" "$EXCL" && exit 0

OUT=$(cd "$RADIX" && ./silva/examen.sh "$REL" -machina 2>/dev/null)
ROWS=$(printf '%s\n' "$OUT" | awk -F'\t' \
    '$1!="VERDICTUM" && $4!="infra" {
        p = ($6=="1") ? " (provisionale)" : "";
        print $1":"$2":"$3": ["$4"] "$7 p
    }' | head -10)
[ -z "$ROWS" ] && exit 0
VERD=$(printf '%s\n' "$OUT" | awk -F'\t' '$1=="VERDICTUM"{print $2}')

jq -n --arg r "EXAMEN C89 (uncus post-editionem, verdictum ${VERD:-?}):
$ROWS
violatio = constrictio C89 (clang -pedantic-errors reiciet); suspectum = licitum sed suspectum (vocatio implicita etc.). Effusor: ./silva/examen.sh <plagula>. Detail: silva/phase-log.md M4a." \
    '{additionalContext:$r}'
exit 0
