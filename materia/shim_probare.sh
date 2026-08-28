#!/bin/bash
# shim_probare.sh - Porta shim C89: materia_scribere contra silva_scribere
#
# Usage:
#   ./materia/shim_probare.sh                  # corpus ordinarium
#   ./materia/shim_probare.sh lib/piscina.c    # plagulae nominatae
#
# Contractus exitus: 0 omnia idem · 1 divergentiae · 2 NIHIL CURSUM.
#
# Silva OBIECTA AEDIFICATA poscit (./silva/compile_probationes.sh).
# Absentia CLAMAT - porta quae tacite nihil agit porta mortua est.

set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="$RADIX/materia/build"
mkdir -p "$BUILD"

OBIECTA=$(ls "$RADIX"/silva/build/*.o 2>/dev/null \
          | grep -v -E "/(fons_|nexus_|silva_amalgama\.o|apparatus\.o)")
if [ -z "$OBIECTA" ]; then
    echo "DEEST: silva/build/*.o - curre ./silva/compile_probationes.sh primum" >&2
    exit 2
fi

for m in materia_token materia_nodus materia_scribere; do
    src="$RADIX/materia/fontes/$m.c"
    obj="$BUILD/$m.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
        clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion \
              -Wsign-conversion -Wcast-qual -Wstrict-prototypes \
              -Wmissing-prototypes -Wwrite-strings -Wno-long-long \
              -I"$RADIX/include" -I"$RADIX/materia/fontes" \
              -c "$src" -o "$obj" || { echo "FRACTA: $m.c" >&2; exit 1; }
    fi
done

BIN="$BUILD/shim_c89"
clang -std=c89 -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 \
  -I"$RADIX/include" -I"$RADIX/silva/fontes" -I"$RADIX/silva/instrumenta" \
  -I"$RADIX/materia/fontes" \
  "$RADIX/materia/instrumenta/shim_c89.c" \
  "$BUILD/materia_token.o" "$BUILD/materia_nodus.o" "$BUILD/materia_scribere.o" \
  $OBIECTA -o "$BIN" || { echo "FRACTA: nexus shim" >&2; exit 1; }

if [ $# -gt 0 ]; then
    PLAGULAE=("$@")
else
    PLAGULAE=($(ls "$RADIX"/lib/*.c "$RADIX"/include/*.h "$RADIX"/silva/fontes/*.c 2>/dev/null))
fi
if [ "${#PLAGULAE[@]}" -eq 0 ]; then
    echo "NULLA PLAGULA - exitus II" >&2
    exit 2
fi

"$BIN" "${PLAGULAE[@]}"
