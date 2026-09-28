#!/bin/bash
# tools/latina_numeri.sh - sectionem numerorum latina.h generare aut
# comparare (tools/latina_numeri.c; fons: numerus_romanus_scribere)
#
# Usus: ./tools/latina_numeri.sh [-scribere]
# Exitus: 0 recens/scriptum · 1 stalum · 2 usus/aedificatio.
# Porta: tools/generata_probare.sh (V).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$RADIX/tools/vexilla.sh"
mkdir -p "$RADIX/build"
BIN="$RADIX/build/latina_numeri"
clang "${VEXILLA_C89[@]}" -I"$RADIX/include" \
    "$RADIX/tools/latina_numeri.c" \
    "$RADIX/lib/numerus_romanus.c" "$RADIX/lib/chorda.c" \
    "$RADIX/lib/chorda_aedificator.c" "$RADIX/lib/piscina.c" \
    "$RADIX/lib/filum.c" \
    -o "$BIN" || exit 2
exec "$BIN" "$RADIX/include/latina.h" "$@"
