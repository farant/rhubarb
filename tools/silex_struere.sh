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
    lib/mensa_assets/fontes/logica.js
    lib/mensa_assets/fontes/thema.js
    lib/mensa_assets/fontes/scida.js
    lib/mensa_assets/fontes/scidula.js
    lib/mensa_assets/fontes/theca.js
    lib/mensa_assets/fontes/imago.js
    lib/mensa_assets/fontes/orbis.js
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
   [ tools/silex_assets/repositorium.js -nt \
     tools/silex_assets/capsula_silex_frons.c ] || \
   [ "$MENSA_EXITUS" -nt \
     tools/silex_assets/capsula_silex_frons.c ]; then
    echo "  [capsula] silex_frons (assetum recentior)"
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || exit 1
    fi
    ./bin/capsula_generare tools/silex_assets/silex.toml || exit 1
fi

# corpus bibliothecarum: capsula infixa. Regeneratum SOLUM cum
# fontes mutantur - stampa commit eius temporis fixatur (semantica
# recta: stampa = ultima mutatio corporis, non ultima aedificatio)
CORPUS_C=build/capsula_corpus_silicis.c
regen=0
if [ ! -f "$CORPUS_C" ]; then
    regen=1
elif [ -n "$(find lib include vendor tools/capsula_generare.c \
        -newer "$CORPUS_C" -print -quit 2>/dev/null)" ]; then
    regen=1
fi
if [ "$regen" = 1 ]; then
    echo "  [corpus] stampa + capsula (tardum semel)"
    STAMPA="commit=$(git rev-parse --short HEAD 2>/dev/null \
        || echo ignotum)"
    if [ -n "$(git status --porcelain 2>/dev/null)" ]; then
        STAMPA="$STAMPA SORDIDUM"
    fi
    STAMPA="$STAMPA dies=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
    printf '%s\n' "$STAMPA" > corpus.versio
    cat > corpus_silicis.toml <<'TOML'
# GENERATUM a silex_struere.sh - NE MANU EDITES (gitignoratum)
corpus_silicis_files = ["lib/*.c", "lib/*.m", "include/*.h", "vendor/*", "tools/capsula_generare.c", "corpus.versio"]
corpus_silicis_compress = true
TOML
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || exit 1
    fi
    ./bin/capsula_generare corpus_silicis.toml || exit 1
    mv capsula_corpus_silicis.h capsula_corpus_silicis.c build/ \
        || exit 1
fi

mkdir -p bin
echo "  [silex] tools/silex.c + capsula_silex_frons.c + corpus"
clang "${GCC_FLAGS[@]}" -Iinclude \
    tools/silex.c tools/silex_assets/capsula_silex_frons.c \
    build/capsula_corpus_silicis.c \
    build/*.o \
    -framework Cocoa -framework Security -framework WebKit \
    -o bin/silex || exit 1
echo "aedificatum: bin/silex"

# institutio: exemplum in ~/.bin (in PATH) - proiecta silicetum
# silicem sine via fabricae vocant; rm ante cp (macOS binarium
# superscriptum SIGKILL dat - inodus recens obligatorius)
mkdir -p "$HOME/.bin"
rm -f "$HOME/.bin/silex"
cp bin/silex "$HOME/.bin/silex"
echo "institutum: ~/.bin/silex"
