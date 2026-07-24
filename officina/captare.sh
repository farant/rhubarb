#!/bin/bash

# officina/captare.sh - effusiones nativas FIGERE (aurea columnae
# stdout cursoris; deliverable M2c "native capture pinning")
#
# Quisque suitem undae (cursor -enumerare = UNUS fons veritatis)
# nativam BIS currit:
#   - effusiones inter se differunt -> NONDETERMINISTA (candidata
#     exclusionis nominatae in cursor.c; NON figitur)
#   - aequae -> figitur in
#     officina/probationes/fixa/stdout_nativa/<titulus>.stdout
#
# NOTA: generare NON curritur - cursor plagulas ut-sunt demittit,
# ergo nativum easdem ut-sunt aedificet (paritas).
#
# Praerequisita: ./compile_tests.sh cucurrit (obiecta in build/);
#                ./officina/cursor.sh cucurrit (cursor in
#                officina/build/).
#
# Usus: ./officina/captare.sh    # ex radice repositorii

set -u

OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
cd "$RADIX_DIR"

CURSOR_BIN="$OFF_DIR/build/cursor"
AUREA_DIR="$OFF_DIR/probationes/fixa/stdout_nativa"
BIN_DIR="$OFF_DIR/build/captare_bin"
CAP_DIR="$OFF_DIR/build/captare_effusiones"

if [ ! -x "$CURSOR_BIN" ]; then
    echo "captare: cursor deest - curre ./officina/cursor.sh primum" >&2
    exit 1
fi

mkdir -p "$AUREA_DIR" "$BIN_DIR" "$CAP_DIR"

# flags IDENTICA compile_tests.sh (paritas nativa)
declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings" "-fbracket-depth=512"
)
declare -a INCLUDE_FLAGS=("-Iinclude" "-Iprobationes" "-Ibook_assets")

# fontes venditi: compile_tests.sh eosdem tenet (linea ~41). Si illic
# additur et hic non, nexus CLAMAT (symbola indefinita -> NEXUS
# FRACTUS -> porta finalis exit 1). Rotatio ergo sonora, non tacita.
declare -a VENDOR_SOURCES=("vendor/sqlite3.c")

# EXCLUSIONES NEXUS - suitae quas captare nectere NON potest, quaeque
# causam nominatam habent (mos cursoris: quaeque exclusio causam).
# Omnes capsulam GENERATAM poscunt (compile_tests.sh eas per
# SPECULUM_APPS cudit in build/speculum/<titulus>/); captare eam
# generationem non agit. NULLA earum sub machinula praeterit
# (VITIUM/DECIPULA/SISTERE), ergo columna stdout eas numquam
# confert et aureum nullum deest. Porta ergo viridis esse potest -
# porta quae viridis fieri non potest neglegitur, et ita hoc
# instrumentum tacite putruit.
declare -a EXCLUSIONES_NEXUS=(
    "probatio_speculum:capsula speculi generata"
    "probatio_speculum_fontium:capsula speculi generata"
    "probatio_vitrea:capsula vitreae generata"
    "probatio_vitrea_hospes:capsula vitreae generata"
    "probatio_vitrea_tabella:capsula vitreae generata"
)

_causa_exclusionis() {
    local t
    for t in "${EXCLUSIONES_NEXUS[@]}"; do
        if [ "${t%%:*}" = "$1" ]; then
            echo "${t#*:}"
            return 0
        fi
    done
    return 1
}

# compagines: PARITAS cum compile_tests.sh linea 321 (nexus probationum).
declare -a COMPAGES=(
    "-framework" "Cocoa" "-framework" "Security" "-framework" "WebKit"
)

# obiecta ex FONTIBUS GENERATIS (unus fons veritatis - aedilis ea
# scribit; compile_tests.sh eosdem haurit). HAURIMUS, non radimus:
# radere formam alterius plagulae fragile est - ita hic fractum est
# a 7e1d1de (SOURCE_FILES ex compile_tests.sh in fontes generata
# migravit; radendo NIHIL redditum est, OBJS vacua, nexus omnis
# fractus, captare tamen exitu 0 "successit").
FONTES_GENERATA="$RADIX_DIR/compile_tests_fontes_generata.sh"
if [ ! -f "$FONTES_GENERATA" ]; then
    echo "captare: fontes generata desunt ($FONTES_GENERATA)" >&2
    echo "         curre bin/aedilis" >&2
    exit 1
fi
# shellcheck source=/dev/null
. "$FONTES_GENERATA"

if [ -z "${SOURCE_FILES+x}" ] || [ "${#SOURCE_FILES[@]}" -eq 0 ]; then
    echo "captare: SOURCE_FILES vacua post haustum -" \
         "fontes generata fracta" >&2
    exit 1
fi

OBJS=""
for src in "${SOURCE_FILES[@]}"; do
    obj="build/$(basename "$src" .c).o"
    if [ ! -f "$obj" ]; then
        echo "captare: obiectum deest ($obj) - curre ./compile_tests.sh" >&2
        exit 1
    fi
    OBJS="$OBJS $obj"
done
for src in "${OBJC_SOURCES[@]}"; do
    obj="build/$(basename "$src" .m).o"
    if [ ! -f "$obj" ]; then
        echo "captare: obiectum deest ($obj) - curre ./compile_tests.sh" >&2
        exit 1
    fi
    OBJS="$OBJS $obj"
done

# obiecta VENDITA (sqlite3 - scrinium ea vocat)
for src in "${VENDOR_SOURCES[@]}"; do
    obj="build/$(basename "$src" .c).o"
    if [ ! -f "$obj" ]; then
        echo "captare: obiectum venditum deest ($obj) -" \
             "curre ./compile_tests.sh" >&2
        exit 1
    fi
    OBJS="$OBJS $obj"
done

# PORTA: OBJS vacua = nexus omnis frangetur silenter. Recusa CLARE.
if [ -z "$OBJS" ]; then
    echo "captare: OBJS vacua - nihil nectendum. Recusatum." >&2
    exit 1
fi

# obiectum novissimum: binarium cached vetustius quam obiecta
# effusionem OBSOLETAM figeret (aureum ex binario sene = vitium
# subdolum). Relige si quodvis obiectum binario novius est.
NOVISSIMUM_OBIECTUM="$(ls -t build/*.o 2>/dev/null | head -1)"

FIXAE=0
NONDETERMINISTAE=0
FRACTAE=0
NEXUS_FRACTI=0
EXCLUSAE=0
LISTA_NONDET=""
LISTA_FRACTAE=""
LISTA_NEXUS_FRACTI=""

while IFS= read -r via; do
    titulus="$(basename "$via" .c)"
    bin="$BIN_DIR/$titulus"
    cap1="$CAP_DIR/$titulus.1"
    cap2="$CAP_DIR/$titulus.2"

    if CAUSA_EXCL="$(_causa_exclusionis "$titulus")"; then
        printf '%-44s EXCLUSA (%s)\n' "$via" "$CAUSA_EXCL"
        EXCLUSAE=$((EXCLUSAE + 1))
        continue
    fi

    # nectere (si fons OPPURE obiectum quodvis binario novius)
    if [ ! -x "$bin" ] || [ "$via" -nt "$bin" ] \
        || [ -n "$NOVISSIMUM_OBIECTUM" -a \
             "$NOVISSIMUM_OBIECTUM" -nt "$bin" ]; then
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$via" \
            $OBJS "${COMPAGES[@]}" \
            -o "$bin" 2>"$CAP_DIR/$titulus.nexus_error"; then
            printf '%-44s NEXUS FRACTUS\n' "$via"
            NEXUS_FRACTI=$((NEXUS_FRACTI + 1))
            LISTA_NEXUS_FRACTI="$LISTA_NEXUS_FRACTI $titulus"
            continue
        fi
    fi

    # cursus duplex - somnus 1s inter cursus: semina time(NULL)
    # in secundo eodem laterent (multipart ita primum latuit!)
    if ! "$bin" >"$cap1" 2>/dev/null; then
        printf '%-44s FRACTA (exitus nativus non 0)\n' "$via"
        FRACTAE=$((FRACTAE + 1))
        LISTA_FRACTAE="$LISTA_FRACTAE $titulus"
        continue
    fi
    sleep 1
    "$bin" >"$cap2" 2>/dev/null

    if ! cmp -s "$cap1" "$cap2"; then
        printf '%-44s NONDETERMINISTA (exclusio nominanda)\n' "$via"
        NONDETERMINISTAE=$((NONDETERMINISTAE + 1))
        LISTA_NONDET="$LISTA_NONDET $titulus"
        continue
    fi

    cp "$cap1" "$AUREA_DIR/$titulus.stdout"
    printf '%-44s FIXA (%s octeti)\n' "$via" \
        "$(wc -c <"$cap1" | tr -d ' ')"
    FIXAE=$((FIXAE + 1))
done < <("$CURSOR_BIN" -enumerare)

echo ""
echo "=== CAPTARE: $FIXAE fixae | $NONDETERMINISTAE nondeterministae |" \
     "$FRACTAE fractae | $NEXUS_FRACTI nexus fracti |" \
     "$EXCLUSAE exclusae ==="
[ -n "$LISTA_NONDET" ] && echo "nondeterministae:$LISTA_NONDET"
[ -n "$LISTA_FRACTAE" ] && echo "fractae:$LISTA_FRACTAE"
[ -n "$LISTA_NEXUS_FRACTI" ] && echo "nexus fracti:$LISTA_NEXUS_FRACTI"

# cautio: effusiones diem hodiernam continentes cras DIFFERENT
HODIE="$(date +%Y-%m-%d)"
SUSPECTAE="$(grep -l "$HODIE" "$AUREA_DIR"/*.stdout 2>/dev/null || true)"
if [ -n "$SUSPECTAE" ]; then
    echo ""
    echo "CAUTIO: aurea diem hodiernam ($HODIE) continentia" \
         "(cras different - exclusiones nominandae?):"
    echo "$SUSPECTAE"
fi

# PORTA: nexus fracti / fractae = aurea TACITE senescunt (suita
# quae nectere non potest aureum vetus servat, et cursor id postea
# ut DIFFERT ostendit - indiscernibile a divergentia machinulae).
# Nondeterministae NON obstant: exclusiones nominatae sunt.
if [ "$NEXUS_FRACTI" -gt 0 ] || [ "$FRACTAE" -gt 0 ]; then
    echo ""
    echo "CAPTARE RECUSAT: $NEXUS_FRACTI nexus fracti," \
         "$FRACTAE fractae - aurea eorum SENESCUNT." >&2
    exit 1
fi
exit 0
