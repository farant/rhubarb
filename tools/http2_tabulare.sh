#!/bin/bash
# tools/http2_tabulare.sh - tabulae HTTP/2 pro probatio_http2_reproductio
# (http2-plan C5, T8/T9).
#
# Casus salve, eco, multae, magnum (CCLVI KiB), abire per
# probatio_http2_nexus currit cum HTTP2_TABULAE: octeti decrypti
# utriusque partis (tap nexus: <casus>.servus.bin, <casus>.cliens.bin)
# et tabula formarum servi Go (GODEBUG=http2debug=2: <casus>.go.log).
# Deinde in probationes/fixa/http2/ ponit cum ORIGO.txt (mandatum,
# go version, dies). Go solum hic opus est - suitae ordinariae tabulas
# sine Go legunt (spec D9).
#
# NON GENERATUM byte pro byte: servus Go caput 'date' mittit, ergo
# tabula nova numquam eadem est - ideo ORIGO.txt, non linea GENERATUM.
#
# Usus:
#   ./tools/http2_tabulare.sh             tabulas in fixis renovat
#   ./tools/http2_tabulare.sh -directorium <dir>
#                                         tabulat in <dir> (fixa intacta)
#   ./tools/http2_tabulare.sh -conferre   tabulat in directorium
#                                         temporarium et probatio_http2_
#                                         reproductio in eo currit
#                                         (HTTP2_FIXA): tabulae recentes
#                                         idem probant ac fixa. Ordo
#                                         formarum Go non stabilis (ACK et
#                                         WINDOW_UPDATE permutantur), ergo
#                                         iudicium per suitam, non per
#                                         comparationem octetorum.
# Exitus: 0 factum | 1 defectus | 2 NIHIL CURSUM (go aut binarium deest)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2

CASUS="salve eco multae magnum abire"
FIXA=probationes/fixa/http2
BINARIUM=build/probationes/probatio_http2_nexus
META=""
CONFERRE=0

case "${1:-}" in
    "") META="$FIXA" ;;
    -conferre) CONFERRE=1; META=$(mktemp -d "${TMPDIR:-/tmp}/http2_conferre.XXXXXX") || exit 1 ;;
    -directorium) META="${2:-}"; [ -n "$META" ] || { echo "usus: $0 [-directorium <dir>]" >&2; exit 2; } ;;
    *) echo "usus: $0 [-directorium <dir> | -conferre]" >&2; exit 2 ;;
esac

GO=""
for g in /usr/local/go/bin/go /opt/homebrew/bin/go /usr/bin/go; do
    [ -x "$g" ] && { GO="$g"; break; }
done
[ -n "$GO" ] || { echo "http2_tabulare: NIHIL CURSUM - go deest" >&2; exit 2; }

if [ ! -x "$BINARIUM" ]; then
    ./compile_tests.sh http2_nexus > /dev/null 2>&1
fi
[ -x "$BINARIUM" ] || { echo "http2_tabulare: NIHIL CURSUM - $BINARIUM deest" >&2; exit 2; }

TEMP=$(mktemp -d "${TMPDIR:-/tmp}/http2_tabulae.XXXXXX") || exit 1
if ! HTTP2_TABULAE="$TEMP" "$BINARIUM" > "$TEMP/cursus.log" 2>&1; then
    tail -20 "$TEMP/cursus.log" >&2
    echo "http2_tabulare: suita fracta (tabulae non positae; $TEMP)" >&2
    exit 1
fi
for c in $CASUS; do
    for f in "$c.servus.bin" "$c.cliens.bin" "$c.go.log"; do
        [ -s "$TEMP/$f" ] || { echo "http2_tabulare: $f deest aut vacuum ($TEMP)" >&2; exit 1; }
    done
done

mkdir -p "$META" || exit 1
for c in $CASUS; do
    cp "$TEMP/$c.servus.bin" "$TEMP/$c.cliens.bin" "$TEMP/$c.go.log" "$META/" || exit 1
done
{
    echo "tabulae HTTP/2 - ab tools/http2_tabulare.sh scriptae (http2-plan C5)"
    echo ""
    echo "dies:     $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
    echo "go:       $("$GO" version)"
    echo "servus:   tools/oracula/http2_servus.go (bibliotheca norma sola)"
    echo "cliens:   probatio_http2_nexus (HTTP2_TABULAE), commit $(git rev-parse --short HEAD 2>/dev/null)"
    echo "casus:    $CASUS"
    echo ""
    echo "<casus>.servus.bin   octeti a servo (decrypti, post TLS)"
    echo "<casus>.cliens.bin   octeti a cliente (praefatio primum)"
    echo "<casus>.go.log       GODEBUG=http2debug=2 (formae a servo scriptae/lectae)"
    echo ""
    echo "Non byte pro byte iterabiles (caput 'date' servi): renovare per"
    echo "./tools/http2_tabulare.sh; probatio_http2_reproductio eas legit sine Go."
} > "$META/ORIGO.txt"
rm -rf "$TEMP"
echo "http2_tabulare: $CASUS -> $META"

if [ "$CONFERRE" = 1 ]; then
    REPRODUCTIO=build/probationes/probatio_http2_reproductio
    if [ ! -x "$REPRODUCTIO" ]; then
        ./compile_tests.sh http2_reproductio > /dev/null 2>&1
    fi
    [ -x "$REPRODUCTIO" ] || { echo "http2_tabulare: $REPRODUCTIO deest" >&2; exit 2; }
    if HTTP2_FIXA="$META" "$REPRODUCTIO" > "$META/reproductio.log" 2>&1; then
        echo "http2_tabulare: -conferre - tabulae recentes probantur ($META)"
        rm -rf "$META"
        exit 0
    fi
    grep -E 'FRACTA|forma ' "$META/reproductio.log" | head -20 >&2
    echo "http2_tabulare: -conferre FRACTUM - tabulae recentes discrepant ($META)" >&2
    exit 1
fi
