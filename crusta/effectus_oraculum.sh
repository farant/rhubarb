#!/bin/bash
# crusta/effectus_oraculum.sh - oraculum effectuum: scriptum sub bash
# Homebrew cum interpositione currit (crusta/instrumenta/
# interpositio_macos.c) et quae bash vere legit, scribit, probat,
# enumerat, exsequitur cum summario statico comparat (effectus-spec
# par. VI.2; planum T5). macOS solum.
#
# Usage:  ./crusta/effectus_oraculum.sh -domus <scriptum> [argumenta]
#             in arbore viva; non tecta nominantur; lectiones ante
#             scripturam in build/effectus/ante_scripta.tsv
#         ./crusta/effectus_oraculum.sh -scribere
#             fixa (crusta/probationes/fixa/effectus/oraculum/*.sh) in
#             copia temporaria currit et aura observata scribit
#             (fixa/effectus/oraculum_aura/<nomen>.stml; causa nominata)
#         ./crusta/effectus_oraculum.sh -probare
#             fixa viva contra summaria statica (sine auris)
# Exit:   0 omnia tecta | 1 non tecta | 2 usus / aedificatio fracta
#
# Lex C14 (crusta): portae nihil spawnant - probatio_crusta_effectus
# summaria statica contra AURAS commissas comparat; hoc instrumentum
# solum bash vivum currit.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
BUILD_DIR="$CRUSTA_DIR/build"
BASH_DOMUS=/opt/homebrew/bin/bash
LIB="$BUILD_DIR/libinterpositio.dylib"
FIXA="$CRUSTA_DIR/probationes/fixa/effectus/oraculum"
AURA="$CRUSTA_DIR/probationes/fixa/effectus/oraculum_aura"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

[ -x "$BASH_DOMUS" ] || { echo "oraculum: $BASH_DOMUS deest" >&2; exit 2; }
mkdir -p "$BUILD_DIR" "$RADIX_DIR/build/effectus"
clang "${GCC_FLAGS[@]}" -I"$RADIX_DIR/include" -dynamiclib \
    -o "$LIB" "$CRUSTA_DIR/instrumenta/interpositio_macos.c" || {
    echo "oraculum: interpositio non structa" >&2; exit 2; }
"$CRUSTA_DIR/effectus.sh" "$CRUSTA_DIR/effectus.sh" > /dev/null || exit 2
EFFECTUS="$BUILD_DIR/effectus"
TABULA="$CRUSTA_DIR/effectus_mandata.stml"

# currere <radix> <liber> <scriptum> [argumenta]: sub interpositione
currere () {
    local radix="$1" liber="$2"
    shift 2
    rm -f "$liber"
    (cd "$radix" && INTERPOSITIO_LIBER="$liber" \
        DYLD_INSERT_LIBRARIES="$LIB" "$BASH_DOMUS" "$@") > /dev/null 2>&1
    return 0
}

modus="${1:-}"
case "$modus" in
    -domus)
        [ $# -ge 2 ] || { echo "usus: -domus <scriptum> [argumenta]" >&2; exit 2; }
        shift
        liber="$RADIX_DIR/build/effectus/oraculum.liber"
        currere "$RADIX_DIR" "$liber" "$@"
        "$EFFECTUS" -comparare "$1" "$liber" -radix "$RADIX_DIR" \
            -ante_scripta "$RADIX_DIR/build/effectus/ante_scripta.tsv"
        ;;
    -scribere|-probare)
        mkdir -p "$AURA"
        fracta=0
        for f in "$FIXA"/*.sh; do
            nomen="$(basename "$f" .sh)"
            T="$(mktemp -d)"
            cp -R "$FIXA/." "$T/"
            radix="$(cd "$T" && pwd -P)"
            currere "$radix" "$T/oraculum.liber" "$nomen.sh"
            if [ "$modus" = -scribere ]; then
                "$EFFECTUS" -observata "$nomen.sh" "$T/oraculum.liber" \
                    -radix "$radix" -tabula "$TABULA" > "$AURA/$nomen.stml" || fracta=1
                echo "aura: $nomen ($(grep -c 'per="observatum"' "$AURA/$nomen.stml") situs)"
            else
                echo "== $nomen"
                "$EFFECTUS" -comparare "$nomen.sh" "$T/oraculum.liber" \
                    -radix "$radix" -tabula "$TABULA" || fracta=1
            fi
            rm -rf "$T"
        done
        exit "$fracta"
        ;;
    *)
        sed -n '9,20p' "${BASH_SOURCE[0]}" >&2
        exit 2
        ;;
esac
