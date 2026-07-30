#!/bin/bash

# probationes/fixa/villa/ssh_stipes.sh - ssh FICTUS pro portis.
#
# VILLA_SSH ad hoc scriptum ponitur; agens id per 'execvp' vocat,
# ergo nulla manipulatio PATH necessaria est et ssh VERUS umquam
# obumbrari non potest. (Idioma RHUBARB_RADIX: sutura una quam et
# configuratio et probatio premunt.)
#
# Vexilla ssh IGNORAT et ultimum argumentum (scriptum remotum)
# inspicit. Effusionem COMPOSITAM emittit quam ex fixis SINGULARIBUS
# componit - ergo fixa singularia veritas UNA manent et forma
# composita ex eis DERIVATUR. Fixum compositum manu servatum mox
# ab illis dissideret.
#
# Modus per VILLA_STIPES_MODUS eligitur:
#   bene           - probatio plena, omnes sectiones, codex 0
#   nginx_fractum  - nginx sectio codicem I reddit (cetera valent)
#   sectio_absens  - sectio 'memoria' omnino deest
#   abscissum      - effusio in medio sectionis desinit
#   hospes_ignotus - ssh ipse cadit (codex CCLV)
#   permissio      - ssh ipse cadit (codex CCLV, causa alia)
#   imperium       - modus actionis: effusionem imperii reddit
#   imperium_malum - modus actionis: codex III

set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
FX="$RADIX/probationes/fixa/villa"
MODUS="${VILLA_STIPES_MODUS:-bene}"

sectio () {
    # $1 = titulus, $2 = plagula fixi, $3 = codex
    printf '##VILLA##%s##\n' "$1"
    if [ -f "$2" ]; then cat "$2"; fi
    printf '##VILLA##fin##%s##\n' "$3"
}

case "$MODUS" in
hospes_ignotus)
    cat "$FX/ssh/hospes_ignotus.err" >&2
    exit 255
    ;;
permissio)
    cat "$FX/ssh/clavis_negata.err" >&2
    exit 255
    ;;
imperium)
    echo "servitium restitutum"
    exit 0
    ;;
imperium_malum)
    echo "Failed to restart nulla.service: Unit not found." >&2
    exit 3
    ;;
abscissum)
    # conexio media cecidit: sectio aperta, nullus finis
    printf '##VILLA##unitates##\n'
    head -3 "$FX/list_units.txt"
    exit 0
    ;;
esac

# Sub modis probationis, scriptum NON-probationis = imperium
# liberum (fumus faciei probationem ET actionem uno cursu agit -
# modus ambientis semel ponitur, dispectio in scripto ipso est)
SCRIPTUM="${!#}"
case "$SCRIPTUM" in
*"s unitates"*) : ;;
*)
    echo "servitium restitutum"
    exit 0
    ;;
esac

sectio unitates "$FX/list_units.txt" 0
sectio servitia "$FX/systemctl_show_activae.txt" 0

if [ "$MODUS" = "nginx_fractum" ]; then
    printf '##VILLA##nginx##\n'
    echo "nginx: [emerg] unexpected end of file, expecting \"}\""
    printf '##VILLA##fin##1##\n'
else
    sectio nginx "$FX/nginx_T.txt" 0
fi

sectio discus "$FX/df.txt" 0
sectio tempus "$FX/uptime.txt" 0

if [ "$MODUS" != "sectio_absens" ]; then
    sectio memoria "$FX/meminfo.txt" 0
fi

exit 0
