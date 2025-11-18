#!/bin/bash

# Compiler flags
declare -a CLANG_FLAGS=(
    "-std=c89"
    "-pedantic"
    "-Wall"
    "-Wextra"
    "-Werror"
    "-Wconversion"
    "-Wsign-conversion"
    "-Wcast-qual"
    "-Wstrict-prototypes"
    "-Wmissing-prototypes"
    "-Wwrite-strings"
    "-Wno-long-long"
)

# Include paths
declare -a INCLUDE_FLAGS=(
    "-Iinclude"
)

do_compile() {
    echo -e "\033[34m======================="
    echo "COMPILING TEMPUS"
    echo -e "=======================\033[0m"

    # Compile tempus.c
    echo "Compiling tempus.c..."
    if ! clang ${CLANG_FLAGS[@]} ${INCLUDE_FLAGS[@]} -c lib/tempus.c -o /tmp/tempus.o; then
        echo -e "\033[31mFailed to compile tempus.c\033[0m"
        return 1
    fi

    # Compile fenestra_macos.m (needed for platform timing functions)
    echo "Compiling fenestra_macos.m..."
    if ! clang ${CLANG_FLAGS[@]} ${INCLUDE_FLAGS[@]} -c lib/fenestra_macos.m -o /tmp/fenestra_macos.o; then
        echo -e "\033[31mFailed to compile fenestra_macos.m\033[0m"
        return 1
    fi

    echo -e "\033[32m✓ Compilation successful\033[0m"
}

if [[ "$1" == "--watch" ]]; then
    do_compile
    echo ""
    echo "Watching for changes..."
    echo "Press Ctrl+C to stop"
    echo ""

    # Try fswatch first (macOS), fall back to inotifywait (Linux)
    if command -v fswatch &> /dev/null; then
        fswatch -e IsDirectory lib/ include/ | while read; do
            echo ""
            do_compile
        done
    elif command -v inotifywait &> /dev/null; then
        inotifywait -m -e modify -r lib/ include/ | while read; do
            echo ""
            do_compile
        done
    else
        echo "Error: neither fswatch nor inotifywait found"
        echo "Install fswatch (macOS) or inotify-tools (Linux)"
        exit 1
    fi
else
    do_compile
fi
