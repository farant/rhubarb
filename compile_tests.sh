#!/bin/bash

# Compiler flags (same strict settings as compile_chorda.sh)
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

# Source files to link
declare -a SOURCE_FILES=(
  "lib/piscina.c"
  "lib/chorda.c"
  "lib/credo.c"
	"lib/tabula_dispersa.c"
	"lib/chorda_aedificator.c"
	"lib/filum.c"
	"lib/xar.c"
	"lib/via.c"
	"lib/argumenta.c"
)

# Color codes
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
YELLOW="\033[33m"
RESET="\033[0m"

# Test results
TESTS_TOTAL=0
TESTS_PASSED=0
TESTS_FAILED=0

compile_and_run_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .c)
    local output_binary="/tmp/$test_name"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))

    echo -e "${BLUE}────────────────────────────────────────${RESET}"
    echo -e "${BLUE}Testing: $test_name${RESET}"
    echo -e "${BLUE}────────────────────────────────────────${RESET}"

    # Compile
    COMPILE_CMD="gcc ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} $test_file ${SOURCE_FILES[@]} -o $output_binary"

    if ! eval $COMPILE_CMD 2>&1; then
        echo -e "${RED}✗ COMPILATION FAILED: $test_name${RESET}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        echo ""
        return 1
    fi

    # Run
    if ! $output_binary 2>&1; then
        echo -e "${RED}✗ TEST FAILED: $test_name${RESET}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        echo ""
        return 1
    fi

    echo -e "${GREEN}✓ TEST PASSED: $test_name${RESET}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
    echo ""
    return 0
}

run_all_tests() {
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}RUNNING ALL TESTS${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo ""

    # Find all test_*.c files in probationes/ directory
    local test_files=$(find probationes -name "probatio_*.c" -type f | sort)

    if [ -z "$test_files" ]; then
        echo -e "${YELLOW}No test files found in probationes/ directory${RESET}"
        return 1
    fi

    # Compile and run each test
    while IFS= read -r test_file; do
        compile_and_run_test "$test_file"
    done <<< "$test_files"
}

print_summary() {
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}TEST SUMMARY${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo "Total:  $TESTS_TOTAL"
    echo -e "Passed: ${GREEN}$TESTS_PASSED${RESET}"
    echo -e "Failed: ${RED}$TESTS_FAILED${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo ""
}

# Main execution
if [ "$1" == "--watch" ]; then
    run_all_tests
    print_summary

    if [ $TESTS_FAILED -eq 0 ]; then
        echo "Watching for changes..."
        echo "Press Ctrl+C to stop"
        echo ""

        # Try fswatch first (macOS), fall back to inotifywait (Linux)
        if command -v fswatch &> /dev/null; then
            fswatch -e IsDirectory probationes/ lib/ include/ | while read; do
                clear
                run_all_tests
                print_summary
            done
        elif command -v inotifywait &> /dev/null; then
            inotifywait -m -e modify -r probationes/ lib/ include/ | while read; do
                clear
                run_all_tests
                print_summary
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
    run_all_tests
    print_summary

    if [ $TESTS_FAILED -eq 0 ]; then
        exit 0
    else
        exit 1
    fi
fi
