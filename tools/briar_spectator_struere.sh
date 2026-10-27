#!/bin/bash
# tools/briar_spectator_struere.sh - bin/briar-spectator aedificare
#
# Spectator = briar + fenestra (spec par. 4.7). Obiecta briaris
# (briar/build/*.o) omnia fert quae redditio poscit; fenestra autem
# atrium/vitream/fenestram poscit, quae in build/ radicis iacent.
#
# LEX NEXUS: briar/build et build plagulas bibliothecae EASDEM ferunt
# (piscina.o, chorda.o...). Ergo e build/ ea SOLA sumuntur quorum
# nomen briar/build non habet - aliter symbola gemina.
#
# ORDO: ./compile_tests.sh registrum (obiecta radicis, .m inclusa) et
# ./briar/compile_probationes.sh registrum ANTE hoc.
#
# Usus: ./tools/briar_spectator_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
source "$SCRIPT_DIR/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

if [ -z "$(ls -A briar/build/briar_*.o 2>/dev/null)" ]; then
    echo "spectator: briar/build/ vacuum - ./briar/compile_probationes.sh registrum prius" >&2
    exit 1
fi
if [ -z "$(ls -A build/atrium.o build/vitrea_macos.o 2>/dev/null)" ]; then
    echo "spectator: build/ atrium aut vitrea caret - ./compile_tests.sh registrum prius" >&2
    exit 1
fi

# capsulae: actiones fabricae suae (1b T6) - sub executore
# (FABRICA_AGIT) hoc scriptum SOLUM nectit (sanare eas ante ordinat);
# manu vocatum eas, si recentiores fontes, regenerat
if [ -z "${FABRICA_AGIT:-}" ]; then
    source "$SCRIPT_DIR/corpus_infixum.sh"
    corpus_infixum_regenerare || exit 1
    source "$SCRIPT_DIR/briar_facies_capsula.sh"
    briar_facies_capsula_regenerare || exit 1
fi

# obiecta briaris (probationes exclusae)
OBJ=""
for o in briar/build/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done

# obiecta radicis quae briar/build NON habet (fenestra, vitrea, atrium,
# thema, imago... et quidquid clausura eorum poscit) - EX CLAUSURA
# (bin/aedilis --partes, ut stml_struere.sh), numquam build/*.o
# universum: obiectum plagulae renominatae aut deletae in build/ manet
# et symbola duplicata nectebat (parcum …QBDJVE1, testa_android_initus
# -> testa_initus 2026-10-24)
if [ ! -x bin/aedilis ]; then
    echo "briar_spectator_struere: bin/aedilis deest - ./tools/aedilis_struere.sh prius" >&2
    exit 1
fi
ADDITA=""
NUMERUS_PARTIUM=0
while IFS=$'\t' read -r genus pars; do
    # O = partes domus, V = vendita (sqlite3, bearssl: obiecta sua)
    [ "$genus" = "O" ] || [ "$genus" = "V" ] || continue
    # fontes GENERATI sub build/ (provenientia, capsula corporis)
    # infra in linea clang ipsa compilantur
    case "$pars" in build/*) continue ;; esac
    NUMERUS_PARTIUM=$((NUMERUS_PARTIUM + 1))
    b="$(basename "${pars%.*}").o"
    [ -f "briar/build/$b" ] && continue
    # amalgamata in briar/build nomine 'amalgama_X.o' (silva.c ->
    # amalgama_silva.o) - iam in $OBJ nexa
    [ -f "briar/build/amalgama_$b" ] && continue
    if [ ! -f "build/$b" ]; then
        echo "briar_spectator_struere: build/$b deest (pars $pars) - ./compile_tests.sh prius" >&2
        exit 1
    fi
    ADDITA="$ADDITA build/$b"
done < <(./bin/aedilis tools/briar_spectator.c --partes)
if [ "$NUMERUS_PARTIUM" -eq 0 ]; then
    echo "briar_spectator_struere: clausura vacua - aedilis defecit?" >&2
    exit 1
fi

mkdir -p bin
# provenientia (fabrica T7)
PROV_OBJ="$(./tools/provenientia_obiectum.sh briar_spectator bin/briar-spectator tools/briar_spectator.c)" || exit 1
echo "  [spectator] tools/briar_spectator.c + briar + atrium/vitrea"
clang "${GCC_FLAGS[@]}" -Iinclude -Imateria/fontes -Imd/fontes \
    -Ibriar/fontes -Isilva/amalgama \
    tools/briar_spectator.c "$PROV_OBJ" \
    build/capsula_corpus_silicis.c \
    $OBJ $ADDITA \
    -framework Cocoa -framework Security -framework WebKit \
    -framework IOKit -framework CoreFoundation \
    -o bin/briar-spectator || exit 1
echo "aedificatum: bin/briar-spectator"

# rm ante cp: macOS binarium superscriptum SIGKILL dat
# institutio in ~/.bin: actio fabricae sua ('institutio_briar_spectator', 1b T5) -
# sub executore (FABRICA_AGIT) hoc scriptum SOLUM nectit; manu vocatum
# etiam instituit - in main solo (arbor secundaria: omissum, -si-licet)
if [ -z "${FABRICA_AGIT:-}" ]; then
    ./tools/instituere.sh -si-licet bin/briar-spectator || exit 1
fi
