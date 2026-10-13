#!/bin/bash
# toml/arbor.sh - proiectio STML plagulae toml (gemellus crusta/arbor.sh)
#
# Usage:  ./toml/arbor.sh <plagula.toml> [-tacitus] [-sedes]
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta toml/build/*.o quae cursor vetus struebat - cursores ad
# gradus T2: cursor nunc gradus est, obiecta illa non iam facit);
# binarium in toml/build/arbor.
set -u
TOML_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$TOML_DIR/.." && pwd)"
BUILD_DIR="$TOML_DIR/build"
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 1
cursor_instrumentum_struere "$TOML_DIR/instrumenta/arbor.c" "$BUILD_DIR/arbor" >&2 || exit 1
exec "$BUILD_DIR/arbor" "$@"
