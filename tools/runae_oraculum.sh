#!/bin/bash

# tools/runae_oraculum.sh - oraculum ICU4C pro tabulis runarum (U3)
#
# Struit tools/runae_oraculum.c (cum lib/runae.c + lib/runae_tabulae.c)
# et currit. ICU4C per dlopen aperitur (Homebrew icu4c 74.2, Unicode
# 15.1) - nihil ICU in aedificatione.
#
# Usus: ./tools/runae_oraculum.sh                 # omnis codex, discordiae
#       ./tools/runae_oraculum.sh -aurum          # scribit aurum_icu.txt
#       ./tools/runae_oraculum.sh -opentui [via]  # politica OpenTUI
#       (ceterae optiones ad instrumentum transeunt)

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"

source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
mkdir -p build
BIN="build/runae_oraculum"
clang "${GCC_FLAGS[@]}" -Iinclude tools/runae_oraculum.c lib/runae.c \
    lib/runae_tabulae.c -o "$BIN" || exit 2

case "${1:-}" in
    -aurum)
        shift
        exec "$BIN" -aurum probationes/fixa/runae/aurum_icu.txt "$@" ;;
    -opentui)
        shift
        ZON="${1:-$RADIX_DIR/../opentui/packages/native/src/tests/unicode-width-map.zon}"
        [ $# -gt 0 ] && shift
        exec "$BIN" -opentui "$ZON" "$@" ;;
    *)
        exec "$BIN" "$@" ;;
esac
