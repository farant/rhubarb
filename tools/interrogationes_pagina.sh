#!/bin/bash
# tools/interrogationes_pagina.sh - docs/faq.html ex interrogationibus
# unitatum (lib/S.interrogationes.stml; spec unitates-codicis par. VII,
# U5b gradus 4). Actio fabricae 'interrogationes_pagina'
# (aedificatio.stml); pagina = bin/fabrica quid -interrogationes -html.
#
#   -pagina    docs/faq.html regenerare (sub iudice fabricae in
#              $FABRICA_SCRIPTURA)
#   -probare   paginam conferre solum: 0 recens, 1 RANCIDA, 2 defectus
#
# Exitus: 0 · 1 vitia interrogationum (nihil scriptum) aut RANCIDA ·
#         2 bin/fabrica deest aut usus
set -u
cd "$(dirname "$0")/.." || exit 2

PAGINA=docs/faq.html
if [ ! -x bin/fabrica ]; then
    echo "interrogationes_pagina: bin/fabrica abest - strue: ./tools/fabrica_struere.sh" >&2
    exit 2
fi

case "${1-}" in
-pagina)
    if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
        mkdir -p "$FABRICA_SCRIPTURA/$(dirname "$PAGINA")" || exit 2
        exec bin/fabrica quid -interrogationes -html "$FABRICA_SCRIPTURA/$PAGINA"
    fi
    exec bin/fabrica quid -interrogationes -html "$PAGINA"
    ;;
-probare)
    T="$(mktemp "${TMPDIR:-/tmp}/faq.XXXXXX")"
    trap 'rm -f "$T"' EXIT
    bin/fabrica quid -interrogationes -html "$T" || exit 2
    [ -s "$T" ] || { echo "interrogationes_pagina: pagina vacua" >&2; exit 2; }
    if ! cmp -s "$T" "$PAGINA"; then
        echo "interrogationes_pagina: RANCIDA $PAGINA - regenera: ./tools/interrogationes_pagina.sh -pagina" >&2
        exit 1
    fi
    echo "interrogationes_pagina: pagina recens"
    exit 0
    ;;
*)
    echo "usus: ./tools/interrogationes_pagina.sh -pagina | -probare" >&2
    exit 2
    ;;
esac
