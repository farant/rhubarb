#!/bin/bash

# Compiler flags (same strict settings as compile_tests.sh)
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
    "-Wno-long-long"
)

# Include paths
declare -a INCLUDE_FLAGS=(
    "-Iinclude"
)

# All library source files
declare -a SOURCE_FILES=(
    "lib/piscina.c"
    "lib/chorda.c"
    "lib/credo.c"
    "lib/tabula_dispersa.c"
    "lib/internamentum.c"
    "lib/chorda_aedificator.c"
    "lib/filum.c"
    "lib/xar.c"
    "lib/via.c"
    "lib/argumenta.c"
    "lib/macho.c"
    "lib/sectio.c"
    "lib/symbola.c"
    "lib/entitas.c"
    "lib/uuid.c"
    "lib/graphus_entitatum.c"
    "lib/color.c"
    "lib/thema.c"
    "lib/nuntium.c"
)

# Color codes
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
YELLOW="\033[33m"
RESET="\033[0m"

print_usage() {
    echo "Usage: $0 <library_name> [--watch]"
    echo ""
    echo "Examples:"
    echo "  $0 filum           # Compile lib/filum.c"
    echo "  $0 entitas         # Compile lib/entitas.c"
    echo "  $0 filum --watch   # Compile and watch for changes"
    echo ""
    echo "Available libraries:"
    for src in "${SOURCE_FILES[@]}"; do
        name=$(basename "$src" .c)
        echo "  $name"
    done
    echo "  fenestra  (special: fenestra_macos.m + fenestra_textus.c)"
}

compile_library() {
    local lib_name="$1"
    local lib_files=""
    local header_file="include/${lib_name}.h"
    local output_obj="/tmp/${lib_name}.o"
    local compiler="gcc"
    local extra_flags=""

    # Special case: fenestra has multiple source files and needs Objective-C
    if [ "$lib_name" == "fenestra" ]; then
        lib_files="lib/fenestra_macos.m lib/fenestra_textus.c"
        compiler="clang"
        extra_flags="-framework Cocoa"
    else
        lib_files="lib/${lib_name}.c"
    fi

    # Check if library file(s) exist
    for lib_file in $lib_files; do
        if [ ! -f "$lib_file" ]; then
            echo -e "${RED}Error: $lib_file not found${RESET}"
            return 1
        fi
    done

    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}Compiling: $lib_name${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"

    # Check header exists
    if [ -f "$header_file" ]; then
        echo -e "Header:  ${GREEN}$header_file${RESET}"
    else
        echo -e "Header:  ${YELLOW}$header_file (not found)${RESET}"
    fi
    for lib_file in $lib_files; do
        echo -e "Source:  ${GREEN}$lib_file${RESET}"
    done
    echo ""

    echo -e "${BLUE}Compiling...${RESET}"

    # Compile each source file
    local all_ok=true
    for lib_file in $lib_files; do
        local obj_name="/tmp/$(basename "$lib_file" | sed 's/\.[cm]$/.o/')"
        COMPILE_CMD="$compiler ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} -c $lib_file -o $obj_name"

        if ! eval $COMPILE_CMD 2>&1; then
            all_ok=false
        else
            rm -f "$obj_name"
        fi
    done

    if $all_ok; then
        echo ""
        echo -e "${GREEN}✓ $lib_name compiled successfully${RESET}"
        return 0
    else
        echo ""
        echo -e "${RED}✗ $lib_name compilation failed${RESET}"
        return 1
    fi
}

# Main execution
if [ -z "$1" ]; then
    print_usage
    exit 1
fi

if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    print_usage
    exit 0
fi

LIB_NAME="$1"

if [ "$2" == "--watch" ]; then
    compile_library "$LIB_NAME"

    if [ $? -eq 0 ]; then
        echo ""
        echo "Watching for changes..."
        echo "Press Ctrl+C to stop"
        echo ""

        # Try fswatch first (macOS), fall back to inotifywait (Linux)
        if command -v fswatch &> /dev/null; then
            fswatch -e IsDirectory "lib/${LIB_NAME}.c" "include/${LIB_NAME}.h" "include/" | while read; do
                clear
                compile_library "$LIB_NAME"
            done
        elif command -v inotifywait &> /dev/null; then
            inotifywait -m -e modify "lib/${LIB_NAME}.c" "include/${LIB_NAME}.h" "include/" | while read; do
                clear
                compile_library "$LIB_NAME"
            done
        else
            echo -e "${RED}Error: neither fswatch nor inotifywait found${RESET}"
            echo "Install fswatch (macOS) or inotify-tools (Linux)"
            exit 1
        fi
    else
        exit 1
    fi
else
    compile_library "$LIB_NAME"
    exit $?
fi
