#!/bin/sh
# stml_aurea_generare.sh - aurum pontis stml_textus_internus
# (re)generare. Vide probationes/probatio_stml_aurea.c pro doctrina:
# aurum est TESTIMONIUM moris veteris - numquam regenerare sine
# iudicio divergentiarum (diff .aurum .novum) contra spec
# project-specs/stml-trivia-spec.md.
#
# PRIVATIO NOMINATA: monstra fontium >= 1MB exclusa (librarium bis,
# gutenberg_index) - pretium suitae quaque cursione vincit.
set -e
cd "$(dirname "$0")/.."

BIN=build/probationes/probatio_stml_aurea
if [ ! -x "$BIN" ]; then
    echo "probatio_stml_aurea non aedificata - curre:" >&2
    echo "  ./compile_tests.sh stml_aurea" >&2
    exit 2
fi

mkdir -p probationes/fixa/stml_aurea

VIAE=$(find . \( -name '*.stml' -o -name '*.census' -o -name '*.canon' \) \
    -not -path './build/*' -not -path './.git/*' \
    -size -1000000c | LC_ALL=C sort)

if [ -z "$VIAE" ]; then
    echo "nullae viae fontium inventae - aliquid valde mali" >&2
    exit 2
fi

EXCLUSA=$(find . \( -name '*.stml' -o -name '*.census' -o -name '*.canon' \) \
    -not -path './build/*' -not -path './.git/*' \
    -size +999999c | LC_ALL=C sort)
if [ -n "$EXCLUSA" ]; then
    echo "monstra exclusa (>= 1MB):"
    echo "$EXCLUSA" | sed 's/^/  /'
fi

# shellcheck disable=SC2086
exec "$BIN" -generare $VIAE
