#!/bin/bash

COMPILE_CMD="gcc -std=c89 -pedantic -Wall -Wextra -Werror -Iinclude -c lib/piscina/piscina.c -o /tmp/piscina.o"

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
        fswatch -e IsDirectory lib/piscina/ include/ | while read; do
            echo ""
            do_compile
        done
    elif command -v inotifywait &> /dev/null; then
        inotifywait -m -e modify -r lib/piscina/ include/ | while read; do
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
