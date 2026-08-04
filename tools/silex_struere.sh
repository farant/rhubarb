#!/bin/bash

# tools/silex_struere.sh - bin/silex aedificare
#
# Cur non compile_tools.sh: silex capsulam frontis fert
# (tools/silex_assets/capsula_silex_frons.c) quam ille nesciret.
# Obiecta bibliothecarum e build/ (suite ea servat recentia);
# si build/ vacuum, suite prima currenda.
#
# Usus: ./tools/silex_struere.sh

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long"
)

if [ -z "$(ls -A build/*.o 2>/dev/null)" ]; then
    echo "silex_struere: build/ vacuum - suite prima curre" \
        "(./compile_tests.sh)" >&2
    exit 1
fi

# mensa.js glomerandum si fontes recentiores (ordo = ordo oneris:
# thema ante scidam, scida ante scidulam quae eam extendit)
MENSA_FONTES=(
    lib/mensa_assets/fontes/thema.js
    lib/mensa_assets/fontes/scida.js
    lib/mensa_assets/fontes/scidula.js
    lib/mensa_assets/fontes/planum.js
    lib/mensa_assets/fontes/persistentia.js
)
MENSA_EXITUS=tools/silex_assets/mensa.js
for fons in "${MENSA_FONTES[@]}"; do
    if [ ! -f "$MENSA_EXITUS" ] || [ "$fons" -nt "$MENSA_EXITUS" ]; then
        ./tools/glomerare.sh "$MENSA_EXITUS" "${MENSA_FONTES[@]}" \
            || exit 1
        break
    fi
done

# capsula frontis regeneranda si assetum quodvis recentior
if [ tools/silex_assets/index.html -nt \
     tools/silex_assets/capsula_silex_frons.c ] || \
   [ "$MENSA_EXITUS" -nt \
     tools/silex_assets/capsula_silex_frons.c ]; then
    echo "  [capsula] silex_frons (assetum recentior)"
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || exit 1
    fi
    ./bin/capsula_generare tools/silex_assets/silex.toml || exit 1
fi

mkdir -p bin
echo "  [silex] tools/silex.c + capsula_silex_frons.c"
clang "${GCC_FLAGS[@]}" -Iinclude \
    tools/silex.c tools/silex_assets/capsula_silex_frons.c \
    build/*.o \
    -framework Cocoa -framework Security -framework WebKit \
    -o bin/silex || exit 1
echo "aedificatum: bin/silex"
