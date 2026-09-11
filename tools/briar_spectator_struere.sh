#!/bin/bash
# tools/briar_spectator_struere.sh - bin/briar-spectator aedificare
#
# Spectator = briar + fenestra (spec par. 4.7). Obiecta briaris
# (briar/build/*.o) omnia fert quae redditio poscit; fenestra autem
# atrium/vitream/fenestram poscit, quae in build/ radicis iacent.
#
# LEX NEXUS: briar/build et build plagulas bibliothecae EASDEM ferunt
# (piscina.o, chorda.o...). Ergo e build/ ea SOLA sumuntur quorum
# nomen briar/build non habet - aliter symbola gemina.
#
# ORDO: ./compile_tests.sh registrum (obiecta radicis, .m inclusa) et
# ./briar/compile_probationes.sh registrum ANTE hoc.
#
# Usus: ./tools/briar_spectator_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
source "$SCRIPT_DIR/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

if [ -z "$(ls -A briar/build/briar_*.o 2>/dev/null)" ]; then
    echo "spectator: briar/build/ vacuum - ./briar/compile_probationes.sh registrum prius" >&2
    exit 1
fi
if [ -z "$(ls -A build/atrium.o build/vitrea_macos.o 2>/dev/null)" ]; then
    echo "spectator: build/ atrium aut vitrea caret - ./compile_tests.sh registrum prius" >&2
    exit 1
fi

source "$SCRIPT_DIR/corpus_infixum.sh"
corpus_infixum_regenerare || exit 1
source "$SCRIPT_DIR/briar_facies_capsula.sh"
briar_facies_capsula_regenerare || exit 1

# obiecta briaris (probationes exclusae)
OBJ=""
for o in briar/build/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done

# obiecta radicis quae briar/build NON habet (fenestra, vitrea, atrium,
# thema, imago... et quidquid clausura eorum poscit)
ADDITA=""
for o in build/*.o; do
    b="$(basename "$o")"
    case "$b" in probatio_*) continue ;; esac
    if [ ! -f "briar/build/$b" ]; then
        ADDITA="$ADDITA $o"
    fi
done

mkdir -p bin
echo "  [spectator] tools/briar_spectator.c + briar + atrium/vitrea"
clang "${GCC_FLAGS[@]}" -Iinclude -Imateria/fontes -Imd/fontes \
    -Ibriar/fontes -Isilva/amalgama \
    tools/briar_spectator.c build/capsula_corpus_silicis.c \
    $OBJ $ADDITA \
    -framework Cocoa -framework Security -framework WebKit \
    -o bin/briar-spectator || exit 1
echo "aedificatum: bin/briar-spectator"

# rm ante cp: macOS binarium superscriptum SIGKILL dat
mkdir -p "$HOME/.bin"
rm -f "$HOME/.bin/briar-spectator"
cp bin/briar-spectator "$HOME/.bin/briar-spectator"
echo "institutum: ~/.bin/briar-spectator"
