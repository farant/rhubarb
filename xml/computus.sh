#!/bin/bash
# xml/computus.sh - imago memoriae et temporis clientis xml (gemellus
# toml/computus.sh) - semita MATERIAE per clientem octavum
#
# usus: ./xml/computus.sh <plagula.xml> [-machina] [-iter N]
#
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, per thesaurum;
# exemplar toml/computus.sh); binarium in xml/build/computus.
set -u
XML_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$XML_DIR/.." && pwd)"
BUILD_DIR="$XML_DIR/build"
source "$RADIX_DIR/tools/cursor_communis.sh"
cursor_instrumenta_parare >&2 || exit 1
cursor_instrumentum_struere "$XML_DIR/instrumenta/computus.c" "$BUILD_DIR/computus" >&2 || exit 1
exec "$BUILD_DIR/computus" "$@"
