#!/bin/bash
# tools/musicxml_arcessere.sh - fontes externi MusicXML (plan musicxml P7/P8)
#
#   -suita     suita W3C (MIT) ad commissum fixum in build/musicxml/suite_tar/
#              arcessit, sha256 probat, cum probationes/fixa/musicxml/suite/
#              (INDEX) confert - fasciculi commissi iam adsunt; hoc
#              provenientiam renovat aut probat.
#
# Fasciculi externi = data non fida: directorium proprium, nihil
# exsequitur. Exitus 0 sanum, 1 fractum, 2 usus.
set -euo pipefail
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SUITA_COMMISSUM=77c19f7e819154c70ca1a1992e80dcda8ff82fea
SUITA_SHA=5717b897bbc64e7f3b882618f0b60f421b36fe008f83867da42adeb32d620cd4

suita () {
    local d="$RADIX/build/musicxml/suite_tar"
    rm -rf "$d" && mkdir -p "$d/x"
    curl -sS -L -m 120 -o "$d/suite.tar.gz" \
        "https://codeload.github.com/w3c-cg/musicxmlTestSuite/tar.gz/$SUITA_COMMISSUM"
    local h
    h=$(shasum -a 256 "$d/suite.tar.gz" | cut -d' ' -f1)
    if [ "$h" != "$SUITA_SHA" ]; then
        echo "MUSICXML ARCESSERE: sha256 tarballi discrepat ($h)" >&2
        return 1
    fi
    tar -xzf "$d/suite.tar.gz" -C "$d/x"
    local fixa="$RADIX/probationes/fixa/musicxml/suite"
    if ( cd "$d"/x/*/xmlFiles && ls | LC_ALL=C sort | tr '\n' '\0' \
            | xargs -0 shasum -a 256 ) | diff -q - <(sed 1d "$fixa/INDEX") >/dev/null; then
        echo "MUSICXML ARCESSERE: suita congruit cum fixis commissis (183)"
    else
        echo "MUSICXML ARCESSERE: suita a fixis commissis DIFFERT" >&2
        return 1
    fi
}

case "${1:-}" in
    -suita) suita ;;
    *) echo "usus: $0 -suita" >&2; exit 2 ;;
esac
