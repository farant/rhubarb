#!/bin/bash
# Demo 117, Part E: the activation hash of demos 100 and 106.
#
# `mat3_activate` reads the signs of 36 integers as base-3 digits into an
# `int`: h = h * 3 + t. 3^36 is about 1.5e17, so every call overflows a
# signed int - undefined behaviour, which UBSan flags (main.c:386 in demo
# 100, main.c:682 in demo 106) and which clang in practice wraps mod 2^32.
# The wrapped value is then made positive (`if (h < 0) h = -h`) and reduced
# mod k. If the wrap lands on INT_MIN, -h is still negative and the cell
# index is negative.
#
# This script builds each demo UNMODIFIED at -O2 and PATCHED with a defined
# hash: the same base-3 digits in `unsigned int` (wraps mod 2^32 by the
# standard), the magnitude taken in unsigned arithmetic, then mod k. That
# equals the wrapped computation whenever the wrap does not land on
# INT_MIN; the patched copy prints INTMIN on stderr if it ever does. It
# compares the two outputs.
#
# Third, the INTENDED hash: H = sum t_i 3^(35-i) < 3^36 fits a 64-bit
# unsigned long exactly (LP64), and the cell is H mod k. Every k used is a
# power of two dividing 2^32, so the wrap alone loses nothing; what changes
# the cells is the sign fold, reachable only through the overflow. The
# intended hash has the negation symmetry H -> 3^36 - 1 - H, so AND3 and
# OR3 must score the same in demo 106's sweep; the script checks that.
#
# Usage: knotapel/demo_117_cyclotomic_audit/audit_hash.sh <work_dir>
set -u
KNOTAPEL="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="${1:?usage: audit_hash.sh <work_dir>}"
mkdir -p "$WORK" || exit 2
WORK="$(cd "$WORK" && pwd)"

unus() {
    local demo="$1" d="$WORK/$1"
    mkdir -p "$d" && cd "$d" || exit 2
    cp "$KNOTAPEL/$demo/main.c" orig.c
    sed -e '/^static int mat3_activate(const Mat3 \*m, int k_param) {$/,/^}$/{' \
        -e 's|^    int h = 0;$|    unsigned int u = 0;|' \
        -e 's|^        h = h \* 3 + (p\[i\] > 0 ? 2 : (p\[i\] < 0 ? 0 : 1));$|        u = u * 3u + (p[i] > 0 ? 2u : (p[i] < 0 ? 0u : 1u));|' \
        -e 's|^    if (h < 0) h = -h;$|    if (u == 0x80000000u) fprintf(stderr, "INTMIN\\n");\
    if (u > 0x7FFFFFFFu) u = 0u - u;|' \
        -e 's|^    return h % k_param;$|    return (int)(u % (unsigned int)k_param);|' \
        -e '}' orig.c > patched.c
    if [ "$(grep -c 'unsigned int u = 0;\|u = u \* 3u\|0u - u;\|u % (unsigned int)k_param' patched.c)" != 4 ]; then
        echo "$demo: patch did not apply"; exit 2
    fi
    sed -e '/^static int mat3_activate(const Mat3 \*m, int k_param) {$/,/^}$/{' \
        -e 's|^    int h = 0;$|    unsigned long h = 0;|' \
        -e 's|^        h = h \* 3 + (p\[i\] > 0 ? 2 : (p\[i\] < 0 ? 0 : 1));$|        h = h * 3ul + (p[i] > 0 ? 2ul : (p[i] < 0 ? 0ul : 1ul));|' \
        -e '/^    if (h < 0) h = -h;$/d' \
        -e 's|^    return h % k_param;$|    return (int)(h % (unsigned long)k_param);|' \
        -e '}' orig.c > exact.c
    if [ "$(grep -c 'unsigned long h = 0;\|h = h \* 3ul\|h % (unsigned long)k_param' exact.c)" != 3 ]; then
        echo "$demo: exact patch did not apply"; exit 2
    fi
    cc -std=c89 -O2 -o orig orig.c -lm && cc -std=c89 -O2 -o patched patched.c -lm \
        && cc -std=c89 -O2 -o exact exact.c -lm || exit 2
    ./orig > orig.log 2> orig.err; local rco=$?
    ./patched > patched.log 2> patched.err; local rcp=$?
    printf '%s: unmodified exit %s, patched exit %s, INT_MIN hashes %s; ' \
        "$demo" "$rco" "$rcp" "$(grep -c '^INTMIN$' patched.err)"
    if cmp -s orig.log patched.log; then
        echo "outputs identical ($(wc -l < orig.log | tr -d ' ') lines)"
    else
        echo "outputs DIFFER:"; diff orig.log patched.log | head -10
    fi
    ./exact > exact.log 2> exact.err; local rce=$?
    echo "$demo, intended hash: exit $rce, $(diff orig.log exact.log | grep -c '^<') of $(wc -l < orig.log | tr -d ' ') lines differ; tests: $(grep -o '[0-9]* pass, [0-9]* fail' orig.log | tail -1) -> $(grep -o '[0-9]* pass, [0-9]* fail' exact.log | tail -1)"
    local v
    for v in orig exact; do
        if grep -q '^  AND3 ' $v.log; then
            echo "  $v: $(grep '^  AND3 ' $v.log | awk '{print "AND3", $2}'), $(grep '^  OR3 ' $v.log | awk '{print "OR3", $2}')"
        fi
    done
}

for demo in demo_100_4strand_dkc demo_106_topological_entropy; do
    unus "$demo"
done
