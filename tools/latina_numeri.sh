#!/bin/bash
# tools/latina_numeri.sh - sectionem numerorum latina.h generare aut
# comparare (tools/latina_numeri.c; fons: numerus_romanus_scribere)
#
# Usus: ./tools/latina_numeri.sh [-scribere]
# Exitus: 0 recens/scriptum · 1 stalum · 2 usus/aedificatio.
# Porta: generata (tools/generata_iudicare.sh, actio numeri_latinae).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$RADIX/tools/vexilla.sh"
mkdir -p "$RADIX/build"
BIN="$RADIX/build/latina_numeri"
clang "${VEXILLA_C89[@]}" -I"$RADIX/include" \
    "$RADIX/tools/latina_numeri.c" \
    "$RADIX/lib/numerus_romanus.c" "$RADIX/lib/chorda.c" \
    "$RADIX/lib/chorda_aedificator.c" "$RADIX/lib/piscina.c" \
    "$RADIX/lib/filum.c" "$RADIX/lib/lectiones.c" \
    -o "$BIN" || exit 2
# FABRICA_SCRIPTURA (bin/fabrica -plenus, plan 1a T5): latina.h in
# scripturam copiatur, sectio numerorum ibi regeneratur (-scribere);
# artificium = plagula tota, pars manu scripta octetim copiata
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    mkdir -p "$FABRICA_SCRIPTURA/include" || exit 1
    cp "$RADIX/include/latina.h" "$FABRICA_SCRIPTURA/include/latina.h" \
        || exit 1
    exec "$BIN" "$FABRICA_SCRIPTURA/include/latina.h" -scribere
fi
exec "$BIN" "$RADIX/include/latina.h" "$@"
