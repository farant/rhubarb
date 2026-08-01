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

# Capacitates byte-mode per versionem/gradum (ISO 18004 tabula 7),
# HIC INDEPENDENTER scriptae. Duplex officium:
#  (1) onus = capacitas ipsa -> versio exspectata SELECTA est, ergo
#      omnes XL compositiones versionis/gradus tanguntur (non solum
#      quas mensurae fortuitae attingunt);
#  (2) onus maximum limitem impletionis/expletionis premit - locus
#      ubi numerus codewordorum falsus primum apparet.
# Si tabula qr.c ab his discrepat, versio recepta exspectatae non
# aequabit et probatio nominatim clamabit.
CAP_L="17 32 53 78 106 134 154 192 230 271"
CAP_M="14 26 42 62 84 106 122 152 180 213"
CAP_Q="11 20 32 46 60 74 86 108 130 151"
CAP_H="7 14 24 34 44 58 64 84 98 119"

for gradus in 0 1 2 3; do
    nomen_gradus=$(echo "L M Q H" | cut -d' ' -f$((gradus + 1)))
    eval "capacitates=\$CAP_$nomen_gradus"
    v_exspectata=0
    for n in $capacitates; do
        v_exspectata=$((v_exspectata + 1))
        textus=$(printf 'B%.0s' $(seq 1 "$n"))
        info=$("$BIN" "$textus" "$SCRATCH/g.bmp" "$gradus" 2>/dev/null)
        case "$info" in
            FRACTUM*)
                echo "FRACTUM  ECC-$nomen_gradus V$v_exspectata:" \
                     "generatio recusata ad capacitatem $n"
                fracturae=$((fracturae + 1))
                continue ;;
        esac
        versio=$(echo "$info" | sed 's/versio=\([0-9]*\).*/\1/')
        if [ "$versio" != "$v_exspectata" ]; then
            echo "FRACTUM  ECC-$nomen_gradus: onus $n octetorum" \
                 "V$versio elegit, V$v_exspectata exspectata"
            fracturae=$((fracturae + 1))
            continue
        fi
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
