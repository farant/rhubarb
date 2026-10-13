#!/bin/bash
# css/computus.sh - imago memoriae et temporis parseris CSS (gemellus
# silva/computus.sh; RP par. 11) - semita MATERIAE per clientem primum
#
# usus: ./css/computus.sh <plagula.css> [-machina] [-iter N]
#
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta css/build/*.o quae cursor vetus struebat - cursores ad
# gradus T3); binarium in css/build/computus.
set -u
CSS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$CSS_DIR/.." && pwd)"
BUILD_DIR="$CSS_DIR/build"
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 1
cursor_instrumentum_struere "$CSS_DIR/instrumenta/computus.c" "$BUILD_DIR/computus" >&2 || exit 1
exec "$BUILD_DIR/computus" "$@"
