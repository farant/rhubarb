#!/bin/bash
# <cursus titulus="silva/lexema_mensura.sh" versio="I"/>
# CHASSIS CURSUUM (analytica T6): cursus in annalibus
source "$(dirname "${BASH_SOURCE[0]}")/../tools/cursus.sh"
# silva/lexema_mensura.sh - MENSURA LEXATORIS silvae (octeti, lexemata,
# MB/s crudi et attachati; tempus processoris, optimum ex -n). Non porta:
# instrumentum mensurae (ut run_benchmark.sh flaturae).
#
# Usus: ./silva/lexema_mensura.sh [-n N] [-machina] [PLAGULA...]
#   ./silva/lexema_mensura.sh lib/*.c    # corpus totum, linea SUMMA
set -u
SILVA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"
BIN="$BUILD_DIR/lexema_mensura"
SRC="$SILVA_DIR/instrumenta/principalia/lexema_mensura.c"
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 2
cd "$RADIX_DIR" || exit 2
"$BIN" "$@"
exit $?
