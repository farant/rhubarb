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
    "-Wno-overlength-strings"
    "-fbracket-depth=512"
)

# Include paths
declare -a INCLUDE_FLAGS=(
    "-Iinclude"
    "-Iprobationes"
    "-Ibook_assets"
    "-Iprobationes/vitrea_assets"
    "-Iprobationes/tabella_assets"
)

# Source files to compile to object files
declare -a SOURCE_FILES=(
    "lib/piscina.c"
    "lib/chorda.c"
    "lib/credo.c"
    "lib/similitudo.c"
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
    "lib/color.c"
    "lib/thema.c"
    "lib/nuntium.c"
    "lib/persistentia_memoria.c"
    "lib/persistentia_nuntium.c"
    "lib/entitas_repositorium_impl.c"
    "lib/friatio.c"
    "lib/sigillum.c"
    "lib/vigilia.c"
    "lib/scrinium.c"
    "lib/xml.c"
    "lib/stml.c"
    "lib/selectio.c"
    "lib/widget.c"
    "lib/pagina.c"
    "lib/tabula_characterum.c"
    "lib/vim.c"
    "lib/navigator_entitatum.c"
    "lib/delineare.c"
    "lib/fenestra_textus.c"
    "lib/tempus.c"
    "lib/fasti.c"
    "lib/layout.c"
    "lib/registrum_commandi.c"
    "lib/registrum_widget.c"
    "lib/actor.c"
    "lib/tractator.c"
    "lib/cursus.c"
    "lib/tempestivum.c"
    "lib/servitium.c"
    "lib/sputnik_lexema.c"
    "lib/sputnik_parser.c"
    "lib/sputnik_interpres.c"
    "lib/coloratio.c"
    "lib/libro_paginarum.c"
    "lib/schirmata.c"
    "lib/arx_caeli.c"
    "lib/concha.c"
    "lib/cursor.c"
    "lib/thema_visus.c"
    "lib/sputnik_syntaxis.c"
    "lib/biblia_dr.c"
    "lib/biblia_visus.c"
    "lib/librarium_visus.c"
    "lib/librarium_lector.c"
    "lib/iter_directoria.c"
    "lib/exemplar.c"
    "lib/toml.c"
    "lib/json.c"
    "lib/internuntius.c"
    "lib/tabellarius.c"
    "lib/paginarium.c"
    "lib/utf8.c"
    "lib/fons.c"
    "lib/fons_visus.c"
    "lib/calendarium_liturgicum.c"
    "lib/sanctorale.c"
    "lib/calendario_visus.c"
    "lib/pinacotheca_visus.c"
    "lib/sententia_fissio.c"
    "lib/sententia_paginarium.c"
    "lib/tcp_posix.c"
    "lib/http.c"
    "lib/base64.c"
    "lib/url.c"
    "lib/multipart.c"
    "lib/uri.c"
    "lib/reactor.c"
    "lib/router.c"
    "lib/mimen.c"
    "lib/hospitium.c"
    "lib/speculum.c"
    "lib/flatura.c"
    "lib/quaerere.c"
    "lib/capsula.c"
    "lib/capsula_caudae.c"
    "lib/elementa.c"
    "lib/imago.c"
    "lib/imago_opus.c"
    "lib/dithering.c"
    "lib/dialogus.c"
    "lib/dialogus_importatio.c"
    "lib/importatio_visus.c"
    "lib/arbor_lexema.c"
    "lib/arbor_praeparator.c"
    "lib/arbor_syntaxis.c"
    "lib/arbor_typus.c"
    "lib/arbor_index.c"
    "lib/arbor_quaestio.c"
    "lib/arbor_formator.c"
    "lib/arbor.c"
    "lib/arbor2_lexema.c"
    "lib/arbor2_token.c"
    "lib/arbor2_expandere.c"
    "lib/arbor2_glr.c"
    "lib/arbor2_glr_tabula.c"
    "lib/arbor2_conditio_evaluare.c"
    "lib/arbor2_scribere.c"
    "lib/lapifex_generare.c"
    "lib/lapifex_parsere.c"
    "lib/lapifex_lexere.c"
    "lib/lapifex_glr.c"
    "lib/arbor2_lapifex_adaptator.c"
    "lib/lapifex_c89_grammatica.c"
    "lib/nuntium_schema.c"
    "lib/nuntium_schema_generare.c"
    "probationes/capsula_assets.c"
    "book_assets/capsula_libri.c"
    "probationes/vitrea_assets/capsula_templates.c"
    "probationes/tabella_assets/capsula_tabella.c"
    "probationes/hospes_assets/capsula_hospes.c"
)

# Objective-C sources (compiled separately)
declare -a OBJC_SOURCES=(
    "lib/fenestra_macos.m"
    "lib/tls_macos.m"
    "lib/clipboard_platform_macos.m"
    "lib/vitrea_macos.m"
)

# Vendored sources (compiled RELAXED in their own objects - treat
# like libc; tabularium-gradus.md phase A). Flags decided at vendor
# time: FTS5 ON (full-text search), THREADSAFE=0 (single-writer
# house law), DQS=0 (no double-quoted strings), no loadable
# extensions, no deprecated API.
declare -a VENDOR_SOURCES=(
    "vendor/sqlite3.c"
)
declare -a VENDOR_FLAGS=(
    "-O2"
    "-DSQLITE_ENABLE_FTS5"
    "-DSQLITE_THREADSAFE=0"
    "-DSQLITE_DQS=0"
    "-DSQLITE_DEFAULT_MEMSTATUS=0"
    "-DSQLITE_OMIT_LOAD_EXTENSION"
    "-DSQLITE_OMIT_DEPRECATED"
    "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"
)

# Apps quae fontem proprium portant (speculum): "titulus:plagula".
# Generatio in build/speculum/<titulus>/ - regeneratur per fabricam,
# numquam commissa (spec: project-specs/speculum-spec-v2.md).
declare -a SPECULUM_APPS=(
    "hospes:probationes/probatio_vitrea_hospes.c"
)

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
FAILED_TESTS=""

# Timing
START_TIME=0
TOTAL_START_TIME=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
TEST_TIMES_FILE=$(mktemp)

# GUI app results
GUI_APPS_BUILT=0
GUI_APPS_FAILED=0
FAILED_GUI_APPS=""

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

    # capita mutata sine recompilo = corruptio ABI (excubitor:
    # antea vexillum globale needs_compile solum accendebatur sed
    # condiciones per-plagulam capita ignorabant - vexillum
    # decorativum, nihil recompilabatur)
    newest_header () {
        find include -name '*.h' -newer "$1" 2>/dev/null | head -1
    }

    # Check if any source file is newer than its object file
    for src_file in "${SOURCE_FILES[@]}"; do
        obj_name=$(basename "$src_file" .c).o
        obj_file="$BUILD_DIR/$obj_name"

        if [ ! -f "$obj_file" ] || [ "$src_file" -nt "$obj_file" ]; then
            needs_compile=1
            break
        fi
    done

    # Also check Objective-C files
    for objc_file in "${OBJC_SOURCES[@]}"; do
        obj_name=$(basename "$objc_file" .m).o
        obj_file="$BUILD_DIR/$obj_name"
        if [ ! -f "$obj_file" ] || [ "$objc_file" -nt "$obj_file" ]; then
            needs_compile=1
            break
        fi
    done

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

        # Recompile if source OR any header is newer than object
        if [ ! -f "$obj_file" ] || [ "$src_file" -nt "$obj_file" ] || [ -n "$(newest_header "$obj_file")" ]; then
            echo -e "  Compiling: $src_file"
            if ! clang -c ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$src_file" -o "$obj_file" 2>&1; then
                echo -e "${RED}✗ FAILED: $src_file${RESET}"
                return 1
            fi
        fi
    done

    # Compile Objective-C files
    for objc_file in "${OBJC_SOURCES[@]}"; do
        obj_name=$(basename "$objc_file" .m).o
        obj_file="$BUILD_DIR/$obj_name"

        if [ ! -f "$obj_file" ] || [ "$objc_file" -nt "$obj_file" ] || [ -n "$(newest_header "$obj_file")" ]; then
            echo -e "  Compiling: $objc_file"
            if ! clang -c ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$objc_file" -o "$obj_file" 2>&1; then
                echo -e "${RED}✗ FAILED: $objc_file${RESET}"
                return 1
            fi
        fi
    done

    # Compile vendored files (relaxed flags, own objects; headers
    # in include/ do not affect them - source-newer check only)
    for vend_file in "${VENDOR_SOURCES[@]}"; do
        obj_name=$(basename "$vend_file" .c).o
        obj_file="$BUILD_DIR/$obj_name"

        if [ ! -f "$obj_file" ] || [ "$vend_file" -nt "$obj_file" ]; then
            echo -e "  Compiling (vendor): $vend_file"
            if ! clang -c ${VENDOR_FLAGS[@]} "$vend_file" -o "$obj_file" 2>&1; then
                echo -e "${RED}✗ FAILED: $vend_file${RESET}"
                return 1
            fi
        fi
    done

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

    # Add Objective-C objects
    for objc_file in "${OBJC_SOURCES[@]}"; do
        obj_name=$(basename "$objc_file" .m).o
        obj_files="$obj_files $BUILD_DIR/$obj_name"
    done

    # Add vendored objects
    for vend_file in "${VENDOR_SOURCES[@]}"; do
        obj_name=$(basename "$vend_file" .c).o
        obj_files="$obj_files $BUILD_DIR/$obj_name"
    done

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
    # -Wno-overlength-strings: GUI apps may have long STML layout strings
    if ! clang ${GCC_FLAGS[@]} -Wno-overlength-strings ${INCLUDE_FLAGS[@]} "$app_file" $obj_files -framework Cocoa -framework Security -framework WebKit -o "$output_binary" 2>&1; then
        echo -e "${RED}✗ BUILD FAILED: $app_name${RESET}"
        GUI_APPS_FAILED=$((GUI_APPS_FAILED + 1))
        FAILED_GUI_APPS="$FAILED_GUI_APPS $app_name"
        echo ""
        return 1
    fi

    echo -e "${GREEN}✓ GUI APP BUILT: $app_name (run with: ./$output_binary)${RESET}"
    GUI_APPS_BUILT=$((GUI_APPS_BUILT + 1))

    # Auto-append assets for capsula_caudae tests
    # Look for matching config: probationes/<name>_caudae_assets.toml
    local caudae_config="probationes/${app_name}_caudae_assets.toml"
    if [[ -f "$caudae_config" ]] && [[ -f "bin/capsula_caudae_adiungere" ]]; then
        echo -e "${YELLOW}Appending assets from $caudae_config${RESET}"
        local start_time=$(date +%s.%N)
        if ./bin/capsula_caudae_adiungere "$caudae_config" "$output_binary" 2>&1; then
            local end_time=$(date +%s.%N)
            local duration=$(echo "$end_time - $start_time" | bc)
            echo -e "${GREEN}✓ Assets appended (${duration}s)${RESET}"
            # Run the test after appending
            echo -e "${BLUE}Running $app_name...${RESET}"
            if $output_binary 2>&1; then
                echo -e "${GREEN}✓ TEST PASSED: $app_name${RESET}"
                TESTS_PASSED=$((TESTS_PASSED + 1))
                TESTS_TOTAL=$((TESTS_TOTAL + 1))
            else
                echo -e "${RED}✗ TEST FAILED: $app_name${RESET}"
                TESTS_FAILED=$((TESTS_FAILED + 1))
                TESTS_TOTAL=$((TESTS_TOTAL + 1))
                FAILED_TESTS="$FAILED_TESTS $app_name"
            fi
        else
            echo -e "${RED}✗ Failed to append assets${RESET}"
        fi
    fi

    echo ""
    return 0
}

compile_and_run_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .c)
    local output_binary="/tmp/$test_name"
    local obj_files
    local test_start_time
    local test_end_time
    local test_duration

    TESTS_TOTAL=$((TESTS_TOTAL + 1))

    echo -e "${BLUE}────────────────────────────────────────${RESET}"
    echo -e "${BLUE}Testing: $test_name${RESET}"
    echo -e "${BLUE}────────────────────────────────────────${RESET}"

    obj_files=$(get_object_files)

    # Obiecta extra per-probatione (capsulae speculi non globales)
    case "$test_name" in
        probatio_speculum_fontium|probatio_speculum)
            obj_files="$obj_files build/speculum/hospes/capsula_speculi_hospes.o"
            ;;
    esac

    # Compile test file and link with object files
    if ! clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$test_file" $obj_files -framework Cocoa -framework Security -framework WebKit -o "$output_binary" 2>&1; then
        echo -e "${RED}✗ COMPILATION FAILED: $test_name${RESET}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        FAILED_TESTS="$FAILED_TESTS $test_name"
        echo ""
        return 1
    fi

    # Run with timing
    test_start_time=$(perl -MTime::HiRes -e 'print Time::HiRes::time')

    if [ $DEBUG_MODE -eq 1 ]; then
        echo -e "${YELLOW}Running in lldb batch mode${RESET}"
        lldb -b -o "run" -o "bt" -o "quit" $output_binary
        # In debug mode, don't track pass/fail
        echo ""
        return 0
    fi

    if ! $output_binary 2>&1; then
        test_end_time=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
        test_duration=$(echo "$test_end_time - $test_start_time" | bc)
        echo "$test_duration $test_name" >> "$TEST_TIMES_FILE"
        echo -e "${RED}✗ TEST FAILED: $test_name ${YELLOW}(${test_duration}s)${RESET}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        FAILED_TESTS="$FAILED_TESTS $test_name"
        echo ""
        return 1
    fi

    test_end_time=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    test_duration=$(echo "$test_end_time - $test_start_time" | bc)
    echo "$test_duration $test_name" >> "$TEST_TIMES_FILE"
    echo -e "${GREEN}✓ TEST PASSED: $test_name ${YELLOW}(${test_duration}s)${RESET}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
    echo ""
    return 0
}

compile_tool_if_needed() {
    local tool_source="$1"
    local tool_binary="$2"
    local obj_files

    if [ ! -f "$tool_source" ]; then
        return 0
    fi

    if [ -f "$tool_binary" ] && [ "$tool_binary" -nt "$tool_source" ]; then
        return 0
    fi

    mkdir -p bin
    obj_files=$(get_object_files)

    echo -e "  Compiling tool: $tool_source"
    if ! clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$tool_source" $obj_files -framework Cocoa -framework Security -framework WebKit -o "$tool_binary" 2>&1; then
        echo -e "${RED}✗ FAILED: $tool_source${RESET}"
        return 1
    fi
    return 0
}

run_generare() {
    local gen_files
    local gen_file

    # Construere instrumenta si opus est
    compile_tool_if_needed "tools/generare.c" "bin/generare" || return 1
    compile_tool_if_needed "instrumenta/nuntium_schema_generare.c" "bin/nuntium_schema_generare" || return 1

    if [ ! -f "bin/generare" ]; then
        return 0
    fi

    # Invenire fila probationum quae compilabuntur
    if [ -n "$FILTER" ]; then
        gen_files=$(find probatio probationes -name "probatio_*${FILTER}*.c" -type f 2>/dev/null | sort)
    else
        gen_files=$(find probatio probationes -name "probatio_*.c" -type f 2>/dev/null | sort)
    fi

    if [ -z "$gen_files" ]; then
        return 0
    fi

    # Currere generare solum in filis quae compilabuntur
    echo -e "${BLUE}Running generare on test files${RESET}"
    while IFS= read -r gen_file; do
        [ -z "$gen_file" ] && continue
        # Saltare probatio_generare.c — continet directivas probationis in literis
        if [[ "$gen_file" == *"probatio_generare.c" ]]; then
            continue
        fi
        if ! ./bin/generare "$gen_file" 2>&1; then
            echo -e "${RED}✗ generare failed for $gen_file${RESET}"
            return 1
        fi
    done <<< "$gen_files"
    echo ""

    return 0
}

run_speculum() {
    local entry
    local titulus
    local app_file
    local app_name
    local stage
    local gen_c
    local gen_o

    compile_tool_if_needed "tools/capsula_generare.c" "bin/capsula_generare" || return 1

    for entry in "${SPECULUM_APPS[@]}"; do
        titulus="${entry%%:*}"
        app_file="${entry#*:}"
        app_name=$(basename "$app_file" .c)

        # Genera solum si app aut probatio speculi in ludo est
        if [ -n "$FILTER" ]; then
            if [[ "$app_name" != *"$FILTER"* ]] && [[ "probatio_speculum_fontium" != *"$FILTER"* ]]; then
                continue
            fi
        fi

        if ! SPECULUM_VEXILLA="${GCC_FLAGS[*]}" SPECULUM_INCLUSA="${INCLUDE_FLAGS[*]}" \
            tools/speculum_generare.sh "$titulus" "$app_file" "compile_tests.sh" \
            -- "${SOURCE_FILES[@]}" -- "${OBJC_SOURCES[@]}" -- "${VENDOR_SOURCES[@]}"; then
            echo -e "${RED}✗ speculum generatio fracta: $titulus${RESET}"
            return 1
        fi

        # Compila capsulam generatam (obiectum in statione, non in
        # build/ radice - nexus explicitus per-consumptore)
        stage="build/speculum/$titulus"
        gen_c="$stage/capsula_speculi_${titulus}.c"
        gen_o="$stage/capsula_speculi_${titulus}.o"
        if [ ! -f "$gen_o" ] || [ "$gen_c" -nt "$gen_o" ]; then
            echo -e "  Compiling (speculum): $gen_c"
            if ! clang -c ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$gen_c" -o "$gen_o" 2>&1; then
                echo -e "${RED}✗ FAILED: $gen_c${RESET}"
                return 1
            fi
        fi
    done

    # Probatio velaminis JS sine navigatro (JavaScriptCore/JXA) -
    # porta per codicem exitus (iactus in plagula -> non-zephyrum)
    if [ -f "lib/speculum_assets/probatio_velaminis.js" ]; then
        if ! osascript -l JavaScript lib/speculum_assets/probatio_velaminis.js > /dev/null 2>&1; then
            echo -e "${RED}✗ speculum velamen probatio fracta (curre: osascript -l JavaScript lib/speculum_assets/probatio_velaminis.js)${RESET}"
            return 1
        fi
        echo -e "${BLUE}speculum velamen: probatio JS bona${RESET}"
    fi
    return 0
}

run_all_tests() {
    # Compile libraries first
    if ! compile_libraries; then
        echo -e "${RED}Library compilation failed${RESET}"
        return 1
    fi

    # Run generare directives before compiling tests
    if ! run_generare; then
        echo -e "${RED}generare step failed${RESET}"
        return 1
    fi

    # Genera capsulas speculi (fontes embedded) ante probationes
    if ! run_speculum; then
        echo -e "${RED}speculum step failed${RESET}"
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
    # Also skip benchmark files (run via run_benchmark.sh)
    local gui_apps=""
    local test_files=""

    while IFS= read -r file; do
        # Skip benchmark files - run separately via run_benchmark.sh
        if [[ "$file" == *"_benchmark.c"* ]]; then
            continue
        # Named live-internet exclusions (TLS is client-only - a local
        # fixture cannot serve it). Run on demand:
        #   ./compile_tests.sh -reticularis     (full suite + live tests)
        #   ./compile_tests.sh tls              (explicit filter also runs)
        elif [[ -z "$FILTER" && $RETICULARIS -eq 0 ]] && { [[ "$file" == *"probatio_tls.c"* ]] || [[ "$file" == *"probatio_tcp.c"* ]]; }; then
            echo -e "${YELLOW}RETICULARIS EXCLUSUM: $(basename "$file" .c) (curre: ./compile_tests.sh -reticularis)${RESET}"
            continue
        elif [[ "$file" == *"probatio_fenestra.c"* ]] || [[ "$file" == *"probatio_vitrea.c"* ]] || [[ "$file" == *"probatio_vitrea_tabella.c"* ]] || [[ "$file" == *"probatio_vitrea_hospes.c"* ]] || [[ "$file" == *"probatio_delineare.c"* ]] || [[ "$file" == *"probatio_tempus.c"* ]] || [[ "$file" == *"probatio_pagina.c"* ]] || [[ "$file" == *"probatio_navigator.c"* ]] || [[ "$file" == *"probatio_combinado.c"* ]] || [[ "$file" == *"probatio_gradientum.c"* ]] || [[ "$file" == *"probatio_capsula_caudae.c"* ]] || [[ "$file" == *"probatio_elementa.c"* ]] || [[ "$file" == *"probatio_imago.c"* ]] || [[ "$file" == *"probatio_dithering.c"* ]]; then
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
    local total_end_time
    local total_duration

    total_end_time=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    total_duration=$(echo "$total_end_time - $TOTAL_START_TIME" | bc)

    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo -e "${BLUE}SUMMARY${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"

    if [ $TESTS_TOTAL -gt 0 ]; then
        echo "Tests Total:  $TESTS_TOTAL"
        echo -e "Tests Passed: ${GREEN}$TESTS_PASSED${RESET}"
        echo -e "Tests Failed: ${RED}$TESTS_FAILED${RESET}"
        if [ -n "$FAILED_TESTS" ]; then
            echo -e "${RED}Failed:$FAILED_TESTS${RESET}"
        fi
    fi

    if [ $((GUI_APPS_BUILT + GUI_APPS_FAILED)) -gt 0 ]; then
        echo ""
        echo -e "GUI Apps Built: ${GREEN}$GUI_APPS_BUILT${RESET}"
        if [ $GUI_APPS_FAILED -gt 0 ]; then
            echo -e "GUI Apps Failed: ${RED}$GUI_APPS_FAILED${RESET}"
            echo -e "${RED}Failed:$FAILED_GUI_APPS${RESET}"
        fi

        if [ $GUI_APPS_BUILT -gt 0 ]; then
            echo -e "${YELLOW}Run GUI apps manually from bin/ directory${RESET}"
        fi
    fi

    # Show slowest tests (up to 5)
    if [ -s "$TEST_TIMES_FILE" ]; then
        echo ""
        echo -e "${BLUE}Slowest tests:${RESET}"
        sort -rn "$TEST_TIMES_FILE" | head -5 | while read duration name; do
            printf "  ${YELLOW}%8ss${RESET}  %s\n" "$duration" "$name"
        done
    fi

    echo ""
    echo -e "Total Time:   ${YELLOW}${total_duration}s${RESET}"
    echo -e "${BLUE}═══════════════════════════════════════${RESET}"
    echo ""

    # Cleanup temp file
    rm -f "$TEST_TIMES_FILE"
}

# Parse arguments
WATCH_MODE=0
DEBUG_MODE=0
CLEAN_MODE=0
RETICULARIS=0
for arg in "$@"; do
    if [ "$arg" == "--watch" ]; then
        WATCH_MODE=1
    elif [ "$arg" == "--debug" ]; then
        DEBUG_MODE=1
    elif [ "$arg" == "--clean" ]; then
        CLEAN_MODE=1
    elif [ "$arg" == "-reticularis" ] || [ "$arg" == "--reticularis" ]; then
        RETICULARIS=1
    else
        FILTER="$arg"
    fi
done

# Handle clean mode - clean build and bin directories, then continue to rebuild
if [ $CLEAN_MODE -eq 1 ]; then
    echo -e "${BLUE}Cleaning build and bin directories...${RESET}"
    rm -rf "$BUILD_DIR"/*
    rm -rf bin/*
    echo -e "${GREEN}Cleaned${RESET}"
    echo ""
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
        # canalis excubitoris: verdictum post-constructionem
        # (tacet nisi stala)
        ./excubitor.sh -tacitus "build/" >&2 || true
        exit 0
    else
        exit 1
    fi
fi
