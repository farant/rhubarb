#!/bin/bash
# tools/briar_struere.sh - bin/briar aedificare et in ~/.bin instituere
#
# Obiecta: briar/build/*.o (cursor briar/compile_probationes.sh ea
# struit: clausura silicis + sqlite + amalgama silvae + materia + md +
# briar) + corpus infixum build/capsula_corpus_silicis.c (bloccus
# COMMUNIS cum silex_struere.sh: tools/corpus_infixum.sh). ORDO: suite
# briar prius (obiecta), deinde hoc. Corpus e radice arboris legitur -
# in arbore secundaria (worktree) stampa eius commissum dicit.
#
# Usus: ./tools/briar_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
source "$SCRIPT_DIR/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

if [ -z "$(ls -A briar/build/briar_*.o 2>/dev/null)" ]; then
    echo "briar_struere: briar/build/ vacuum - ./briar/compile_probationes.sh registrum prius" >&2
    exit 1
fi
if [ ! -x bin/capsula_generare ] && [ -z "$(ls -A build/*.o 2>/dev/null)" ]; then
    echo "briar_struere: build/ vacuum et bin/capsula_generare abest - ./compile_tests.sh registrum prius" >&2
    exit 1
fi

source "$SCRIPT_DIR/corpus_infixum.sh"
corpus_infixum_regenerare || exit 1

# obiecta: omnia briar/build/*.o praeter probationes (obiecta
# probationum ibi non cadunt, sed custodia eadem quae silex_struere)
OBJ=""
for o in briar/build/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done

mkdir -p bin
echo "  [briar] tools/briar.c + corpus"
clang "${GCC_FLAGS[@]}" -Iinclude -Imateria/fontes -Imd/fontes \
    -Ibriar/fontes -Isilva/amalgama \
    tools/briar.c build/capsula_corpus_silicis.c $OBJ \
    -o bin/briar || exit 1
echo "aedificatum: bin/briar"

# institutio: exemplum in ~/.bin (in PATH) - '#!/usr/bin/env briar'
# eum ibi invenit; rm ante cp (macOS binarium superscriptum SIGKILL
# dat - inodus recens obligatorius)
mkdir -p "$HOME/.bin"
rm -f "$HOME/.bin/briar"
cp bin/briar "$HOME/.bin/briar"
echo "institutum: ~/.bin/briar"
