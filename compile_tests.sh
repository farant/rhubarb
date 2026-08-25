#!/bin/bash

# Effusio cursus ULTIMI semper in plagulam scripta (stdout +
# stderr ambo) - visibilis etiam si fistulam paravimus male.
# Involucrum se-ipsum-exsequens: cursus verus filius per tee
# fluit, ergo plagula INTEGRA est cum promptum redit (nulla
# certatio effusionis), et codex exitus per PIPESTATUS servatur
# (contractus 0/1/2 intactus: 2 = NIHIL cucurrit). Invocatio
# quaevis plagulam obruit - cursus ultimus vincit. Schema:
# build/test_logs/<cursor>.log, cursor quisque suam (radix,
# silva, officina, gesta, saltuarius, tessera, linux).
if [ -z "${COMPILE_TESTS_EFFUSIO:-}" ]; then
    export COMPILE_TESTS_EFFUSIO=1
    mkdir -p "$(dirname "$0")/build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/build/test_logs/radix.log"
    exit "${PIPESTATUS[0]}"
fi

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

# Fontes obiectorum (SOURCE_FILES + OBJC_SOURCES): GENERATI ab
# aedile - unio clausurarum omnium principalium suite. Via media:
# structura necte-omnia intacta, listae solae derivatae.
# Regeneratio: ./tools/compile_tests_fontes_generare.sh
source "$(dirname "$0")/compile_tests_fontes_generata.sh"

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
COMPILE_TIMES_FILE=$(mktemp)

# ==================================================
# MENSURA (metra) - OPTIONALIA, suitam numquam frangunt
# ==================================================
# Porta probationum verdictum suum servat: instrumentum metiens quod
# suitam frangere posset peius est quam nullum. Ergo omnis vocatio
# mensoris '|| true' fert et defectus tacet.
# MENSOR_TACET=1 metra omnino claudit.
MENSOR=""
if [ -z "${MENSOR_TACET:-}" ] && [ -x "./bin/mensor" ]; then
    if eval "$(./bin/mensor sessio 2>/dev/null)" 2>/dev/null; then
        MENSOR="./bin/mensor"
    fi
fi

# mensura_addere <titulus> <valor> <unitas> [parens]
mensura_addere() {
    [ -n "$MENSOR" ] || return 0
    if [ -n "${4:-}" ]; then
        "$MENSOR" addere -titulus "$1" -valor "$2" -unitas "$3" \
            -parens "$4" >/dev/null 2>&1 || true
    else
        "$MENSOR" addere -titulus "$1" -valor "$2" -unitas "$3" \
            >/dev/null 2>&1 || true
    fi
}

# mensura_nodus <titulus> <valor> <unitas> [parens] -> clavem imprimit
# (nodus qui filias habebit; clavis eius parens earum fit)
mensura_nodus() {
    [ -n "$MENSOR" ] || { echo ""; return 0; }
    if [ -n "${4:-}" ]; then
        "$MENSOR" addere -titulus "$1" -valor "$2" -unitas "$3" \
            -parens "$4" -id 2>/dev/null || echo ""
    else
        "$MENSOR" addere -titulus "$1" -valor "$2" -unitas "$3" \
            -id 2>/dev/null || echo ""
    fi
}

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

    # AMALGAMATA COMMISSA: adhuc compilant? (< I sec.)
    # Amalgamata artificia GENERATA et COMMISSA sunt quae suite
    # ordinaria numquam tangebat - unde per menses rumpi poterant
    # tacite. Haec porta putredinem ARTIFICII capit (corruptio
    # manualis, fructus fractus commissus, caput manu scriptum a
    # corpore divergens). Semitam REGENERATIONIS NON capit: vide
    # tools/amalgamata_probare.sh pro discrimine, quod momenti est.
    if [ -x tools/amalgamata_probare.sh ]; then
        if ! tools/amalgamata_probare.sh > "$BUILD_DIR/amalgamata.txt" 2>&1; then
            echo -e "${RED}AMALGAMATA FRACTA:${RESET}"
            head -12 "$BUILD_DIR/amalgamata.txt"
            exit 1
        fi
    fi

    # capita mutata sine recompilo = corruptio ABI (excubitor:
    # antea vexillum globale needs_compile solum accendebatur sed
    # condiciones per-plagulam capita ignorabant - vexillum
    # decorativum, nihil recompilabatur)
    #
    # CAPUT RECENTISSIMUM SEMEL (mensura 2026-08-25): ansa
    # nidificata prior (150 capita x 130 fontes cum $(basename)
    # FURCA per iterationem = ~20k furcae) ~30s cursu QUOVIS
    # solvebat, etiam calido. Aequivalentia: aliquod caput
    # recentius obiecto <=> caput RECENTISSIMUM recentius obiecto.
    CAPUT_RECENS=""
    for header in include/*.h; do
        if [ -z "$CAPUT_RECENS" ] || [ "$header" -nt "$CAPUT_RECENS" ]; then
            CAPUT_RECENS="$header"
        fi
    done

    newest_header () {
        if [ -n "$CAPUT_RECENS" ] && [ "$CAPUT_RECENS" -nt "$1" ]; then
            echo "$CAPUT_RECENS"
        fi
    }

    # Check if any source file is newer than its object file
    # (expansio parametrorum, non $(basename) - furca per fontem)
    for src_file in "${SOURCE_FILES[@]}"; do
        obj_name="${src_file##*/}"
        obj_file="$BUILD_DIR/${obj_name%.c}.o"

        if [ ! -f "$obj_file" ] || [ "$src_file" -nt "$obj_file" ]; then
            needs_compile=1
            break
        fi
    done

    # Also check Objective-C files
    for objc_file in "${OBJC_SOURCES[@]}"; do
        obj_name="${objc_file##*/}"
        obj_file="$BUILD_DIR/${obj_name%.m}.o"
        if [ ! -f "$obj_file" ] || [ "$objc_file" -nt "$obj_file" ]; then
            needs_compile=1
            break
        fi
    done

    # Check if any header changed (una transitio contra caput
    # recentissimum - ansa nidificata retirata)
    if [ $needs_compile -eq 0 ] && [ -n "$CAPUT_RECENS" ]; then
        for src_file in "${SOURCE_FILES[@]}"; do
            obj_name="${src_file##*/}"
            obj_file="$BUILD_DIR/${obj_name%.c}.o"
            if [ -f "$obj_file" ] && [ "$CAPUT_RECENS" -nt "$obj_file" ]; then
                needs_compile=1
                break
            fi
        done
    fi

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

    # Obiecta extra per-app (capsulae speculi non globales)
    case "$app_name" in
        probatio_vitrea_hospes)
            obj_files="$obj_files build/speculum/hospes/capsula_speculi_hospes.o"
            ;;
    esac

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

    # Compile test file and link with object files.
    # COMPILATIO SEORSUM METITUR: test_start_time infra POST
    # compilationem ponitur, ergo sine hoc tempus compilandi -
    # pars maxima suitae - omnino invisibile manet.
    local compilatio_initium
    local compilatio_finis
    compilatio_initium=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    if ! clang ${GCC_FLAGS[@]} ${INCLUDE_FLAGS[@]} "$test_file" $obj_files -framework Cocoa -framework Security -framework WebKit -o "$output_binary" 2>&1; then
        echo -e "${RED}✗ COMPILATION FAILED: $test_name${RESET}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        FAILED_TESTS="$FAILED_TESTS $test_name"
        echo ""
        return 1
    fi
    compilatio_finis=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    echo "$(echo "$compilatio_finis - $compilatio_initium" | bc) $test_name" \
        >> "$COMPILE_TIMES_FILE"

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

        if ! SPECULUM_VEXILLA="${GCC_FLAGS[*]}" \
            tools/speculum_generare.sh "$titulus" "$app_file" "compile_tests.sh"; then
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

    # SYNTAXIS JS - porta ANTE probationes velaminum.
    #
    # CUR OMNINO: haec domus JS scribit quem NULLUS compilator legit -
    # assets velaminum hic, et manus eum ex chordis C concatenat. BIS
    # in duobus diebus vitium syntaxis transiit (manus_focus_ponere:
    # '}' intra litteras JS clausum, ergo obiectum numquam clausum;
    # experimentum laboratorii 0003: paren impar a renominatione), et
    # UTRUMQUE sola applicatio viva cepit. Probationes quae in
    # CONTENTO JS asserta faciunt vitium syntaxis capere
    # structuraliter non possunt.
    #
    # ANTE velamina consulto: plagula prava eas quoque frangit, sed
    # cum nuntio obscuro ('probatio fracta'). Hic causa NOMINATUR.
    if [ -x "./tools/js_syntaxis.sh" ]; then
        js_plagulae=$(find lib tools apps probationes \
            -name '*.js' -not -path '*/node_modules/*' 2>/dev/null \
            | grep -v '^tools/js_syntaxis.js$' | sort)
        if [ -n "$js_plagulae" ]; then
            if ! ./tools/js_syntaxis.sh -tacite $js_plagulae; then
                echo -e "${RED}✗ syntaxis JS prava (curre:"
                echo -e "  ./tools/js_syntaxis.sh <plagula.js>)${RESET}"
                return 1
            fi
            echo -e "${BLUE}syntaxis JS: omnes plagulae validae${RESET}"
        fi
    fi

    # Probatio velaminis JS sine navigatro (JavaScriptCore/JXA) -
    # porta per codicem exitus (iactus in plagula -> non-zephyrum)
    if [ -f "lib/speculum_assets/probatio_velaminis.js" ]; then
        if ! osascript -l JavaScript lib/speculum_assets/probatio_velaminis.js > /dev/null 2>&1; then
            echo -e "${RED}✗ speculum velamen probatio fracta (curre: osascript -l JavaScript lib/speculum_assets/probatio_velaminis.js)${RESET}"
            return 1
        fi
        echo -e "${BLUE}speculum velamen: probatio JS bona${RESET}"
    fi

    # Velamen fori: logica quae in C non stat (derivatio tituli,
    # aequatio ADDITIVA definitionum viventium, cribrum indicis).
    # Aequatio tacite fallit - optiones non auctae = species nova a
    # machina recusata, et velamen causam non monstrat.
    if [ -f "apps/forum/assets/probatio_fori.js" ]; then
        if ! osascript -l JavaScript apps/forum/assets/probatio_fori.js > /dev/null 2>&1; then
            echo -e "${RED}✗ forum velamen probatio fracta (curre: osascript -l JavaScript apps/forum/assets/probatio_fori.js)${RESET}"
            return 1
        fi
        echo -e "${BLUE}forum velamen: probatio JS bona${RESET}"
    fi

    # Logica mensae: arbiter gestuum (tene/trahe/duplex) + plica JS
    # (speculum plicae C) + filtrum tabularum + derivatio retro -
    # logica.js SINE DOM consulto, ergo probatio synchrona simplex.
    if [ -f "lib/mensa_assets/probatio_mensae.js" ]; then
        if ! osascript -l JavaScript lib/mensa_assets/probatio_mensae.js > /dev/null 2>&1; then
            echo -e "${RED}✗ mensa logica probatio fracta (curre: osascript -l JavaScript lib/mensa_assets/probatio_mensae.js)${RESET}"
            return 1
        fi
        echo -e "${BLUE}mensa logica: probatio JS bona${RESET}"
    fi

    # Oraculum gyri QR: qr.c scribit, CoreImage (decodator alienus)
    # legit. probatio_qr.c FORMAM probat; bits solum hoc probat -
    # bibliotheca V4+ fracta LXVI casus structurales praeteribat.
    if [ -x "probationes/probatio_qr_gyrus.sh" ]; then
        if ! ./probationes/probatio_qr_gyrus.sh > /dev/null 2>&1; then
            echo -e "${RED}✗ qr gyrus fractus (curre: ./probationes/probatio_qr_gyrus.sh)${RESET}"
            return 1
        fi
        echo -e "${BLUE}qr gyrus: codices legibiles (oraculum CoreImage)${RESET}"
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
    local titulus

    total_end_time=$(perl -MTime::HiRes -e 'print Time::HiRes::time')
    total_duration=$(echo "$total_end_time - $TOTAL_START_TIME" | bc)

    # Compendium machinae-legibile in fistulam III (-machina).
    # Schema STABILE est: claves omnes semper emittuntur, etiam
    # zephyrae, ne lector 'clavis abest' ab 'valor zephyrus'
    # discernere cogatur.
    #
    # VERDICTUM codici exitus CONSENTIT consulto (probationes solae,
    # non applicationes) - compendium quod portae dissentiret vitium
    # novum esset, non sartura. Aedificatio applicationis fracta
    # suitam NON frangit (mos praecedens, hic non mutatus); qui
    # severior esse vult APPLICATIONES_FRACTAE legat.
    if [ $MACHINA -eq 1 ]; then
        printf 'TOTALIS\t%s\n'    "$TESTS_TOTAL"  >&3
        printf 'PRAETERITI\t%s\n' "$TESTS_PASSED" >&3
        printf 'FRACTI\t%s\n'     "$TESTS_FAILED" >&3
        for titulus in $FAILED_TESTS; do
            printf 'FRACTA\t%s\n' "$titulus" >&3
        done
        printf 'APPLICATIONES\t%s\n'         "$GUI_APPS_BUILT"  >&3
        printf 'APPLICATIONES_FRACTAE\t%s\n' "$GUI_APPS_FAILED" >&3
        for titulus in $FAILED_GUI_APPS; do
            printf 'APPLICATIO_FRACTA\t%s\n' "$titulus" >&3
        done
        printf 'TEMPUS\t%s\n' "$total_duration" >&3
        if [ $TESTS_TOTAL -eq 0 ]; then
            printf 'VERDICTUM\tNULLUS\n' >&3
        elif [ $TESTS_FAILED -eq 0 ]; then
            printf 'VERDICTUM\tPLENUS\n' >&3
        else
            printf 'VERDICTUM\tFRACTUS\n' >&3
        fi
    fi

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

    # ==================================================
    # MENSURAE
    # ==================================================
    # POST compendium visibile: metra nihil morantur quod homo videt,
    # et si mensor haeret, numeri iam impressi sunt.
    if [ -n "$MENSOR" ]; then
        local summa_cursus
        local summa_compilationis

        summa_cursus=$(awk '{s+=$1} END {printf "%.6f", s+0}' \
            "$TEST_TIMES_FILE" 2>/dev/null || echo 0)
        summa_compilationis=$(awk '{s+=$1} END {printf "%.6f", s+0}' \
            "$COMPILE_TIMES_FILE" 2>/dev/null || echo 0)

        # Aggregata. compilatio et cursus SEORSUM: sine ea divisione
        # 'tempus totum' fere solum narrat an arca aedificationis
        # calida fuerit - strepitus bimodalis cum signo intus sepulto.
        local radix
        local nodus_compilationis
        local nodus_cursus
        local praevolatus

        # ARBOR, non tabula plana: totum radix est, phases rami,
        # probationes singulae folia. Ita pictura flammae ostendere
        # potest UBI tempus abierit, non solum quantum fuerit.
        radix=$(mensura_nodus "suita.tempus.totum" "$total_duration" secunda)
        nodus_compilationis=$(mensura_nodus "suita.tempus.compilatio" \
            "$summa_compilationis" secunda "$radix")
        nodus_cursus=$(mensura_nodus "suita.tempus.cursus" \
            "$summa_cursus" secunda "$radix")

        # PRAEVOLATUS = quod superest. Mensuratum 2026-08-13: LXVI
        # secunda ex CLXXX - probationes JS/oraculi et generare, quae
        # OMNI cursu currunt, filtro neglecto. Residuum nominare id
        # ex 'ignoto' in 'mensuratum' vertit.
        praevolatus=$(echo \
            "$total_duration - $summa_compilationis - $summa_cursus" \
            | bc 2>/dev/null || echo 0)
        mensura_addere "suita.tempus.praevolatus" "$praevolatus" \
            secunda "$radix"

        mensura_addere "suita.probationes.totae"  "$TESTS_TOTAL" \
            numerus "$radix"
        mensura_addere "suita.probationes.fractae" "$TESTS_FAILED" \
            numerus "$radix"
        # LIBS_COMPILED = an bibliothecae hoc cursu recompilatae sint:
        # frigidus contra calidum, discrimen quod tempus totum duplicat
        mensura_addere "suita.bibliothecae.recompilatae" \
            "$LIBS_COMPILED" veritas "$radix"

        # Fasces per-probationem sub nodo phasis suae: UNUS processus
        # pro omnibus (CXXXI probationes => CCLXII processus aliter)
        "$MENSOR" addere -tabula "$TEST_TIMES_FILE" \
            -praefixum "probatio.cursus." -unitas secunda \
            -parens "$nodus_cursus" >/dev/null 2>&1 || true
        "$MENSOR" addere -tabula "$COMPILE_TIMES_FILE" \
            -praefixum "probatio.compilatio." -unitas secunda \
            -parens "$nodus_compilationis" >/dev/null 2>&1 || true

        "$MENSOR" condere >/dev/null 2>&1 || true
    fi

    # Cleanup temp files
    rm -f "$TEST_TIMES_FILE" "$COMPILE_TIMES_FILE"
}

# Parse arguments
WATCH_MODE=0
DEBUG_MODE=0
CLEAN_MODE=0
RETICULARIS=0
MACHINA=0
for arg in "$@"; do
    if [ "$arg" == "--watch" ]; then
        WATCH_MODE=1
    elif [ "$arg" == "--debug" ]; then
        DEBUG_MODE=1
    elif [ "$arg" == "--clean" ]; then
        CLEAN_MODE=1
    elif [ "$arg" == "-reticularis" ] || [ "$arg" == "--reticularis" ]; then
        RETICULARIS=1
    elif [ "$arg" == "-machina" ] || [ "$arg" == "--machina" ]; then
        MACHINA=1
    else
        FILTER="$arg"
    fi
done

# NORMALIZATIO FILTRI
#
# Filtrum in formam 'probatio_*${FILTER}*.c' inseritur, ergo
# './compile_tests.sh probatio_canon' in
# 'probatio_*probatio_canon*.c' vertitur et NIHIL invenit -
# exitus II, recte sed obscure. Nomen quod operans in effusione
# VIDET ('probatio_canon', 'Failed: probatio_canon') est ipsum
# nomen quod re-scribere non potest: instrumentum nomen suum
# proprium respuit.
#
# Formae quas homo revera scribit idem significant:
#   canon                             (nomen suitae)
#   probatio_canon                    (nomen ut effusum)
#   probationes/probatio_canon.c      (via, e completione TAB)
# Omnes ad 'canon' reducuntur.
#
# CLAMAT, non tacet: transformatio tacita filtri est id ipsum
# genus quod domus vetat ('successum nuntiare sine ope facto').
# Operans videat quid re vera quaesitum sit.
#
# Filtrum in NIHIL cadens ('probatio_' solum) SERVATUR: vacuum
# 'omnes probationes' significat, et sweep totam ex mendo typico
# incipere multo peius est quam exitus II.
if [ -n "$FILTER" ]; then
    FILTRUM_DATUM="$FILTER"
    FILTRUM_NOVUM="${FILTER##*/}"        # viam demere
    FILTRUM_NOVUM="${FILTRUM_NOVUM%.c}"  # extensionem demere
    FILTRUM_NOVUM="${FILTRUM_NOVUM#probatio_}"
    if [ -z "$FILTRUM_NOVUM" ]; then
        if [ $MACHINA -eq 0 ]; then
            echo "compile_tests: filtrum '$FILTRUM_DATUM' ad nihil" \
                 "reducitur - servatum ut est (vacuum omnes" \
                 "probationes vocaret)" >&2
        fi
    else
        FILTER="$FILTRUM_NOVUM"
        if [ "$FILTER" != "$FILTRUM_DATUM" ] && [ $MACHINA -eq 0 ]; then
            echo "compile_tests: filtrum '$FILTRUM_DATUM' ->" \
                 "'$FILTER'" >&2
        fi
    fi
fi

# Fistula III = canalis MACHINAE, seiunctus a canali humano.
#
# Cur canalis seiunctus et non sola linea plana in fine: vitium
# verum non erat "compendium colores fert" sed "FLUMEN colores
# fert". grep octetos ANSI videt, plagulam BINARIAM iudicat, et
# 'binary file matches' reddit sine linea ulla - etiam si linea
# petita ipsa munda est. sed eodem flumine 'illegal byte sequence'
# cadit. Ergo compendium mundum in fine NIHIL sanavisset; effusio
# humana TOTA silere debet.
#
# Forma: TSV, clavis MAIUSCULA in columna prima - eadem qua
# examen -machina utitur (exemplar domi iam erat).
if [ $MACHINA -eq 1 ]; then
    exec 3>&1 1>/dev/null 2>/dev/null
else
    exec 3>/dev/null
fi

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

    # NULLA probatio cursa = error operantis (filtrum male scriptum),
    # NON successus. Antea hoc exitum 0 reddebat, ergo
    # './compile_tests.sh probatio_sentntiae' (litera perdita) portam
    # VIRIDEM ostendebat sine probatione ulla cursa - idem genus ac
    # 'transformatio successum nuntiat sine ope facto'. Codex II
    # 'nihil cursum' a codice I 'aliquid fractum' discernit.
    #
    # Vocator unicus qui casum-nullum consulto adhibet
    # (compile_tools.sh:65, './compile_tests.sh --libs-only') exitum
    # NON custodit - obiecta ipsa postea probat (linea 74). Tutum.
    if [ $TESTS_TOTAL -eq 0 ]; then
        exit 2
    elif [ $TESTS_FAILED -eq 0 ]; then
        # canalis excubitoris: verdictum post-constructionem
        # (tacet nisi stala)
        ./excubitor.sh -tacitus "build/" >&2 || true
        exit 0
    else
        exit 1
    fi
fi
