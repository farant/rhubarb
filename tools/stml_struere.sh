#!/bin/bash

# tools/stml_struere.sh - bin/stml aedificare + in ~/.bin instituere
#
# Clausura per bin/aedilis --partes ex tools/stml.c ipso computatur
# (include/X.h -> lib/X.c sponte) - numquam build/*.o universum:
# silex omnia nectit et ideo obiecta .m trahit et compages Cocoa
# postulat; stml binarium parvum sine compagibus manet. Obiecta e
# build/ (suite ea recentia servat); si build/ vacuum aut pars
# deest, suite prima currenda (./compile_tests.sh).
#
# Usus: ./tools/stml_struere.sh

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$SCRIPT_DIR/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

if [ ! -x bin/aedilis ]; then
    echo "stml_struere: bin/aedilis deest - suite prima curre" \
        "(./compile_tests.sh)" >&2
    exit 1
fi

OBIECTA=()
while IFS=$'\t' read -r genus pars; do
    [ "$genus" = "O" ] || continue
    base="$(basename "$pars" .c)"
    obj="build/$base.o"
    if [ ! -f "$obj" ]; then
        echo "stml_struere: $obj deest - suite prima curre" \
            "(./compile_tests.sh)" >&2
        exit 1
    fi
    OBIECTA+=("$obj")
done < <(./bin/aedilis tools/stml.c --partes)

if [ "${#OBIECTA[@]}" -eq 0 ]; then
    echo "stml_struere: clausura vacua - aedilis defecit?" >&2
    exit 1
fi

mkdir -p bin
echo "  [stml] tools/stml.c + ${#OBIECTA[@]} obiecta clausurae"
clang "${GCC_FLAGS[@]}" -Iinclude tools/stml.c "${OBIECTA[@]}" \
    -o bin/stml || exit 1
echo "aedificatum: bin/stml"

# institutio: exemplum in ~/.bin (in PATH); rm ante cp (macOS
# binarium superscriptum SIGKILL dat - inodus recens obligatorius)
mkdir -p "$HOME/.bin"
rm -f "$HOME/.bin/stml"
cp bin/stml "$HOME/.bin/stml"
echo "institutum: ~/.bin/stml"
