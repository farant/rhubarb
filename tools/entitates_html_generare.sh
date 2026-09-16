#!/bin/bash

# tools/entitates_html_generare.sh - tabula entitatum HTML5 GENERATA
#
# lib/entitates_html_tabula.c ex html.entities.html5 Pythonis (copia
# stdlib ipsius WHATWG entities.json - sine rete, deterministica):
# MMCCXXXI tituli ('amp;' et formae veteres 'amp' sine ';'), octeti
# UTF-8 (puncta I aut II), ordine octetorum titulorum (quaestio
# binaria in lib/entitates_html.c). Consumentes: md_decoctum
# (CommonMark: titulus cum ';' solus) et html_coctum (HTML5: forma
# longissima, veteres inclusae). Natum O2b-4 (2026-09-15).
#
# Usus: ./tools/entitates_html_generare.sh            # scribit
#       ./tools/entitates_html_generare.sh -probare   # recens? 0/1

set -u
export LC_ALL=C
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

EXITUS="lib/entitates_html_tabula.c"
PROBARE=0
for arg in "$@"; do
    case "$arg" in
        -probare) PROBARE=1 ;;
        *) echo "usus: entitates_html_generare.sh [-probare]" >&2
           exit 2 ;;
    esac
done

generare () {
python3 - <<'EOF'
import html.entities

t = html.entities.html5
rows = sorted(t.items())
out = []
out.append('/* entitates_html_tabula.c - GENERATA '
           '(tools/entitates_html_generare.sh)')
out.append(' * ex html.entities.html5 Pythonis (WHATWG entities.json).')
out.append(' * NOLI MANU EDERE - regenera et confer (-probare). Ordine')
out.append(' * octetorum titulorum: quaestio binaria in entitates_html.c.')
out.append(' */')
out.append('')
out.append('#include "entitates_html_tabula.h"')
out.append('')
out.append('constans EntitasHtml ENTITATES_HTML[] = {')
for k, v in rows:
    b = v.encode('utf-8')
    octeti = ''.join('\\x%02X' % x for x in b)
    out.append('    { "%s", "%s", %d },' % (k, octeti, len(b)))
out[-1] = out[-1].rstrip(',')
out.append('};')
out.append('')
out.append('constans i32 ENTITATES_HTML_NUMERUS = (i32)%d;' % len(rows))
out.append('')
print('\n'.join(out))
EOF
}

if [ "$PROBARE" = "1" ]; then
    if [ ! -f "$EXITUS" ]; then
        echo "entitates_html: $EXITUS abest - RANCIDUM" >&2
        exit 1
    fi
    if diff -q <(generare) "$EXITUS" >/dev/null; then
        echo "entitates_html: recens ($EXITUS)"
        exit 0
    fi
    echo "entitates_html: RANCIDUM - regenera: $0" >&2
    exit 1
fi

generare > "$EXITUS"
echo "entitates_html: scriptum $EXITUS ($(grep -c '^    { "' "$EXITUS") tituli)"
