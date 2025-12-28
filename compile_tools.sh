#!/bin/bash

# Compiler flags (same as compile_tests.sh)
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

# Build directory for object files
BUILD_DIR="build"

# Color codes
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
YELLOW="\033[33m"
RESET="\033[0m"

print_usage() {
    echo "Usage: $0 <tool_name>"
    echo ""
    echo "Examples:"
    echo "  $0 gutenberg_census"
    echo "  $0 sputnik"
    echo ""
    echo "Available tools:"
    for tool in tools/*.c; do
        name=$(basename "$tool" .c)
        echo "  $name"
    done
}

compile_tool() {
    local tool_name="$1"
    local tool_file="tools/${tool_name}.c"
    local output_binary="bin/${tool_name}"

    if [ ! -f "$tool_file" ]; then
        echo -e "${RED}Error: $tool_file not found${RESET}"
        return 1
    fi

    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}Compiling Tool: $tool_name${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"

    # Make sure libraries are compiled
    if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls -A $BUILD_DIR/*.o 2>/dev/null)" ]; then
        echo -e "${YELLOW}Building libraries first...${RESET}"
        ./compile_tests.sh --libs-only 2>/dev/null || ./compile_tests.sh dummy 2>&1 | head -20
    fi

    # Create bin directory
    mkdir -p bin

    # Get object files
    local obj_files=$(ls $BUILD_DIR/*.o 2>/dev/null | tr '\n' ' ')

    if [ -z "$obj_files" ]; then
        echo -e "${RED}Error: No object files in $BUILD_DIR/${RESET}"
        echo "Run ./compile_tests.sh first to build libraries"
        return 1
    fi

    echo -e "Source: ${GREEN}$tool_file${RESET}"
    echo -e "Output: ${GREEN}$output_binary${RESET}"
    echo ""

    # Compile
    if clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$tool_file" $obj_files -framework Cocoa -framework Security -o "$output_binary" 2>&1; then
        echo ""
        echo -e "${GREEN}✓ $tool_name compiled successfully${RESET}"
        echo -e "Run with: ./$output_binary"
        return 0
    else
        echo ""
        echo -e "${RED}✗ $tool_name compilation failed${RESET}"
        return 1
    fi
}

# Main
if [ -z "$1" ]; then
    print_usage
    exit 1
fi

if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    print_usage
    exit 0
fi

compile_tool "$1"
exit $?
