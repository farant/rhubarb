#!/bin/bash
# oratio/vocabula.sh - recensio vocabulorum: identificatores et commentaria (Latine, T10) aut prosa markdown (Anglice, T15a) contra vocabularia
#
# Usage:  ./oratio/vocabula.sh [-symbola | -commenta | -omnia | -prosa | -nova] [-machina] [-omnes] [-omnes-viae] [-tectum N]
#         -nova = verba ignota NOVA contra copiam toleratam cum exitibus (ante commissionem; exitus 1 si adsunt)
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Obiecta oratio/build/*.o poscit (cursor oratio/compile_probationes.sh ea
# struit); instrumentum in oratio/build/vocabula.
set -u
ORATIO_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
MATERIA_DIR="$RADIX_DIR/materia"
BUILD_DIR="$ORATIO_DIR/build"
# SERA suitae orationis (tools/sera.sh, 2026-09-07): obiecta et
# binaria in oratio/build sub cursore currenti non tanguntur; cursor
# ab hoc vocatus reentrat; exitus 2 si tenta post SERA_TECTUM.
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
BIN="$BUILD_DIR/vocabula"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$MATERIA_DIR/fontes" "-I$ORATIO_DIR/fontes")
SRC="$ORATIO_DIR/instrumenta/vocabula.c"
if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls "$BUILD_DIR"/oratio_*.o 2>/dev/null)" ]; then
    # SE STRUENS (2026-09-07, uncus commissionis in arbore recenti):
    # obiecta per cursorem (porta minima 'registrum') struuntur, non
    # recusantur - uncus qui iudicem struere non potest porta muta est
    echo "vocabula: obiecta absunt - struuntur per cursorem (registrum), semel" >&2
    mkdir -p "$RADIX_DIR/build"
    if ! "$ORATIO_DIR/compile_probationes.sh" registrum > "$RADIX_DIR/build/vocabula_structura.log" 2>&1; then
        echo "vocabula: obiecta strui non potuerunt (build/vocabula_structura.log)" >&2
        exit 2
    fi
fi
OBJ=""
for o in "$BUILD_DIR"/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$SRC" ] || [ -n "$(find "$BUILD_DIR" -name 'oratio_*.o' -newer "$BIN" 2>/dev/null)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$SRC" $OBJ -o "$BIN" || exit 1
fi
git -C "$RADIX_DIR" ls-files "lib/*.c" "silva/fontes/*.c" > "$BUILD_DIR/corpus_c.txt"
git -C "$RADIX_DIR" ls-files "*.c" "*.h" > "$BUILD_DIR/corpus_tractatae.txt"
git -C "$RADIX_DIR" ls-files "*.md" > "$BUILD_DIR/corpus_md.txt"
# non exec: crusta manet ut sera suitae usque ad finem instrumenti
# teneatur et in exitu dimittatur (exec trap EXIT praeterit -
# sera cum pid mortuo relicta, 2026-09-07)
RHUBARB_RADIX="$RADIX_DIR" "$BIN" "$@"
exit $?
