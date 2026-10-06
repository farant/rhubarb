#!/bin/bash
# tools/dispositio_oraculum.sh - oraculum dispositionis: Clay (../clay @
# e6cc369) arborem STML disponit (project-specs/dispositio-plan.md D0)
#
# Usus:
#   ./tools/dispositio_oraculum.sh <arbor.stml>   # fines per nodum
#   ./tools/dispositio_oraculum.sh -probare       # fixa oracula/clay/probationes
#   CLAY_DIR=<via> ...                            # clonus alius (probatio)
#
# Glutinum C99 (oracula/clay/clay_vinculum.c) RELAXATE compilatur, ut
# vendicata; ductor Latinus et bibliothecae domus C89. Exitus: 0
# congruit/scriptum; 1 discrepantia aut error; 2 NIHIL CUCURRIT (clonus
# deest aut commissum alienum - nominatur).
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
RADIX_DIR="$PWD"
CLAY_DIR="${CLAY_DIR:-$RADIX_DIR/../clay}"
CLAY_COMMISSUM="e6cc369"
BUILD="build/oraculum_dispositio"

si_fracta() { echo "dispositio_oraculum: $1" >&2; exit "${2:-1}"; }

# ---- clavus: clonus ad commissum fixum ----
[ -f "$CLAY_DIR/clay.h" ] \
    || si_fracta "clonus Clay deest: $CLAY_DIR (git clone https://github.com/nicbarker/clay ../clay; git -C ../clay checkout $CLAY_COMMISSUM)" 2
caput="$(git -C "$CLAY_DIR" rev-parse --short=7 HEAD 2>/dev/null)"
[ "$caput" = "$CLAY_COMMISSUM" ] \
    || si_fracta "Clay ad '$caput', non ad $CLAY_COMMISSUM (oraculum fixum: git -C $CLAY_DIR checkout $CLAY_COMMISSUM)" 2

# ---- constructio ----
source tools/vexilla.sh
mkdir -p "$BUILD"
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 2
obiecta=()
while IFS= read -r fons; do
    case "$fons" in lib/*.c) ;; *) continue ;; esac
    obj="$BUILD/$(basename "$fons" .c).o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$fons" ]; then
        clang "${VEXILLA_C89[@]}" -Iinclude -c "$fons" -o "$obj" || si_fracta "$fons" 2
    fi
    obiecta+=("$obj")
done < <(./bin/aedilis oracula/clay/dispositio_oraculum.c --enumerare 2>/dev/null)
clang "${VEXILLA_C89[@]}" -Iinclude -Ioracula/clay \
    -c oracula/clay/dispositio_oraculum.c -o "$BUILD/ductor.o" || si_fracta "ductor" 2
clang -std=c99 -O1 -w -I"$CLAY_DIR" -Ioracula/clay \
    -c oracula/clay/clay_vinculum.c -o "$BUILD/clay_vinculum.o" || si_fracta "vinculum" 2
clang "$BUILD/ductor.o" "$BUILD/clay_vinculum.o" "${obiecta[@]}" \
    -o "$BUILD/dispositio_oraculum" || si_fracta "nexus" 2

# ---- cursus ----
if [ "${1:-}" = "-probare" ]; then
    n=0; fractae=0
    for arbor in oracula/clay/probationes/*.stml; do
        exspectata="${arbor%.stml}.exspectata"
        n=$((n + 1))
        if "$BUILD/dispositio_oraculum" "$arbor" | diff -u "$exspectata" - ; then
            echo "  sanum   $(basename "$arbor")"
        else
            echo "  FRACTUM $(basename "$arbor")"
            fractae=$((fractae + 1))
        fi
    done
    [ "$n" -gt 0 ] || si_fracta "nulla fixa in oracula/clay/probationes" 2
    echo "dispositio_oraculum: $((n - fractae))/$n congruunt (Clay $CLAY_COMMISSUM)"
    [ "$fractae" -eq 0 ] || exit 1
    exit 0
fi
[ $# -eq 1 ] || si_fracta "usus: $0 <arbor.stml> | -probare" 2
exec "$BUILD/dispositio_oraculum" "$1"
