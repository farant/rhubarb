#!/bin/bash
# excubitor-custos.sh - PostToolUse (Write|Edit): monitum LEVE cum
# caput COMMUNE mutatur (>= II includentes directi in grapho) -
# obiecta consumptorum trans piscinas stala fiunt. Non-praeventivum:
# scripta sana proximo cursu recompilant; hoc solum conscientiam
# radii explosionis dat (classis ABI, inventum v0.2).
#
# Disciplina silentii: caput privatum (0-1 includentes) = NIHIL;
# graphus deest = NIHIL (numquam impedit).

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.h) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */scratchpad/*|*/build/*|*/fixa/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
case "$FILE" in
    "$RADIX"/*) REL="${FILE#"$RADIX"/}" ;;
    *) exit 0 ;;
esac

GRAPHUS="$RADIX/build/inclusiones.tsv"
[ -f "$GRAPHUS" ] || exit 0

# -F cum tab litterali: grep BSD systematis -P nescit (uncus PATH
# systematis currit, non profilum usoris)
TAB=$'\t'
N=$(grep -F "${TAB}${REL}${TAB}" "$GRAPHUS" 2>/dev/null | cut -f1 | sort -u | wc -l | tr -d ' ')
[ "$N" -lt 2 ] && exit 0

jq -n --arg r "EXCUBITOR (uncus post-editionem): $REL a $N plagulis directe includitur - obiecta consumptorum trans piscinas nunc stala fiunt (scripta sana proximo cursu recompilant). Quis pendet: awk -F'\t' -v h='$REL' '\$2==h{print \$1}' build/inclusiones.tsv. Verdictum: ./excubitor.sh" \
    '{additionalContext:$r}'
exit 0
