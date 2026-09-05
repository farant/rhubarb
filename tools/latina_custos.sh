#!/bin/bash
# tools/latina_custos.sh - identificatores DECLARATI contra macros latina.h
# Usus: ./tools/latina_custos.sh <plagula.c|.h> ...   (exitus 1 si collisio)
# Macro latina.h (nomen = typedef, duplex = double, casus = case ...) ut
# nomen variabilis/campi/parametri = error compilatoris obscurus. Hic
# clare nominatur ANTE aedificationem. Keywords (si, redde, per...) ut
# keywords licent - solae DECLARATIONES iudicantur.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
macros=$(grep -oE '^#define[[:space:]]+[a-z_]+' "$RADIX/include/latina.h" | awk '{print $2}' | sort -u)
typi='b32|i32|s32|s64|f32|f64|i8|s8|u8|u32|chorda|Punctum|Fines|Eventus|character|integer|longus|brevis|memoriae_index|[A-Z][A-Za-z0-9_]*'
exitus=0
for f in "$@"; do
    for m in $macros; do
        # declaratio: <typus> [*] m [;,)=[]   - non in commentariis
        if grep -nE "(^|[^A-Za-z_])(${typi})[[:space:]]*\*?[[:space:]]+${m}[[:space:]]*[;,)=[]" "$f" \
           | grep -vE '^[0-9]+:[[:space:]]*(/\*|\*)' | grep -q .; then
            echo "$f: identificator declaratus '$m' est macro latina.h" >&2
            grep -nE "(^|[^A-Za-z_])(${typi})[[:space:]]*\*?[[:space:]]+${m}[[:space:]]*[;,)=[]" "$f" | head -2 | sed 's/^/    /' >&2
            exitus=1
        fi
    done
done
exit $exitus
