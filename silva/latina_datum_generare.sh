#!/bin/bash
# silva/latina_datum_generare.sh - silva/fontes/silva_latina_datum.{h,c}
# ex include/latina.h (silva/instrumenta/principalia/latina_datum.c)
#
# Actio fabricae 'latina_datum' (1b T5, D5): olim amalgamator silvae has
# plagulas commissas in omni cursu scribebat. FABRICA_SCRIPTURA
# (iudex): exitus in directorium scripturae, viae eaedem ac commissae.
#
# Usus: ./silva/latina_datum_generare.sh
# Exitus: 0 scriptum · 1 fractum · 2 aedificatio.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$RADIX/tools/vexilla.sh"
mkdir -p "$RADIX/build"
BIN="$RADIX/build/latina_datum"
clang "${VEXILLA_C89[@]}" -I"$RADIX/include" \
    "$RADIX/silva/instrumenta/principalia/latina_datum.c" -o "$BIN" || exit 2
SCRIPTURA="$RADIX"
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    SCRIPTURA="$FABRICA_SCRIPTURA"
    mkdir -p "$SCRIPTURA/silva/fontes" || exit 1
fi
exec "$BIN" "$RADIX" "$SCRIPTURA"
