#!/bin/bash

# silva/differre_git.sh - differre contra historiam git (NATIVUM)
#
# Usage:
#   ./silva/differre_git.sh <via> [ref_vetus] [ref_novum] [-machina]
#   ref_vetus ordinarius: HEAD; ref_novum absens = arbor laborans
#
# Facies stabilis, viscera mutata 2026-08-17: olim materializatio
# per 'git show' subprocessum, nunc modus -git differre ipsius
# (bibliotheca lib/git - lectio nativa, omnis obiectum
# sha-verificatum). Alias tenuis.

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$SILVA_DIR/differre.sh" -git "$@"
