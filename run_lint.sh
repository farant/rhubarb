#!/bin/bash

# Compile lint tool
echo "Compiling lint_probatio..."

# Just compile libraries (not run tests) - check if .o files exist
if [ ! -f "build/piscina.o" ]; then
    echo "Building libraries first..."
    ./compile_tests.sh > /dev/null 2>&1
fi

clang -std=c89 -pedantic \
    -Wall -Wextra -Werror \
    -Wconversion -Wsign-conversion \
    -Wcast-qual -Wstrict-prototypes \
    -Wmissing-prototypes -Wwrite-strings \
    -Wno-long-long \
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
