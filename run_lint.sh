#!/bin/bash

# Compile lint tool
# VEXILLA EX UNA SEDE (tools/vexilla.sh): tabula hic scripta erat, et
# regula II lintris eam invenit (2026-09-18).
RADIX_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

echo "Compiling lint_probatio..."

# Just compile libraries (not run tests) - check if .o files exist
if [ ! -f "build/piscina.o" ]; then
    echo "Building libraries first..."
    ./compile_tests.sh > /dev/null 2>&1
fi

clang "${GCC_FLAGS[@]}" \
    -Iinclude -Ilib \
    lint/lint_probatio.c \
    build/*.o \
    -framework Cocoa -framework Security \
    -o bin/lint_probatio 2>&1

if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

echo "Compilation successful"
echo ""

# Run on all probatio files if no args, otherwise use provided args
if [ $# -eq 0 ]; then
    ./bin/lint_probatio probationes/probatio_*.c
else
    ./bin/lint_probatio "$@"
fi
