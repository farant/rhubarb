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
    "-Wno-long-long"
)

# Include paths
declare -a INCLUDE_FLAGS=(
    "-Iinclude"
)

# Source files to compile to object files
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
    "lib/graphus_persistens.c"
    "lib/friatio.c"
    "lib/xml.c"
    "lib/stml.c"
    "lib/selectio.c"
    "lib/widget.c"
    "lib/pagina.c"
    "lib/navigator_entitatum.c"
    "lib/delineare.c"
    "lib/fenestra_textus.c"
    "lib/tempus.c"
    "lib/layout.c"
    "lib/registrum_commandi.c"
)

# Objective-C source (compiled separately)
OBJC_SOURCE="lib/fenestra_macos.m"

# Build directory for object files
BUILD_DIR="build"

# Color codes
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
YELLOW="\033[33m"
RESET="\033[0m"

# Filter pattern (e.g., "stml" to run only probatio_stml.c)
FILTER=""

# Test results
TESTS_TOTAL=0
TESTS_PASSED=0
TESTS_FAILED=0

# GUI app results
GUI_APPS_BUILT=0
GUI_APPS_FAILED=0

# Track if libraries need recompilation
LIBS_COMPILED=0

# Compile all library source files to object files
compile_libraries() {
    local needs_compile=0
    local src_file
    local obj_file
    local obj_name

    # Create build directory
    mkdir -p "$BUILD_DIR"

    # Check if any source file is newer than its object file
    for src_file in "${SOURCE_FILES[@]}"; do
        obj_name=$(basename "$src_file" .c).o
        obj_file="$BUILD_DIR/$obj_name"

        if [ ! -f "$obj_file" ] || [ "$src_file" -nt "$obj_file" ]; then
            needs_compile=1
            break
        fi
    done

    # Also check Objective-C file
    obj_file="$BUILD_DIR/fenestra_macos.o"
    if [ ! -f "$obj_file" ] || [ "$OBJC_SOURCE" -nt "$obj_file" ]; then
        needs_compile=1
    fi

    # Check if any header changed
    for header in include/*.h; do
        for src_file in "${SOURCE_FILES[@]}"; do
            obj_name=$(basename "$src_file" .c).o
            obj_file="$BUILD_DIR/$obj_name"
            if [ -f "$obj_file" ] && [ "$header" -nt "$obj_file" ]; then
                needs_compile=1
                break 2
            fi
        done
    done

    if [ $needs_compile -eq 0 ] && [ $LIBS_COMPILED -eq 0 ]; then
        echo -e "${BLUE}Libraries up to date${RESET}"
        return 0
    fi

    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}COMPILING LIBRARIES${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"

    # Compile C source files
    for src_file in "${SOURCE_FILES[@]}"; do
        obj_name=$(basename "$src_file" .c).o
        obj_file="$BUILD_DIR/$obj_name"

        # Only recompile if source is newer than object
        if [ ! -f "$obj_file" ] || [ "$src_file" -nt "$obj_file" ]; then
            echo -e "  Compiling: $src_file"
            if ! clang -c ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$src_file" -o "$obj_file" 2>&1; then
                echo -e "${RED}✗ FAILED: $src_file${RESET}"
                return 1
            fi
        fi
    done

    # Compile Objective-C file
    obj_file="$BUILD_DIR/fenestra_macos.o"
    if [ ! -f "$obj_file" ] || [ "$OBJC_SOURCE" -nt "$obj_file" ]; then
        echo -e "  Compiling: $OBJC_SOURCE"
        if ! clang -c ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$OBJC_SOURCE" -o "$obj_file" 2>&1; then
            echo -e "${RED}✗ FAILED: $OBJC_SOURCE${RESET}"
            return 1
        fi
    fi

    echo -e "${GREEN}Libraries compiled${RESET}"
    echo ""
    LIBS_COMPILED=1
    return 0
}

# Get all object files for linking
get_object_files() {
    local obj_files=""
    local src_file
    local obj_name

    for src_file in "${SOURCE_FILES[@]}"; do
        obj_name=$(basename "$src_file" .c).o
        obj_files="$obj_files $BUILD_DIR/$obj_name"
    done

    # Add Objective-C object
    obj_files="$obj_files $BUILD_DIR/fenestra_macos.o"

    echo "$obj_files"
}

compile_gui_app() {
    local app_file="$1"
    local app_name=$(basename "$app_file" .c)
    local output_binary="bin/$app_name"
    local obj_files

    echo -e "${BLUE}────────────────────────────────────────${RESET}"
    echo -e "${BLUE}Building GUI: $app_name${RESET}"
    echo -e "${BLUE}────────────────────────────────────────${RESET}"

    # Create bin directory if it doesn't exist
    mkdir -p bin

    obj_files=$(get_object_files)

    # Compile test file and link with object files
    if ! clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$app_file" $obj_files -framework Cocoa -o "$output_binary" 2>&1; then
        echo -e "${RED}✗ BUILD FAILED: $app_name${RESET}"
        GUI_APPS_FAILED=$((GUI_APPS_FAILED + 1))
        echo ""
        return 1
    fi

    echo -e "${GREEN}✓ GUI APP BUILT: $app_name (run with: ./$output_binary)${RESET}"
    GUI_APPS_BUILT=$((GUI_APPS_BUILT + 1))
    echo ""
    return 0
}

compile_and_run_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .c)
    local output_binary="/tmp/$test_name"
    local obj_files

    TESTS_TOTAL=$((TESTS_TOTAL + 1))

    echo -e "${BLUE}────────────────────────────────────────${RESET}"
    echo -e "${BLUE}Testing: $test_name${RESET}"
    echo -e "${BLUE}────────────────────────────────────────${RESET}"

    obj_files=$(get_object_files)

    # Compile test file and link with object files
    if ! clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$test_file" $obj_files -framework Cocoa -o "$output_binary" 2>&1; then
        echo -e "${RED}✗ COMPILATION FAILED: $test_name${RESET}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        echo ""
        return 1
    fi

    # Run
    if [ $DEBUG_MODE -eq 1 ]; then
        echo -e "${YELLOW}Running in lldb batch mode${RESET}"
        lldb -b -o "run" -o "bt" -o "quit" $output_binary
        # In debug mode, don't track pass/fail
        echo ""
        return 0
    fi

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
    # Compile libraries first
    if ! compile_libraries; then
        echo -e "${RED}Library compilation failed${RESET}"
        return 1
    fi

    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}RUNNING ALL TESTS${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo ""

    # Find all probatio_*.c files, separating GUI apps from regular tests
    local all_files
    if [ -n "$FILTER" ]; then
        all_files=$(find probatio probationes -name "probatio_*${FILTER}*.c" -type f 2>/dev/null | sort)
    else
        all_files=$(find probatio probationes -name "probatio_*.c" -type f 2>/dev/null | sort)
    fi

    if [ -z "$all_files" ]; then
        if [ -n "$FILTER" ]; then
            echo -e "${YELLOW}No test files found matching '$FILTER'${RESET}"
        else
            echo -e "${YELLOW}No test files found${RESET}"
        fi
        return 1
    fi

    # Separate GUI apps from regular tests
    local gui_apps=""
    local test_files=""

    while IFS= read -r file; do
        if [[ "$file" == *"probatio_fenestra.c"* ]] || [[ "$file" == *"probatio_delineare.c"* ]] || [[ "$file" == *"probatio_tempus.c"* ]] || [[ "$file" == *"probatio_pagina.c"* ]] || [[ "$file" == *"probatio_navigator.c"* ]] || [[ "$file" == *"probatio_combinado.c"* ]] || [[ "$file" == *"probatio_gradientum.c"* ]]; then
            gui_apps="$gui_apps$file"$'\n'
        else
            test_files="$test_files$file"$'\n'
        fi
    done <<< "$all_files"

    # Compile and run regular tests
    if [ -n "$test_files" ]; then
        while IFS= read -r test_file; do
            [ -z "$test_file" ] && continue
            compile_and_run_test "$test_file"
        done <<< "$test_files"
    fi

    # Build GUI apps (but don't run them)
    if [ -n "$gui_apps" ]; then
        echo -e "${BLUE}═══════════════════════════════════════${RESET}"
        echo -e "${BLUE}BUILDING GUI APPS${RESET}"
        echo -e "${BLUE}═══════════════════════════════════════${RESET}"
        echo ""

        while IFS= read -r gui_file; do
            [ -z "$gui_file" ] && continue
            compile_gui_app "$gui_file"
        done <<< "$gui_apps"
    fi
}

print_summary() {
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}SUMMARY${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"

    if [ $TESTS_TOTAL -gt 0 ]; then
        echo "Tests Total:  $TESTS_TOTAL"
        echo -e "Tests Passed: ${GREEN}$TESTS_PASSED${RESET}"
        echo -e "Tests Failed: ${RED}$TESTS_FAILED${RESET}"
    fi

    if [ $((GUI_APPS_BUILT + GUI_APPS_FAILED)) -gt 0 ]; then
        echo ""
        echo -e "GUI Apps Built: ${GREEN}$GUI_APPS_BUILT${RESET}"
        if [ $GUI_APPS_FAILED -gt 0 ]; then
            echo -e "GUI Apps Failed: ${RED}$GUI_APPS_FAILED${RESET}"
        fi

        if [ $GUI_APPS_BUILT -gt 0 ]; then
            echo -e "${YELLOW}Run GUI apps manually from bin/ directory${RESET}"
        fi
    fi

    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo ""
}

# Parse arguments
WATCH_MODE=0
DEBUG_MODE=0
CLEAN_MODE=0
for arg in "$@"; do
    if [ "$arg" == "--watch" ]; then
        WATCH_MODE=1
    elif [ "$arg" == "--debug" ]; then
        DEBUG_MODE=1
    elif [ "$arg" == "--clean" ]; then
        CLEAN_MODE=1
    else
        FILTER="$arg"
    fi
done

# Handle clean mode
if [ $CLEAN_MODE -eq 1 ]; then
    echo -e "${BLUE}Cleaning build directory...${RESET}"
    rm -rf "$BUILD_DIR"
    echo -e "${GREEN}Done${RESET}"
    exit 0
fi

# Add debug symbols if debug mode
if [ $DEBUG_MODE -eq 1 ]; then
    GCC_FLAGS+=("-g")
fi

# Main execution
if [ $WATCH_MODE -eq 1 ]; then
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
