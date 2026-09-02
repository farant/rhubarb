#!/bin/bash
# silva/aequivalentia.sh - aequivalentia parsurae octetim (RP 5.1)
#
# usus:
#   ./silva/aequivalentia.sh -basis            # documenta corporis condere
#   ./silva/aequivalentia.sh -conferre         # nunc contra basim conferre
#   ... [-corpus 'lib/*.c']                    # corpus (ordinarius lib/*.c)
#
# -basis: arbor.sh (documentum STML canonicum) super corpus ->
#   build/aequivalentia/basis/<nomen>.stml + .err, commissio notata.
# -conferre: arbor NUNC aedificatur (fontes praesentes), deinde CAPITA
#   quae a basi differunt (git diff basis -- '*.h', arbor operis inclusa)
#   ad textum basis in disco RESTITUUNTUR sub trap, binarium DIRECTE
#   curritur (involucrum per mtime reaedificaret capitibus veteribus),
#   capita reddita, documenta conferuntur. Plagulae .c corporis ipsae
#   mutatae ('mutatae ipsae') differre licet - nominantur, verdictum non
#   tangunt. Exitus 0 = ceterae omnes octetim eaedem | 1 differunt | 2
#   nihil actum.
#
# Lectio 2026-09-02: collatio inputum FIXUM tenere debet - capita
# edita in clausura omnium plagularum sunt, et differentia allocatoris
# a differentia inputus distingui non potest nisi textus capitum idem
# sit utrobique.
set -u
SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
cd "$RADIX_DIR"
modus=""; corpus='lib/*.c'
while [ $# -gt 0 ]; do
    case "$1" in
        -basis|-conferre) modus="$1"; shift ;;
        -corpus) corpus="$2"; shift 2 ;;
        *) echo "usus: aequivalentia.sh -basis|-conferre [-corpus 'lib/*.c']" >&2; exit 2 ;;
    esac
done
[ -n "$modus" ] || { echo "usus: aequivalentia.sh -basis|-conferre [-corpus 'lib/*.c']" >&2; exit 2; }
A="build/aequivalentia"
plagulae=$(ls $corpus 2>/dev/null)
[ -n "$plagulae" ] || { echo "aequivalentia: corpus vacuum: $corpus" >&2; exit 2; }
# arbor aedificatum ex fontibus PRAESENTIBUS (involucrum reaedificat)
./silva/arbor.sh lib/piscina.c -tacitus >/dev/null 2>&1 || { echo "aequivalentia: arbor non aedificatur" >&2; exit 2; }
ARBOR="silva/build/arbor"

capere () {   # <dir>
    local d="$1" f b
    mkdir -p "$d"
    for f in $plagulae; do
        b="$(basename "$f" .c)"
        "$ARBOR" "$f" -tacitus > "$d/$b.stml" 2> "$d/$b.err"
    done
}

if [ "$modus" = "-basis" ]; then
    rm -rf "$A/basis"; mkdir -p "$A/basis"
    capere "$A/basis"
    git rev-parse HEAD > "$A/basis/commissio"
    [ -z "$(git status --porcelain -- include silva/fontes lib 2>/dev/null)" ] && echo munda > "$A/basis/arbor_operis" || echo sordida > "$A/basis/arbor_operis"
    echo "aequivalentia: basis condita ($(echo "$plagulae" | wc -w | tr -d ' ') documenta, commissio $(cut -c1-8 "$A/basis/commissio"), arbor operis $(cat "$A/basis/arbor_operis"))"
    exit 0
fi

[ -f "$A/basis/commissio" ] || { echo "aequivalentia: basis absens - primum -basis" >&2; exit 2; }
basis=$(cat "$A/basis/commissio")
rm -rf "$A/post" "$A/capita_nunc"; mkdir -p "$A/post" "$A/capita_nunc"
# capita mutata contra basim (commissiones ET arbor operis)
capita=$(git diff --name-only "$basis" -- '*.h' 2>/dev/null | grep -v '^archivum/' || true)
restituere () {
    local h
    for h in $capita; do
        [ -f "$A/capita_nunc/$h" ] && { mkdir -p "$(dirname "$h")"; cp "$A/capita_nunc/$h" "$h"; }
    done
}
if [ -n "$capita" ]; then
    trap restituere EXIT
    for h in $capita; do
        if [ -f "$h" ]; then mkdir -p "$A/capita_nunc/$(dirname "$h")"; cp "$h" "$A/capita_nunc/$h"; fi
        if git cat-file -e "$basis:$h" 2>/dev/null; then
            git show "$basis:$h" > "$h"
        else
            echo "aequivalentia: caput NOVUM (non in basi): $h - documenta eius clausurae differre possunt" >&2
        fi
    done
    echo "aequivalentia: capita ad basim restituta dum curritur: $(echo $capita | wc -w | tr -d ' ')"
fi
capere "$A/post"
restituere; trap - EXIT
# verificatio restitutionis
for h in $capita; do
    [ -f "$A/capita_nunc/$h" ] && ! cmp -s "$A/capita_nunc/$h" "$h" && { echo "aequivalentia: RESTITUTIO FRACTA: $h" >&2; exit 2; }
done
mutatae=$(git diff --name-only "$basis" -- $corpus 2>/dev/null || true)
eaedem=0; diversae=""; ipsae=""
for f in $plagulae; do
    b="$(basename "$f" .c)"
    if cmp -s "$A/basis/$b.stml" "$A/post/$b.stml" && cmp -s "$A/basis/$b.err" "$A/post/$b.err"; then
        eaedem=$((eaedem + 1))
    elif echo "$mutatae" | grep -qx "$f"; then
        ipsae="$ipsae $f"
    else
        diversae="$diversae $f"
    fi
done
totae=$(echo "$plagulae" | wc -w | tr -d ' ')
echo "aequivalentia contra $(cut -c1-8 "$A/basis/commissio"): $eaedem/$totae octetim eaedem"
[ -n "$ipsae" ] && echo "  mutatae ipsae (differre licet):$ipsae"
if [ -n "$diversae" ]; then
    echo "  DIVERSAE (inputu fixo):$diversae"
    echo "aequivalentia: FRACTA"; exit 1
fi
echo "aequivalentia: sana"; exit 0
