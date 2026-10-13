#!/bin/bash
# toml/computus.sh - imago memoriae et temporis clientis toml (gemellus
# crusta/computus.sh) - semita MATERIAE per clientem septimum
#
# usus: ./toml/computus.sh <plagula.toml> [-machina] [-iter N]
#
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# olim obiecta toml/build/*.o quae cursor vetus struebat - cursores ad
# gradus T2); binarium in toml/build/computus.
set -u
TOML_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$TOML_DIR/.." && pwd)"
BUILD_DIR="$TOML_DIR/build"
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 1
cursor_instrumentum_struere "$TOML_DIR/instrumenta/computus.c" "$BUILD_DIR/computus" >&2 || exit 1
exec "$BUILD_DIR/computus" "$@"
