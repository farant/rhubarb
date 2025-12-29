#!/bin/bash

# run_benchmark.sh - Compile and run flatura benchmark
#
# USUS:
#   ./run_benchmark.sh [megabytes]
#   Default: 100 MB

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
    "-O2"
)

# Include paths
declare -a INCLUDE_FLAGS=(
    "-Iinclude"
)

# Source files needed for benchmark
declare -a SOURCE_FILES=(
    "lib/piscina.c"
    "lib/flatura.c"
)

# Build directory
BUILD_DIR="build"

# Colors
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
YELLOW="\033[33m"
RESET="\033[0m"

# Get megabytes argument (default 100)
MEGABYTES="${1:-100}"

echo -e "${BLUE}═══════════════════════════════════════${RESET}"
echo -e "${BLUE}FLATURA BENCHMARK${RESET}"
echo -e "${BLUE}═══════════════════════════════════════${RESET}"
echo ""

# Create build directory
mkdir -p "$BUILD_DIR"
mkdir -p bin

# Compile source files
echo -e "${BLUE}Compiling...${RESET}"
for src_file in "${SOURCE_FILES[@]}"; do
    obj_name=$(basename "$src_file" .c).o
    obj_file="$BUILD_DIR/$obj_name"

    if [ ! -f "$obj_file" ] || [ "$src_file" -nt "$obj_file" ]; then
        echo "  $src_file"
        if ! clang -c ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$src_file" -o "$obj_file" 2>&1; then
            echo -e "${RED}✗ FAILED: $src_file${RESET}"
            exit 1
        fi
    fi
done

# Compile benchmark
echo "  probationes/probatio_flatura_benchmark.c"
if ! clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} \
    probationes/probatio_flatura_benchmark.c \
    $BUILD_DIR/piscina.o \
    $BUILD_DIR/flatura.o \
    -o bin/probatio_flatura_benchmark 2>&1; then
    echo -e "${RED}✗ FAILED: benchmark compilation${RESET}"
    exit 1
fi

echo -e "${GREEN}✓ Compiled${RESET}"
echo ""

# Run benchmark
echo -e "${BLUE}Running benchmark with ${MEGABYTES} MB...${RESET}"
./bin/probatio_flatura_benchmark "$MEGABYTES"

echo -e "${BLUE}═══════════════════════════════════════${RESET}"

# Also show gzip reference if available
echo ""
echo -e "${YELLOW}Reference: system gzip (same test pattern)${RESET}"

# Use the compiled benchmark to generate identical data, then pipe to gzip
# For now, just time gzip on similar random-word data
TEMP_FILE=$(mktemp)

# Generate similar word-based content
perl -e '
    @w = qw(the and of to a in that is was he for it with as his on be at by I
            this had not are but from or have an they which one you were her all
            she there would their we him been has when who will more no if out so);
    srand(12345);
    for (1..($ARGV[0]*1024*1024/5)) { print $w[rand @w], " "; }
' "$MEGABYTES" > "$TEMP_FILE" 2>/dev/null

FILESIZE=$(wc -c < "$TEMP_FILE" | tr -d ' ')
FILEMB=$(echo "scale=1; $FILESIZE / 1048576" | bc)

echo "  Test data: ${FILEMB} MB"

# Benchmark gzip -1
START=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
gzip -1 < "$TEMP_FILE" > /dev/null
END=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
ELAPSED=$(echo "$END - $START" | bc)
MB_SEC=$(echo "scale=1; $FILEMB / $ELAPSED" | bc 2>/dev/null || echo "?")
echo "  gzip -1:   ${ELAPSED}s  ${MB_SEC} MB/s"

# Benchmark gzip -6
START=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
gzip -6 < "$TEMP_FILE" > /dev/null
END=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
ELAPSED=$(echo "$END - $START" | bc)
MB_SEC=$(echo "scale=1; $FILEMB / $ELAPSED" | bc 2>/dev/null || echo "?")
echo "  gzip -6:   ${ELAPSED}s  ${MB_SEC} MB/s"

rm -f "$TEMP_FILE"

echo ""
