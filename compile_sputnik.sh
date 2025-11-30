#!/bin/bash

# Compile sputnik CLI tool

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
    "-Wno-long-long"
)

# Include paths
declare -a INCLUDE_FLAGS=(
    "-Iinclude"
)

# Source files
declare -a SOURCE_FILES=(
    "lib/piscina.c"
    "lib/chorda.c"
    "lib/friatio.c"
    "lib/tabula_dispersa.c"
    "lib/internamentum.c"
    "lib/chorda_aedificator.c"
    "lib/filum.c"
    "lib/xar.c"
    "lib/via.c"
    "lib/argumenta.c"
    "lib/uuid.c"
    "lib/nuntium.c"
    "lib/entitas.c"
    "lib/persistentia_memoria.c"
    "lib/persistentia_nuntium.c"
    "lib/entitas_repositorium_impl.c"
    "lib/sputnik_lexema.c"
    "lib/sputnik_parser.c"
    "lib/sputnik_interpres.c"
)

# Color codes
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
RESET="\033[0m"

BUILD_DIR="build"
OUTPUT="bin/sputnik"

echo -e "${BLUE}═══════════════════════════════════════${RESET}"
echo -e "${BLUE}COMPILING SPUTNIK CLI${RESET}"
echo -e "${BLUE}═══════════════════════════════════════${RESET}"

mkdir -p "$BUILD_DIR"
mkdir -p "bin"

# Compile source files to object files
for src_file in "${SOURCE_FILES[@]}"; do
    obj_name=$(basename "$src_file" .c).o
    obj_file="$BUILD_DIR/$obj_name"

    if [ ! -f "$obj_file" ] || [ "$src_file" -nt "$obj_file" ]; then
        echo "  Compiling: $src_file"
        if ! clang -c ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$src_file" -o "$obj_file" 2>&1; then
            echo -e "${RED}✗ FAILED: $src_file${RESET}"
            exit 1
        fi
    fi
done

# Build object file list
OBJ_FILES=""
for src_file in "${SOURCE_FILES[@]}"; do
    obj_name=$(basename "$src_file" .c).o
    OBJ_FILES="$OBJ_FILES $BUILD_DIR/$obj_name"
done

# Compile and link sputnik tool
echo "  Linking: tools/sputnik.c"
if ! clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} tools/sputnik.c $OBJ_FILES -o "$OUTPUT" -lm 2>&1; then
    echo -e "${RED}✗ LINK FAILED${RESET}"
    exit 1
fi

echo ""
echo -e "${GREEN}✓ Built: $OUTPUT${RESET}"
echo ""
echo "Usage:"
echo "  $OUTPUT script.sputnik"
echo "  $OUTPUT -e \"print(2 + 2);\""
echo "  $OUTPUT --help"
