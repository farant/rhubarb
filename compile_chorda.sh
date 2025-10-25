#!/bin/bash

# Compiler flags
declare -a GCC_FLAGS=(
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
)

# Include paths
declare -a INCLUDE_FLAGS=(
    "-Iinclude"
)

# Build the compile command
COMPILE_CMD="gcc ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} -c lib/chorda/chorda.c -o /tmp/chorda.o"

do_compile() {
    echo -e "\033[34m======================="
    echo "COMPILING"
    echo -e "=======================\033[0m"
    eval $COMPILE_CMD
}

if [[ "$1" == "--watch" ]]; then
    do_compile
    echo ""
    echo "Watching for changes..."
    echo "Press Ctrl+C to stop"
    echo ""

    # Try fswatch first (macOS), fall back to inotifywait (Linux)
    if command -v fswatch &> /dev/null; then
        fswatch -e IsDirectory lib/chorda/ include/ | while read; do
            echo ""
            do_compile
        done
    elif command -v inotifywait &> /dev/null; then
        inotifywait -m -e modify -r lib/chorda/ include/ | while read; do
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
