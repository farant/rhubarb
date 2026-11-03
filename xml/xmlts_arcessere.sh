#!/bin/bash
# xml/xmlts_arcessere.sh - oraculum W3C (XML Conformance Test Suite
# 20130923) arcessere et probare (planum xml-arbor P1).
#
# Licentia collectionis Clarkianae redistributionem nisi arcae
# originalis vetat (xml/probationes/fixa/xmlts/PROVENIENTIA.md): ergo
# in repositorio solum manifestum (sha256 per plagulam) stat; plagulae
# in xml/build/xmlts/ (numquam commissae) vivunt.
#
# Usus: ./xml/xmlts_arcessere.sh               arcessit (si deest),
#                                               sigillum arcae probat,
#                                               explicat, manifestum
#                                               comparat
#       ./xml/xmlts_arcessere.sh -manifestum   manifestum ex plagulis
#                                               explicatis scribit
# Exitus 0 = suite adest et manifesto congruit; 1 = vitium (nuntiatur).
# Nihil delet: directorium explicatum quod manifesto non congruit
# manu removendum nuntiatur.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
URL="https://www.w3.org/XML/Test/xmlts20130923.tar.gz"
SIGILLUM="9b61db9f5dbffa545f4b8d78422167083a8568c59bd1129f94138f936cf6fc1f"
AEDES="$RADIX/xml/build"
ARCA="$AEDES/xmlts20130923.tar.gz"
EXPLICATA="$AEDES/xmlts"
MANIFESTUM="$RADIX/xml/probationes/fixa/xmlts.manifestum"

_sigillum () {
    shasum -a 256 "$1" | cut -d' ' -f1
}

_manifestum_scribere () {
    echo "# xmlts.manifestum - DATA: sigilla suitae (scribit xml/xmlts_arcessere.sh -manifestum)"
    echo "# Plagulae xmlts20130923 explicatae (xml/build/xmlts/); linea:"
    echo "# sha256<TAB>via relativa. Arca: $URL"
    echo "# sha256 arcae $SIGILLUM"
    # processus unus shasum pro omnibus (unus per plagulam: minuta)
    (cd "$EXPLICATA" && find . -type f -print0 | LC_ALL=C sort -z \
        | xargs -0 shasum -a 256 | sed 's#^\([0-9a-f]*\)  \./#\1	#')
}

mkdir -p "$AEDES" || exit 1
if [ ! -f "$ARCA" ]; then
    echo "arcesso $URL"
    curl -sS -L -o "$ARCA.novum" "$URL" || exit 1
    mv "$ARCA.novum" "$ARCA" || exit 1
fi
if [ "$(_sigillum "$ARCA")" != "$SIGILLUM" ]; then
    echo "xmlts: sigillum arcae non congruit ($ARCA) - remove manu et iterum curre" >&2
    exit 1
fi
if [ ! -d "$EXPLICATA" ]; then
    mkdir -p "$EXPLICATA.novum" || exit 1
    tar -xzf "$ARCA" -C "$EXPLICATA.novum" || exit 1
    mv "$EXPLICATA.novum" "$EXPLICATA" || exit 1
fi
if [ "${1:-}" = "-manifestum" ]; then
    _manifestum_scribere > "$MANIFESTUM" || exit 1
    echo "manifestum: $(grep -vc '^#' "$MANIFESTUM") plagulae -> $MANIFESTUM"
    exit 0
fi
if ! diff -q <(_manifestum_scribere | grep -v '^#') \
             <(grep -v '^#' "$MANIFESTUM") > /dev/null; then
    echo "xmlts: $EXPLICATA manifesto non congruit - remove manu et iterum curre" >&2
    exit 1
fi
echo "xmlts: $(grep -vc '^#' "$MANIFESTUM") plagulae congruunt ($EXPLICATA)"
