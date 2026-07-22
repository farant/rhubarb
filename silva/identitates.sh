#!/bin/bash

# silva/identitates.sh - index identitatum nid + mintatio + solutio
# (parcum 01KY3D7EJP frustum B)
#
# Usus:
#   ./silva/identitates.sh -renovare              -> build/identitates.tsv
#   ./silva/identitates.sh '##PRAEFIXUM'          -> solvere (>= VI char)
#   ./silva/identitates.sh -mintare [-scribere] [viae...]
#   ./silva/identitates.sh -porta                 -> porta mintationis
#
# NB: semper ex radice repositorii currit (cd infra).

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

declare -a GCC_FLAGS=(
    "-std=c89" "-pedantic" "-Wall" "-Wextra" "-Werror"
    "-Wconversion" "-Wsign-conversion" "-Wcast-qual"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wwrite-strings"
    "-Wno-long-long" "-Wno-overlength-strings"
)
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/identitates_fontes_generata.sh"

newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$SILVA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/fons_$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

BIN_SRC="$SILVA_DIR/instrumenta/principalia/identitates.c"
BIN="$BUILD_DIR/identitates"
if [ ! -x "$BIN" ] || [ "$BIN_SRC" -nt "$BIN" ] || [ -n "$(newest_header "$BIN")" ]; then
    echo "  [silva] identitates.c" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$BIN_SRC" $obj_files \
        -o "$BIN" || exit 1
fi

cd "$RADIX_DIR"

# candidatae: plagulae domus cum commentariis tag-formibus (filtrum
# vile - adoptio in-postulatione paucas plagulas notatas relinquit;
# generata/amalgamata/fixturae numquam indicantur)
_candidatae() {
    local omnes
    omnes="$(git ls-files '*.c' '*.h' \
        | grep -vE '^vendor/|/vendor/|^build/|/build/|/amalgama/|_generata|/fixa/')"
    [ -n "$omnes" ] || return 0
    echo "$omnes" | tr '\n' '\0' \
        | xargs -0 grep -lE '/\*[[:space:]]*<[A-Za-z]|//[[:space:]]*<[A-Za-z]|^[[:space:]]*\*[[:space:]]*<[A-Za-z]' \
            2>/dev/null || true
}

case "${1:-}" in
    -renovare)
        vias="$(_candidatae)"
        if [ -z "$vias" ]; then
            {
                echo "# identitates.tsv GENERATUM $(date +%s) - DISPONIBILE, noli committere; regenera: ./silva/identitates.sh -renovare"
            } > "$RADIX_DIR/build/identitates.tsv"
            {
                echo "# citationes.tsv GENERATUM $(date +%s) - DISPONIBILE, noli committere; regenera: ./silva/identitates.sh -renovare"
            } > "$RADIX_DIR/build/citationes.tsv"
            echo "identitates: nullae candidatae - indices vacui" >&2
            exit 0
        fi
        # shellcheck disable=SC2086
        exec "$BIN" --renovare "$RADIX_DIR/build/identitates.tsv" \
            "$RADIX_DIR/build/citationes.tsv" $vias
        ;;
    -mintare)
        shift
        scribere=""
        if [ "${1:-}" = "-scribere" ]; then
            scribere="--scribere"
            shift
        fi
        if [ $# -gt 0 ]; then
            vias="$*"
        else
            vias="$(_candidatae)"
        fi
        if [ -z "$vias" ]; then
            echo "identitates: nihil mintandum" >&2
            exit 0
        fi
        # shellcheck disable=SC2086
        exec "$BIN" --mintare $scribere $vias
        ;;
    -porta)
        fixtura="$SILVA_DIR/probationes/fixa/annotationes/mintanda.c"
        porta="$BUILD_DIR/mintanda_porta.c"
        cp "$fixtura" "$porta" || exit 1
        "$BIN" --mintare --scribere "$porta" || {
            echo "identitates -porta: FRACTA (mintatio)"; exit 1; }
        strip_sed='s/ v="[0-9A-HJKMNP-TV-Z]\{26\}"//g; s/nid="[0-9A-HJKMNP-TV-Z]\{26\}"/nid/g'
        if ! diff <(sed "$strip_sed" "$porta") <(sed "$strip_sed" "$fixtura") >/dev/null; then
            echo "identitates -porta: FRACTA (byte-identitas exutis valoribus)"
            exit 1
        fi
        reliquiae="$("$BIN" --mintare "$porta" 2>/dev/null | grep -c 'petitio' || true)"
        if [ "$reliquiae" != "0" ]; then
            echo "identitates -porta: FRACTA (petitiones reliquae: $reliquiae)"
            exit 1
        fi
        echo "identitates -porta: VERIFICATUM"
        ;;
    ""|-*)
        echo "usus: identitates.sh -renovare | '##PRAEFIXUM' | -mintare [-scribere] [viae...] | -porta" >&2
        exit 2
        ;;
    *)
        if [ ! -f "$RADIX_DIR/build/identitates.tsv" ]; then
            "$0" -renovare || exit 1
        fi
        exec "$BIN" --solvere "$1" "$RADIX_DIR/build/identitates.tsv"
        ;;
esac
