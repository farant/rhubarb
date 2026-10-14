#!/bin/bash
# oratio/reticulum.sh - instrumentum reticuli partitionum (T35 d): ordines
# machinae cum ordine COLUMNAE per titulum, partitiones retentae
#
# Usage:  ./oratio/reticulum.sh <x.tsv> -genus K -aurum aurum-...
#           (-sortes columna | -sortes-alternae N)
#           [-ubi titulus=v1,v2]... [-columnae t1,t2 | -praeter t1,t2]
#           [-gradus t1,t2] [-limen N] [-lucrum N] [-profunditas N] [-prima N]
#           [-initium t1,t2] [-greges columna|catena|catena-libera|initium] [-exempla N] [-machina]
#         -ubi valores crudos confert (ante -gradus); numeri decimales
# Exit:   0 iudicatum | 2 nihil iudicatum (causa in stderr), usus, plagula absens
# Instrumentum ex clausura sua; binarium in oratio/build/reticulum.
set -u
ORATIO_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
BUILD_DIR="$ORATIO_DIR/build"
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
BIN="$BUILD_DIR/reticulum"
SRC="$ORATIO_DIR/instrumenta/reticulum.c"
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, 2026-10-14):
# olim obiecta oratio/build/*.o ligabantur quae cursor ULTIMO struxerat
# (vetustas nulla iudicata - instrumentum contra fontes veteres)
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 2
# non exec: crusta manet ut sera suitae usque ad finem instrumenti teneatur
"$BIN" "$@"
exit $?
