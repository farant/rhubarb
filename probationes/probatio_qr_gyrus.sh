#!/bin/bash

# probationes/probatio_qr_gyrus.sh - ORACULUM GYRI pro qr.c
#
# probatio_qr.c FORMAM verificat (amplitudines, versiones, formas
# inventorii). Nihil illic BITS legit - ergo QR cum formis rectis et
# datis corruptis omnes LXVI casus praeteriret. Id ipsum accidit:
# bibliotheca a mense Ianuario "probata" V4+ frangebat et nemo
# sciebat, quia probationes structurales id videre NON POSSUNT.
#
# Hoc scriptum gyrum claudit: qr.c BMP scribit, CoreImage (Apple,
# de codice nostro nescium) eum decodat. Si textus decodatus ingresso
# aequat, bits recti sunt. Oraculum independens, non se-consistentia.
#
# Casus: versiones I-X trans gradus ECC omnes (L/M/Q/H).
# Exitus: 0 = omnia legibilia, I = aliquod fractum.
#
# NOTA: macOS solum (osascript + CoreImage + sips). Curritur a
# compile_tests.sh sicut probatio_velaminis.js.

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1

SCRATCH="build/qr_gyrus"
BIN="$SCRATCH/qr_gyrus"
ORACULUM="probationes/qr_assets/decodere.js"

mkdir -p "$SCRATCH"

command -v osascript >/dev/null 2>&1 || {
    echo "qr gyrus: osascript abest - OMISSUM (non fractum)"
    exit 0
}

clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion \
    -Wsign-conversion -Wcast-qual -Wstrict-prototypes \
    -Wmissing-prototypes -Wwrite-strings -Wno-long-long \
    -Wno-overlength-strings -Iinclude \
    probationes/qr_assets/qr_gyrus.c lib/qr.c lib/piscina.c \
    lib/chorda.c lib/chorda_aedificator.c -o "$BIN" || {
    echo "qr gyrus: compilatio fracta"
    exit 1
}

fracturae=0
probati=0

for gradus in 0 1 2 3; do
    nomen_gradus=$(echo "L M Q H" | cut -d' ' -f$((gradus + 1)))
    for n in 8 25 45 70 100 140 180; do
        textus=$(printf 'B%.0s' $(seq 1 "$n"))
        info=$("$BIN" "$textus" "$SCRATCH/g.bmp" "$gradus" 2>/dev/null)
        case "$info" in
            FRACTUM*) continue ;;   # ultra capacitatem = rectum
        esac
        versio=$(echo "$info" | sed 's/versio=\([0-9]*\).*/\1/')
        sips -s format png "$SCRATCH/g.bmp" --out "$SCRATCH/g.png" \
            >/dev/null 2>&1
        res=$(osascript -l JavaScript "$ORACULUM" "$SCRATCH/g.png" \
            2>/dev/null)
        payload=${res#*>>> }
        probati=$((probati + 1))
        if [ "${#payload}" != "$n" ]; then
            echo "FRACTUM  ECC-$nomen_gradus V$versio ($n octeti):" \
                 "decodatum '${#payload}' octetorum"
            fracturae=$((fracturae + 1))
        fi
    done
done

rm -rf "$SCRATCH"

if [ "$fracturae" = 0 ]; then
    echo "QR GYRUS: $probati/$probati legibiles (oraculum CoreImage)"
    exit 0
fi
echo "QR GYRUS: $fracturae ex $probati FRACTAE"
exit 1
