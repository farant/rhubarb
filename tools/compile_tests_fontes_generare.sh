#!/bin/bash

# tools/compile_tests_fontes_generare.sh - listae fontium suite ex aedile
#
# VIA MEDIA (probatio sicca adoptionis, non adoptio): structura
# necte-omnia compile_tests.sh INTACTA manet - listae solae
# derivantur. Unio clausurarum aedilis super OMNES principales
# nexos (probationes praeter benchmark + instrumenta necte-omnia)
# -> snippet commissum quem compile_tests.sh fontat.
#
# Lineae: obiecta .c -> SOURCE_FILES, .m -> OBJC_SOURCES.
# vendor/ = linea propria compile_tests (VENDOR_SOURCES + vexilla
# venditoria); build/ = obiecta generata, nexus per-scopum
# (speculum) - ambae hic exclusae.
#
# Usus: ./tools/compile_tests_fontes_generare.sh
# Regeneratio: post bibliothecam / inclusionem / probationem novam
# (nexus fractus clamat - lista stala numquam tacite mentitur).

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

EXITUS="compile_tests_fontes_generata.sh"

if [ ! -x bin/aedilis ]; then
    ./tools/aedilis_struere.sh || exit 1
fi

TEMPORARIUM=$(mktemp)
trap 'rm -f "$TEMPORARIUM"' EXIT

for f in probationes/probatio_*.c \
         tools/generare.c tools/capsula_generare.c \
         instrumenta/nuntium_schema_generare.c; do
    case "$f" in *_benchmark.c) continue ;; esac
    if ! ./bin/aedilis "$f" --enumerare >> "$TEMPORARIUM"; then
        echo "compile_tests_fontes_generare: derivatio fracta: $f" >&2
        exit 1
    fi
done

UNIO_C=$(sort -u "$TEMPORARIUM" | awk '!/^build\// && !/^vendor\// && /\.c$/')
UNIO_M=$(sort -u "$TEMPORARIUM" | awk '!/^build\// && !/^vendor\// && /\.m$/')

if [ -z "$UNIO_C" ]; then
    echo "compile_tests_fontes_generare: unio vacua - recusatum" >&2
    exit 1
fi

# obiecta plana (build/<basis>.o) - collisio basium obiectum prius
# tacite obrueret; recusa clare
DUPLICATA=$(printf '%s\n%s\n' "$UNIO_C" "$UNIO_M" \
    | awk -F/ 'NF { print $NF }' | sort | uniq -d)
if [ -n "$DUPLICATA" ]; then
    echo "compile_tests_fontes_generare: bases obiectorum collidunt:" >&2
    echo "$DUPLICATA" >&2
    exit 1
fi

{
    echo "# $EXITUS - GENERATUM AB AEDILE - NE MANU EDITES"
    echo "# unio clausurarum omnium principalium suite (necte-omnia)"
    echo "# regeneratio: ./tools/compile_tests_fontes_generare.sh"
    echo "declare -a SOURCE_FILES=("
    echo "$UNIO_C" | while IFS= read -r f; do echo "    \"$f\""; done
    echo ")"
    echo "declare -a OBJC_SOURCES=("
    echo "$UNIO_M" | while IFS= read -r f; do echo "    \"$f\""; done
    echo ")"
} > "$EXITUS"

echo "$EXITUS scriptum ($(echo "$UNIO_C" | wc -l | tr -d ' ') C + $(echo "$UNIO_M" | wc -l | tr -d ' ') ObjC)"
