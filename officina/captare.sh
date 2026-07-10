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

# obiecta ex compile_tests.sh EXTRACTA (unus fons veritatis)
OBJS=""
extract_sources() {
    sed -n "/^declare -a $1=(/,/^)/p" compile_tests.sh \
        | grep '^\s*"' | tr -d '",' | tr -d ' '
}
for src in $(extract_sources SOURCE_FILES); do
    obj="build/$(basename "$src" .c).o"
    if [ ! -f "$obj" ]; then
        echo "captare: obiectum deest ($obj) - curre ./compile_tests.sh" >&2
        exit 1
    fi
    OBJS="$OBJS $obj"
done
for src in $(extract_sources OBJC_SOURCES); do
    obj="build/$(basename "$src" .m).o"
    if [ ! -f "$obj" ]; then
        echo "captare: obiectum deest ($obj) - curre ./compile_tests.sh" >&2
        exit 1
    fi
    OBJS="$OBJS $obj"
done

FIXAE=0
NONDETERMINISTAE=0
FRACTAE=0
NEXUS_FRACTI=0
LISTA_NONDET=""
LISTA_FRACTAE=""

while IFS= read -r via; do
    titulus="$(basename "$via" .c)"
    bin="$BIN_DIR/$titulus"
    cap1="$CAP_DIR/$titulus.1"
    cap2="$CAP_DIR/$titulus.2"

    # nectere (si fons obiectis novior)
    if [ ! -x "$bin" ] || [ "$via" -nt "$bin" ]; then
        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$via" \
            $OBJS -framework Cocoa -framework Security \
            -o "$bin" 2>"$CAP_DIR/$titulus.nexus_error"; then
            printf '%-44s NEXUS FRACTUS\n' "$via"
            NEXUS_FRACTI=$((NEXUS_FRACTI + 1))
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
     "$FRACTAE fractae | $NEXUS_FRACTI nexus fracti ==="
[ -n "$LISTA_NONDET" ] && echo "nondeterministae:$LISTA_NONDET"
[ -n "$LISTA_FRACTAE" ] && echo "fractae:$LISTA_FRACTAE"

# cautio: effusiones diem hodiernam continentes cras DIFFERENT
HODIE="$(date +%Y-%m-%d)"
SUSPECTAE="$(grep -l "$HODIE" "$AUREA_DIR"/*.stdout 2>/dev/null || true)"
if [ -n "$SUSPECTAE" ]; then
    echo ""
    echo "CAUTIO: aurea diem hodiernam ($HODIE) continentia" \
         "(cras different - exclusiones nominandae?):"
    echo "$SUSPECTAE"
fi
