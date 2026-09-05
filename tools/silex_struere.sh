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

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$SCRIPT_DIR/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

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

# corpus bibliothecarum: capsula infixa - bloccus COMMUNIS cum
# tools/briar_struere.sh (tools/corpus_infixum.sh)
source "$SCRIPT_DIR/corpus_infixum.sh"
corpus_infixum_regenerare || exit 1

# obiecta silvae (machina differentiae unitatum --unitates) -
# lacus idem quo differre.sh (silva/build), vexillae eaedem
# (fontes silvae -Wno-overlength-strings poscunt)
declare -a SILVA_FLAGS=("${GCC_FLAGS[@]}" "-Wno-overlength-strings")
declare -a SILVA_INCLUDA=("-Iinclude" "-Isilva/fontes"
    "-Isilva/instrumenta")
mkdir -p silva/build
silva_recentior () {
    find include silva/fontes silva/instrumenta -name '*.h' \
        -newer "$1" 2>/dev/null | head -1
}
silva_obiecta=""
for src in silva/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="silva/build/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || \
       [ -n "$(silva_recentior "$obj")" ]; then
        echo "  [silva] $base.c"
        clang "${SILVA_FLAGS[@]}" "${SILVA_INCLUDA[@]}" -c "$src" \
            -o "$obj" || exit 1
    fi
    silva_obiecta="$silva_obiecta $obj"
done
for base in silva_unitates silva_differre; do
    src="silva/instrumenta/$base.c"
    obj="silva/build/instr_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || \
       [ -n "$(silva_recentior "$obj")" ]; then
        echo "  [instr] $base.c"
        clang "${SILVA_FLAGS[@]}" "${SILVA_INCLUDA[@]}" -c "$src" \
            -o "$obj" || exit 1
    fi
    silva_obiecta="$silva_obiecta $obj"
done

# obiecta bibliothecarum: build/*.o SINE obiectis probationum
# (probatio_*.o - reliquiae cursoris radicis ante 2026-09-04, cum
# obiecta probationum in build/ cadebant; unum eorum symbolum capsulae
# speculi poscit quod sola probatio nectit - globus nudus nexum frangit)
bibliothecae_obiecta=""
for o in build/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    bibliothecae_obiecta="$bibliothecae_obiecta $o"
done

mkdir -p bin
echo "  [silex] tools/silex.c + capsula_silex_frons.c + corpus"
clang "${GCC_FLAGS[@]}" -Iinclude -Isilva/fontes \
    -Isilva/instrumenta \
    tools/silex.c tools/silex_assets/capsula_silex_frons.c \
    build/capsula_corpus_silicis.c \
    $bibliothecae_obiecta \
    $silva_obiecta \
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
