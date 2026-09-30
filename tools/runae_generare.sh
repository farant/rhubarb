#!/bin/bash

# tools/runae_generare.sh - tabulae runarum GENERATAE (runae)
#
# lib/runae_tabulae.c ex datis Unicode fixis
# (probationes/fixa/unicode/<versio>/) per tools/runae_generare.c.
# Versio ex RUNAE_VERSIO in include/runae.h (una sedes).
#
# Usus: ./tools/runae_generare.sh            # scribit
#       ./tools/runae_generare.sh -probare   # tabula commissa recens? 0/1
#
# -probare EXACTA (cmp): plagula commissa = effusio generatoris
# octetim (GENERATUM in linea prima - uncus pre-commit eam non format;
# fabrica P2 via B, 2026-09-29). Olim spatia neglegebat.

set -u
export LC_ALL=C
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"

EXITUS="lib/runae_tabulae.c"
PROBARE=0
for arg in "$@"; do
    case "$arg" in
        -probare) PROBARE=1 ;;
        *) echo "usus: runae_generare.sh [-probare]" >&2
           exit 2 ;;
    esac
done

VERSIO="$(sed -n 's/^#define RUNAE_VERSIO "\(.*\)"$/\1/p' include/runae.h)"
if [ -z "$VERSIO" ]; then
    echo "runae_generare: RUNAE_VERSIO in include/runae.h non inventa" >&2
    exit 2
fi
UCD="probationes/fixa/unicode/$VERSIO"

source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
mkdir -p build
BIN="build/runae_generare"
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "tools/runae_generare.c" ]; then
    clang "${GCC_FLAGS[@]}" -Iinclude tools/runae_generare.c -o "$BIN" || exit 2
fi

if [ "$PROBARE" = 1 ]; then
    TMP="$(mktemp)"
    trap 'rm -f "$TMP"' EXIT
    "$BIN" "$UCD" "$TMP" 2>/dev/null || exit 2
    if cmp -s "$TMP" "$EXITUS"; then
        echo "runae_generare: $EXITUS recens (Unicode $VERSIO)"
        exit 0
    fi
    echo "runae_generare: $EXITUS RANCIDA (regenera: ./tools/runae_generare.sh)"
    exit 1
fi
"$BIN" "$UCD" "$EXITUS"
