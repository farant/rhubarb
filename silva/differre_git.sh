#!/bin/bash

# silva/differre_git.sh - differre contra historiam git
#
# Usage:
#   ./silva/differre_git.sh <via> [ref_vetus] [ref_novum] [-machina]
#   ref_vetus ordinarius: HEAD
#   ref_novum absens = ARBOR LABORANS (plagula in disco)
# Exit: differre ipsius (0 cucurrit | 2 usus/illegibilis)
#
# Exempla:
#   ./silva/differre_git.sh lib/silex.c                  # HEAD -> discus
#   ./silva/differre_git.sh lib/silex.c HEAD~3           # HEAD~3 -> discus
#   ./silva/differre_git.sh lib/silex.c abc123 def456    # inter refs
#
# FRONS TEMPORARIUS: hic solum materializatio (git show in
# silva/build/); cum bibliotheca lectionis git nascatur, viscera
# haec substituit facie immota. Plagula quae ad ref non exsistit =
# latus vacuum (omnia ADDITA/REMOTA - honestum).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
cd "$RADIX_DIR"

VIA=""
REF_A=""
REF_B=""
MACHINA=""
for arg in "$@"; do
    case "$arg" in
        -machina) MACHINA="-machina" ;;
        *)
            if [ -z "$VIA" ]; then VIA="$arg"
            elif [ -z "$REF_A" ]; then REF_A="$arg"
            elif [ -z "$REF_B" ]; then REF_B="$arg"
            fi
            ;;
    esac
done
if [ -z "$VIA" ]; then
    echo "usus: differre_git.sh <via> [ref_vetus] [ref_novum] [-machina]" >&2
    exit 2
fi
REF_A="${REF_A:-HEAD}"

TMP="$SILVA_DIR/build/differre_git"
mkdir -p "$TMP"

VETUS="$TMP/vetus"
git show "$REF_A:$VIA" > "$VETUS" 2>/dev/null || : > "$VETUS"

if [ -n "$REF_B" ]; then
    NOVUM="$TMP/novum"
    git show "$REF_B:$VIA" > "$NOVUM" 2>/dev/null || : > "$NOVUM"
else
    NOVUM="$VIA"
    if [ ! -f "$NOVUM" ]; then
        : > "$TMP/novum"
        NOVUM="$TMP/novum"
    fi
fi

exec "$SILVA_DIR/differre.sh" "$VETUS" "$NOVUM" $MACHINA
