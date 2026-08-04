#!/bin/sh

# tools/glomerare.sh - fontes JS in fasciculum unum glomerare
# (concatenatio cum provenientia - NON ESM: harness osascript
# modulos sequi nequit, et provenientia in commentariis debugationem
# ad fontem remittit. Ordo argumentorum = ordo fasciculi.)
#
# Usus: ./tools/glomerare.sh <exitus.js> <fons1.js> [fons2.js ...]

set -e
exitus="$1"
shift

{
    echo "/* GLOMERATUM a tools/glomerare.sh - NOLI MANU EDERE;"
    echo "   fontes veri infra nominantur. */"
    for fons in "$@"; do
        echo ""
        echo "/* ================ ex $fons ================ */"
        cat "$fons"
    done
} > "$exitus"
echo "glomeratum: $exitus ($# fontes)"
