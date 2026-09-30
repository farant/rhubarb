#!/bin/bash
# tools/provenientia_scribere.sh - plagulam provenientiae scribere
# (plan fabrica 1a T7; include/provenientia.h)
#
# Scribit build/fabrica/provenientia/TITULUS.c:
#   constans ProvenientiaRelatio provenientia_TITULUS = {...};
# cum ingressu = 'bin/fabrica digestum TITULUS' (sigillum IDEM quod iudex
# computat: fabrica_actionem_sigillare, plagula haec exclusa). Installator
# ante struere.sh vocat; binarium eam per annotationem aedilis nectit.
#
# Plagula SOLUM si contentum mutatur scribitur; tunc obiectum eius
# DELETUR - struere.sh ab aedile genitum per mtime iudicat, et scriptura
# eodem secundo quo compilatio (T3, ictus in parco 01KZYN4VPZ) binarium
# cum digesto vetere relinqueret.
#
# Usus: ./tools/provenientia_scribere.sh TITULUS ARTIFICIUM
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
T="${1:?usus: provenientia_scribere.sh TITULUS ARTIFICIUM}"
A="${2:?usus: provenientia_scribere.sh TITULUS ARTIFICIUM}"
DIR="build/fabrica/provenientia"
EXITUS="$DIR/$T.c"
OBIECTUM="build/aedilis/obiecta/build__fabrica__provenientia__$T.o"
mkdir -p "$DIR" || exit 1

# bootstrap: sine bin/fabrica digestum ignotum (bin/fabrica ipse bis
# struitur - tools/fabrica_struere.sh)
# PROVENIENTIA_TOLERANS=1 (cursus primus fabricae ipsius): binarium
# PRIOR exclusionem plagulae provenientiae nescire potest (ante T7
# structum) - tunc 'ignotum' scribitur, cursus secundus corrigit
if [ -x bin/fabrica ] && H="$(bin/fabrica digestum "$T" 2>/dev/null)"; then
    :
elif [ "${PROVENIENTIA_TOLERANS:-0}" = 1 ] || [ ! -x bin/fabrica ]; then
    H="ignotum"
else
    echo "provenientia_scribere: digestum $T fractum:" >&2
    bin/fabrica digestum "$T" >&2
    exit 1
fi
C="$(git rev-parse --short=8 HEAD 2>/dev/null || echo ignotum)"
[ -n "$(git status --porcelain --untracked-files=no 2>/dev/null)" ] && C="$C SORDIDUM"

NOVUM="$(mktemp)"
trap 'rm -f "$NOVUM"' EXIT
cat > "$NOVUM" <<CORPUS
/* $T.c - GENERATUM a tools/provenientia_scribere.sh - NE MANU EDITES */
#include "provenientia.h"

constans ProvenientiaRelatio provenientia_$T = {
    "$A",
    "$H",
    "$C"
};
CORPUS
if ! cmp -s "$NOVUM" "$EXITUS"; then
    cp "$NOVUM" "$EXITUS" || exit 1
    rm -f "$OBIECTUM"
fi
