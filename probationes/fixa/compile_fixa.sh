#!/bin/bash
# compile_fixa.sh - Compile test fixtures for Mach-O testing

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo "Compiling test fixtures..."

# Compile simplex - minimal binary
echo "  simplex..."
cc -o simplex simplex.c

# Compile cum_functione - binary with multiple functions
echo "  cum_functione..."
cc -o cum_functione cum_functione.c

echo "Done. Binaries compiled to probationes/fixa/"
echo ""
echo "Files created:"
ls -lh simplex cum_functione
