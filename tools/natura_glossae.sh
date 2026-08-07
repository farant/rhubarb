#!/bin/bash

# tools/natura_glossae.sh - census glossarum (spec glossae par. 5)
#
#   sine argumentis   relatio (tabula + CARENS/GEMINA nominatim)
#   -pagina           natura/cocta/glossae.html regenerare
#   -probare          paginam conferre solum: 0 recens, 1 RANCIDA,
#                     2 defectus instrumenti
#   cetera            recta ad bin/natura_glossae
#     (-porta -machina -linguae CSV -radix DIR -html VIA)

set -u
cd "$(dirname "$0")/.." || exit 2

PORTA=bin/natura_glossae
PAGINA=natura/cocta/glossae.html
STRUCTOR=./tools/natura_struere.sh

if [ ! -x "$PORTA" ]; then
    echo "natura_glossae: $PORTA abest - strue: $STRUCTOR" >&2
    exit 2
fi
for _f in tools/natura_glossae.c lib/natura.c include/natura.h \
          lib/stml.c include/stml.h; do
    if [ ! -e "$_f" ]; then
        echo "natura_glossae: custos '$_f' ABEST" >&2
        exit 2
    fi
    if [ "$_f" -nt "$PORTA" ]; then
        echo "natura_glossae: $PORTA STALUS ($_f recentior) - strue: $STRUCTOR" >&2
        exit 2
    fi
done

case "${1-}" in
-pagina)
    exec "$PORTA" -html "$PAGINA"
    ;;
-probare)
    TMP=build/glossae_probandum.$$.html
    trap 'rm -f "$TMP"' EXIT
    "$PORTA" -html "$TMP" >/dev/null || exit 2
    [ -s "$TMP" ] || { echo "natura_glossae: pagina vacua" >&2; exit 2; }
    if ! cmp -s "$TMP" "$PAGINA"; then
        echo "natura_glossae: RANCIDA $PAGINA - regenera: ./tools/natura_glossae.sh -pagina" >&2
        exit 1
    fi
    echo "natura_glossae: pagina recens"
    exit 0
    ;;
esac

exec "$PORTA" "$@"
