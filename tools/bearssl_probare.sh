#!/bin/bash
# tools/bearssl_probare.sh - porta 'bearssl' (tls-bearssl-plan T1):
#   I   integritas: summa arboris vendor/bearssl == VENDOR.md (editio
#       fortuita intra arborem upstream capitur)
#   II  KAT upstream contra AMALGAMAM: test_crypto all (LXXXVIII
#       algorithmi) + test_x509 (corpus catenarum) - amalgama renominata
#       eadem facit ac fontes separati
# Generatio ipsa (amalgama == generator hodie) = porta 'generata'.
# <porta titulus="bearssl" tegit="vendor/bearssl/* vendor/bearssl_amalgama.c vendor/bearssl_amalgama.h tools/bearssl_amalgamare.py tools/bearssl_amalgamare.sh"/>
#
# Usus: ./tools/bearssl_probare.sh          # porta
#       ./tools/bearssl_probare.sh -summa   # summam arboris imprimit
set -u
cd "$(dirname "$0")/.."
summa_arboris() {
    (cd vendor/bearssl && find . -type f ! -path ./VENDOR.md -print0 \
        | LC_ALL=C sort -z | xargs -0 shasum -a 256) \
        | shasum -a 256 | cut -d' ' -f1
}
if [ "${1:-}" = "-summa" ]; then
    summa_arboris
    exit 0
fi
fracta=0
scripta=$(sed -n 's/^summa: \([0-9a-f]\{64\}\)$/\1/p' vendor/bearssl/VENDOR.md 2>/dev/null)
vera=$(summa_arboris)
if [ -n "$scripta" ] && [ "$scripta" = "$vera" ]; then
    echo "  I   integritas arboris OK"
else
    echo "  I   FRACTUM: summa $vera != VENDOR.md '${scripta}'"
    fracta=1
fi
B=build/bearssl_probare
mkdir -p "$B"
if clang -std=c99 -O2 -c vendor/bearssl_amalgama.c -o "$B/bearssl.o" 2> "$B/fons.err" \
    && clang -std=c99 -O2 -Ivendor/bearssl/inc -Ivendor/bearssl/src \
        -o "$B/test_crypto" vendor/bearssl/test/test_crypto.c "$B/bearssl.o" 2> "$B/crypto.err" \
    && clang -std=c99 -O2 -Ivendor/bearssl/inc -Ivendor/bearssl/src \
        -o "$B/test_x509" vendor/bearssl/test/test_x509.c "$B/bearssl.o" 2> "$B/x509.err"; then
    if "$B/test_crypto" all > "$B/crypto.log" 2>&1; then
        echo "  II  test_crypto all OK ($(grep -c 'done' "$B/crypto.log") probationes)"
    else
        echo "  II  FRACTUM: test_crypto (vide $B/crypto.log)"
        tail -5 "$B/crypto.log"
        fracta=1
    fi
    if (cd vendor/bearssl && "../../$B/test_x509") > "$B/x509.log" 2>&1; then
        echo "  II  test_x509 OK"
    else
        echo "  II  FRACTUM: test_x509 (vide $B/x509.log)"
        tail -5 "$B/x509.log"
        fracta=1
    fi
else
    echo "  II  FRACTUM: aedificatio"
    cat "$B"/*.err | head -20
    fracta=1
fi
if [ "$fracta" -eq 0 ]; then
    echo "BEARSSL: sana"
    exit 0
fi
echo "BEARSSL: FRACTA"
exit 1
