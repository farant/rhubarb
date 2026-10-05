#!/bin/bash
# crusta/effectus.sh - summarium effectuum scripti (STML, dialectus effectus)
# (effectus-spec par. IV; planum T3)
#
# Usage:  ./crusta/effectus.sh <scriptum>
#         ./crusta/effectus.sh -lintrum <plagula>...  (regulae effectus)
#         ./crusta/effectus.sh -catenae               (radices catenarum)
#         ./crusta/effectus.sh -census                (build/effectus/census.tsv)
# Exit:   0 sanum | 2 usus / scriptum absens / tabula illegibilis
#
# Effusio: summarium STML in stdout; viae radici relativae; scriptum
# relativum radici, non directorio operis. Situs et regulae:
# crusta/fontes/crusta_effectus.h; canon: effectus.canon.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$CRUSTA_DIR/build"
BIN="$BUILD_DIR/effectus"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$CRUSTA_DIR/fontes")
SRC="$CRUSTA_DIR/instrumenta/effectus.c"
# instrumentum se struit (exemplar crusta/facies.sh): clausura sua ab
# aedile, obiecta per bin/compilator, nexus solum si obiectum mutatum
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || {
    echo "effectus: aedificatio fracta" >&2; exit 2; }
if [ "${1:-}" = -census ]; then
    # census omnium scriptorum arboris (oracula/ excluditur: glutinum
    # dialecti alienae) -> build/effectus/census.tsv
    (cd "$RADIX_DIR" && git ls-files '*.sh' | grep -v '^oracula/') \
        | "$BIN" -radix "$RADIX_DIR" -census
    exit $?
fi
exec "$BIN" -radix "$RADIX_DIR" "$@"
