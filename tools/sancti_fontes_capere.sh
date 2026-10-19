#!/bin/bash
# tools/sancti_fontes_capere.sh - fontes bibliothecae sanctorum in
# speculum EXTRA repositorium (project-specs/sancti-plan.md par. I.3):
# ../sancti-fontes/<fons>/ . Textus OCR archive.org verbatim servatur
# (data non fida: nihil hic exsequitur, solum scribitur); paginae
# Encyclopaediae Catholicae per 'ce TITULUS' singulae capiuntur.
#
# Usus: ./tools/sancti_fontes_capere.sh            fontes fixi omnes
#       ./tools/sancti_fontes_capere.sh ce TITULUS  pagina CE 1913 una
#                                                   (titulus Wikisource)
#       ./tools/sancti_fontes_capere.sh ce-quaerere PRAEFIXUM
#                                                   tituli CE congruentes
# Exitus: 0 sanum · 1 captio fracta · 2 usus pravus
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
SPECULUM="${SANCTI_FONTES:-../sancti-fontes}"

capere () {   # capere VIA URL - semel; vacuum aut fractum non servatur
    local via="$1" url="$2"
    [ -s "$via" ] && return 0
    mkdir -p "$(dirname "$via")" || return 1
    if curl -sfL -m 300 --retry 3 -o "$via.tmp" "$url" && [ -s "$via.tmp" ]; then
        mv "$via.tmp" "$via"
    else
        rm -f "$via.tmp"
        echo "sancti_fontes_capere: FRACTUM $url" >&2
        return 1
    fi
}

if [ "${1:-}" = "ce-quaerere" ]; then
    # tituli articulorum CE 1913 praefixo incipientes (Wikisource)
    [ -n "${2:-}" ] || { echo "usus: $0 ce-quaerere PRAEFIXUM" >&2; exit 2; }
    curl -sfL -m 60 "https://en.wikisource.org/w/api.php?action=query&list=prefixsearch&pslimit=20&format=json&pssearch=Catholic_Encyclopedia_(1913)/$(python3 -I -c 'import sys,urllib.parse; print(urllib.parse.quote(sys.argv[1]))' "$2")" \
        | python3 -I -c 'import json,sys
for x in json.load(sys.stdin)["query"]["prefixsearch"]:
    print(x["title"].split("/", 1)[1])' || exit 1
    exit 0
fi
if [ "${1:-}" = "ce" ]; then
    [ -n "${2:-}" ] || { echo "usus: $0 ce TITULUS" >&2; exit 2; }
    t="${2// /_}"
    # titulus in nomen plagulae: '/' non licet. Articuli e paginis
    # scansis transcluduntur (action=raw solum <pages> dat): HTML
    # renderatum per api.php capitur, in textum planum redactum
    f="$SPECULUM/ce1913/$(printf '%s' "$t" | tr '/' '~').txt"
    [ -s "$f" ] && { echo "$f"; exit 0; }
    u="https://en.wikisource.org/w/api.php?action=parse&prop=text&formatversion=1&format=json&page=Catholic_Encyclopedia_(1913)/$(python3 -I -c 'import sys,urllib.parse; print(urllib.parse.quote(sys.argv[1]))' "$t")"
    capere "$f.json" "$u" || exit 1
    python3 -I tools/sancti_html_textus.py "$f.json" "$f" || { rm -f "$f" "$f.json"; exit 1; }
    rm -f "$f.json"
    echo "$f"
    exit 0
fi
[ $# -eq 0 ] || { echo "usus: $0 [ce TITULUS]" >&2; exit 2; }

fracta=0
A=https://archive.org/download
capere "$SPECULUM/mr1916/mr1916.txt" \
    "$A/romanmartyrology00cathuoft/romanmartyrology00cathuoft_djvu.txt" || fracta=1
capere "$SPECULUM/mr1902la/mr1902la.txt" \
    "$A/gtu_32400004276022/gtu_32400004276022_djvu.txt" || fracta=1
for v in 01 02 03 04 05 06 07 08 09 10 11 12; do
    id="livesoffathersma${v}butl_0"
    capere "$SPECULUM/butler1812/v$v.txt" "$A/$id/${id}_djvu.txt" || fracta=1
done
ls -la "$SPECULUM"/*/ | grep -c '\.txt$' | sed 's/^/textus: /'
exit $fracta
